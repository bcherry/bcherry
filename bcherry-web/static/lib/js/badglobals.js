/*jslint white: true, onevar: true, devel: true, undef: true, nomen: true, eqeqeq: true, plusplus: true, bitwise: true, regexp: true, strict: false, newcap: true, immed: true */
/*globals window: false */

// Include this script BEFORE any scripts you want to scan for bad globals.
// After the scripts you want to scan, run BADGLOBALS.check() and watch firebug.
// This script will only work in Firefox or Chrome, NOT in Internet Explorer.
var BADGLOBALS = (function (window) {
	var report,		
		skip = [],
		that = {
			check: function (exclude) {
				var prop,
					val,
					bad = [],
					good = [],
					skipped = [];
				
				exclude = exclude || [];
				
				for (prop in window) {
					if (window.hasOwnProperty(prop)) {
						if (skip.indexOf(prop) >= 0 || exclude.indexOf(prop) >= 0) {
							skipped.push(prop);
						} else {
							val = window[prop];
							if (delete window[prop]) {
								console.warn("Found non-var global %o with value %o", prop, val);
								bad.push(prop);
								try {
									window[prop] = val;
								} catch (e) {
									console.error("Oops, there was an error putting %o back :(", prop);
								}
							} else {
								good.push(prop);
							}
						}
					}
				}
				report = {
					bad: bad,
					good: good,
					skipped: skipped
				};
			},
			exclude: function () {
				var i = 0,
					l = arguments.length;
				
				for (; i < l; i += 1) {
					skip.push(arguments[i]);
				}
			},
			report: function () {
				if (!report) {
					that.check();
				}
				return report;
			}
		},
		prop;
	
	for (prop in window) {
		if (window.hasOwnProperty(prop)) {
			skip.push(prop);
		}
	}
	
	return that;
}(window));