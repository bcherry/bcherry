;(function(){
	var _on = true;
	var protectedMethods = ["setOn", "extend", "turnOff"];

	var _AGD = function(prefix, on) {
		if (on === undefined) {
			on = _on;
		}

		var _prefix = prefix || "";
		prefix = function(str) { 
			var space = "";
			if (prefix) {
				space = " ";
			}
			return _prefix + space + str;
		};

		this.log = function() {
			arguments[0] = prefix(arguments[0]);
			console.log.apply(console, arguments);
		};

		this.dir = function(obj, msg) {
			if (msg !== undefined) {
				this.log.apply(arguments.caller, Array.prototype.slice.call(arguments, 1));
			}
			console.dir.call(console, obj);
		};

		this.warn = function() {
			arguments[0] = prefix(arguments[0]);
			console.warn.apply(console, arguments);
		};

		this.assert = function() {
			arguments[1] = prefix(arguments[1]);
			console.assert.apply(console, arguments);
		};

		this.error = function() {
			arguments[0] = prefix(arguments[0]);
			console.error.apply(console, arguments);
		};

		for (var prop in this) {
			if (this[prop].constructor != Function) {
				continue;
			}

			var fn = function(fn) {
				return function() {
					//TODO: better check and add more supported browsers
					if (window.console && ("firebug" in window.console || "chromium" in window)) {
						return fn.apply(this, arguments);
					}
				};
			};

			this[prop] = fn(this[prop]);
		}

		this.turnOff = function() {
			for (var prop in this) {
				if (this[prop].constructor == Function && !(prop in protectedMethods)) {
					this[prop] = function() {return;};
				}
			}
		};

		this.setOn = function(on) {
			if (on !== undefined && !on) {
				agd.turnOff();
			}
		};

		this.extend = function(prefix, on) {
			var sep = "";
			if (_prefix) {
				sep = "::";
			}
			return new _AGD(_prefix + sep + prefix, on);
		};

		this.rebase = function(prefix) {
			_prefix = prefix;
		};

		if (on !== undefined && !on) {
			this.turnOff();
		}
	};

	var agd = new _AGD();

	var _setOn = agd.setOn;
	agd.setOn = function(on) {
		_on = !!on;
		_setOn(on);
	};


	window.AGD = agd;
})();
