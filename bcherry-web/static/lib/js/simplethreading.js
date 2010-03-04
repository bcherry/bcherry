/*jslint white: true, onevar: true, browser: true, devel: true, undef: true, nomen: true, eqeqeq: true, plusplus: true, bitwise: true, regexp: true, strict: true, newcap: true, immed: true */
/*globals window: false, SimpleThread: false */
"use strict";
var simpleThreading = (function (window, console) {
	var my = {},
		threads = [],
		timer,
		
		// co-dependent functions
		process,
		iterate;
	
	function log() {
		if (console) {
			console.log.apply(this, Array.prototype.slice.call(arguments));
		}
	}
	
	function add(thread) {
		threads.push(thread);
		
		// to ensure we're running
		my.run();
	}
	
	function remove(thread) {
		var index,
			i,
			l;
		
		// Modern browsers have an indexOf method for arrays
		if (Array.prototype.indexOf) {
			index = threads.indexOf(thread);
		
		// But Internet Explorer does not
		} else {
			index = -1;
			for (i = 0, l = threads.length; i < l; i += 1) {
				if (threads[i] === thread) {
					index = i;
					break;
				}
			}
		}
		
		if (index < 0) {
			return false; // thread was not found
		}
		
		// At least IE has splice though
		threads.splice(index, 1);
		
		return true;
	}
	
	my.make = function (func, options) {
		var priv,
			pub;
		
		options = options || {};
		
		priv = {
			func: func,
			options: options
		};
		
		pub = {
			start: function () {
				add(priv);
			},
			stop: function () {
				remove(priv);
			}
		};
		
		if (options.autoStart !== false) {
			pub.start();
		}
		
		return pub;
	};
	
	process = function () {
		var i,
			l,
			thread,
			newThreads = [];
		
		for (i = 0, l = threads.length; i < l; i += 1) {
			thread = threads[i];
			if (thread.func() !== false) {
				newThreads.push(thread);
			}
		}
		
		threads = newThreads;
		
		iterate();
	};
	
	iterate = function () {
		if (threads.length) {
			timer = setTimeout(process, 0);
		} else {
			timer = undefined;
		}
	};
	
	my.run = function () {
		if (!timer) {
			iterate();
		}
	};
	
	my.pauseAll = function () {
		if (timer) {
			clearTimeout(timer);
			timer = undefined;
		}
	};
	
	return my;
}(window, window.console));
