// example code for building a robust asynchronous chainer

// $ is a module
var $ = (function () {
	// only done like this to count up calls to $() for logging purposes
	var dollars = 0;
	
	// our root $ object, which is a function
	function $() {
		// count of chain segments
		var count = 0;
		// counting calls to $()
		var d = dollars++;
		
		// The real underlying object
		// could be prototype-based or whatever, just needs to be an object
		var me = {
			// a function on our object
			// can take whatever args it wants
			// this is an asynchronous function
			foo: function () {
				// just counting segments
				var c = count;
				log("%o, %o foo started", d, c);
				// async
				timeout(function () {
					log("%o, %o foo ended", d, c);
					
					// this is important, call done to trigger the next segment
					done();
				});
				// more counting
				count += 1;
			},
			// a synchronous function
			bar: function () {
				var c = count;
				log("%o, %o bar started", d, c);
				log("%o, %o bar ended", d, c);
				// we call done immediately to move to the next segment
				done();
				count += 1;
			},
			// toggleable between wait/no-wait (takes a param)
			// defaults to not waiting
			baz: function (wait) {
				var c = count;
				log("%o, %o baz started", d, c);
				timeout(function () {
					log("%o, %o baz ended", d, c);
					
					// if we were waiting, trigger the next segment
					if (wait) {
						done();
					}
				});
				
				// if we're not waiting, just move straight to the next segment
				if (!wait) {
					done();
				}
				count += 1;
			},
			// this function reads and modifies a property on the object asynchronously
			spam: function () {
				var c = count,
					that = this;
				
				log("%o, %o spam started this.val is %o", d, c, that.val);
				timeout(function () {
					that.val++;
					log("%o, %o spam ended this.val is %o", d, c, that.val);
					done();
				});
				
				count += 1;
			},
			// just a property
			val: 0
		};
		
		// the function queue
		var queue = [];
		
		// pushes a function to the queue
		// if the queue was empty, this function is called as well
		function enqueue(func) {
			queue.push(func);
			
			// if we had an empty queue, run our function
			if (queue.length === 1) {
				func();
			}
		}
		
		// removes the function on the front of the queue
		// if any are left, calls the next one
		function done() {
			var f = queue.shift();
			if (queue.length > 0) {
				queue[0]();
			}
		}
		
		// building the proxied object
		
		// empty constructor to attach a prototype
		function P() {
			return this;
		}

		// our proxy will have the original object
		// as its prototype to pass-through normal values
		P.prototype = me;
		var proxy = new P();

		// find all the functions on the original and proxy them on the proxy
		for (var prop in me) {
			if (typeof me[prop] === "function") {
				// closure on `prop` to protect against the loop
				(function (prop) {
					// building the proxied function
					proxy[prop] = function () {
						var args = Array.prototype.slice.call(arguments);
						// func that wraps the original function
						function call() {
							me[prop].apply(me, args);
						}
						
						// push the wrapper to the queue
						enqueue(call);

						// return the proxy again (enables chaining)
						return proxy;
					};
				}(prop));
			}
		}
		
		// return the proxy, not the original
		return proxy;
	}
	
	// a static function on $
	$.version = function () {
		return "basic chainer 1.0";
	};
	
	// return our $ module
	return $;
	
	// simple console.log helper that falls back to writing HTML if firebug is not there
	function log() {
		if (window.console && console.log) {
			console.log.apply(console, Array.prototype.slice.call(arguments));
		} else {
			document.getElementsByTagName("body")[0].innerHTML += "<p>" + Array.prototype.join.call(arguments, " ") + "</p>";
		}
	}

	// execute a func with a random <1s delay
	function timeout(func) {
		var time = Math.random() * 1000;
		setTimeout(func, time);
	}
	
}());

// bunch of tests
$().foo().bar().baz().spam();
// $().foo().bar();
// $().foo().bar().baz().bar().foo();
// $().foo().bar();
// $().foo().bar().baz().bar().foo();
// $().foo().bar();