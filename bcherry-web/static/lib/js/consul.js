;(function(){
	var _consul= function() {
		
		// consul defaults to on, feel free to change this
		var on = true;
		
		// This function can turn consul on or off (default is on)
		this.turnOn = function(bool) { on = (bool === undefined || bool); };
		
		// These are the browser JS consoles that consul supports
		// Each tool needs two fields:
		// 		bit - A unique int, so we can pack browser support bitwise into an integer
		//		detect - A function that checks for the presence of the debugging tools
		var tools = {
			// Firebug for Firefox, on all platforms (not Firebug Lite for other browsers)
			"firebug"		: { bit : 1, detect : function() { return window.console && "firebug" in window.console; } },
			
			// Google Chrome and Chromium for Windows, Mac, and Linux
			"chromium"		: { bit : 2, detect : function() { return window.console && "chromium" in window; } },
			
			// Internet Explorer, provided window.console exists (IE8+ I think, this might also catch Firebug Lite actually)
			"ie"			: { bit : 3, detect : function() { return /MSIE/.test(navigator.userAgent) && window.console; } },
			
			// Opera on all platforms.  Dragonfly does not yet support console.*, but shows messages from opera.postError
			"opera"			: { bit : 4, detect : function() { return "opera" in window && "postError" in window.opera; } },
			
			// Apple Safari, on Mac and Windows
			"safari"		: { bit : 5, detect : function() { return /Safari/.test(navigator.userAgent) && window.console; } },
			
			// Firebug Lite, a cross-browser implementation of the basic Firebug features.  Could break on versions < 1.2.
			"firebuglite"	: { bit : 6, detect : function() { return "firebug" in window; } }
		};
		
		// Takes an arbitrary number of browser ids (like above), and returns an int representing all of them
		var b = function() {
			var args = Array.prototype.slice.call(arguments);
			var val = 0;
			for (var i = 0, n = args.length; i < n; i++) {
				val += (1 << tools[args[i]].bit);
			}
			return val;
		};
		
		// These are the functions that will be ported from window.console to our console
		// Each entry should use the b() function above to mark browsers which support it
		var consoleFuncs = {
			log			: b("firebug", "firebuglite", "chromium", "safari", "ie", "opera"),
			debug		: b("firebug", "firebuglite", "chromium", "safari"),
			info		: b("firebug", "firebuglite", "chromium", "safari", "ie", "opera"),
			warn		: b("firebug", "firebuglite", "chromium", "safari", "ie", "opera"),
			error		: b("firebug", "firebuglite", "chromium", "safari", "ie", "opera"),
			assert		: b("firebug", "firebuglite", "chromium", "safari", "ie", "opera"),
			dir			: b("firebug", "firebuglite", "chromium", "safari"),
			dirxml		: b("firebug", "firebuglite", "chromium", "safari"),
			trace		: b("firebug", "firebuglite", "chromium", "safari"),
			group		: b("firebug", "firebuglite", "chromium", "safari"),
			groupEnd	: b("firebug", "firebuglite", "chromium", "safari"),
			time		: b("firebug", "firebuglite", "chromium", "safari"),
			timeEnd		: b("firebug", "firebuglite", "chromium", "safari"),
			count		: b("firebug", "firebuglite", "chromium", "safari")
		};
		
		// Tries to detect a browser which supports the given function, returning the result
		var isSupported = function(func) {
			for (var prop in tools) {
				if (tools.hasOwnProperty(prop)) {
					var tool = tools[prop];
					if (tool.detect() && (consoleFuncs[func] & (1 << tool.bit))) {
						return true;
					}
				}
			}
			return false;
		};
		
		// Wraps a given function from window.console with browser and on/off detection
		var generateFn = function(prop) {
			return function() {
				// So we can muck with this value without clobbering the closure for future calls
				var func = prop;
				if (on && isSupported(func)) {
					// This method is supported by our available tools, so let's work through them in order of priority
					if (tools.firebug.detect() || tools.chromium.detect() || tools.safari.detect()) {
						// For browsers that don't suck, just re-apply the arguments to the desired window.console method
						window.console[func].apply(window.console, arguments);
					} else if (tools.firebuglite.detect()) {
						// Firebug Lite doesn't suck, but it uses firebug.d.console instead of window.console
						window.firebug.d.console.cmd[func].apply(window.firebug.d.console, arguments);
					} else if (tools.opera.detect()) {
						// Opera doesn't suck, per-se, but it also doesn't have any console.* methods AT ALL
						var args = arguments;
						if (func == "assert") {
							// For assert, we need to implement this method here
							if (args.length === 0 || !(args[0])) {
								// Assertion failed, we set the func to that for messaging, and remove the first argument
								func = "Assertion Failed";
								args = Array.prototype.slice.call(args, 1);
								// default text if none specified
								if (args.length === 0) {
									args = [func];
								}
							} else {
								// Assertion passed, don't log anything
								return;
							}
						}
						// Now, to post the message, we just pass everything along to opera.postError, but prepended with the method name
						window.opera.postError(func.toUpperCase() + ": " + Array.prototype.join.call(args, ', '));
					} else if (tools.ie.detect()) {
						// For IE dev toolbar, window.console methods aren't real functions, or something.  And they have shitty features.
						// ...so just string-concatenate all the arguments and pass that along to the right "function"
						if (func == "assert") {
							// For assert, we need to pass along the first argument on its own
							var test = arguments.length > 0 ? arguments[0] : false;
							var msg = "Assertion Failed: " + Array.prototype.slice.call(arguments, 1).join(', ');
							window.console[func](test, msg);
						} else {
							// Otherwise, just concat and call
							window.console[func](Array.prototype.join.call(arguments, ', '));
						}
					}
				}
			};
		};
		
		// Loop through our defined functions and assign them
		for (var prop in consoleFuncs) {
			// Skip extra Object.prototype properties
			if (consoleFuncs.hasOwnProperty(prop)) {
				// Apply our generator (this allows us to avoid closure issues with the loop)
				this[prop] = generateFn(prop);
			}
		}
	};

	// Assign to window.consul
	window.consul = new _consul();
})();
