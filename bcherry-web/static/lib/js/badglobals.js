/*jslint white: true, onevar: true, devel: true, undef: true, nomen: true, eqeqeq: true, plusplus: true, bitwise: true, regexp: true, strict: false, newcap: true, immed: true */
/*globals window: false */

// Include this script BEFORE any scripts you want to scan for bad globals.
// After the scripts you want to scan, run BADGLOBALS.check() and watch firebug.
var BADGLOBALS = (function (window) {
	var report = {
			badGlobals: []
		},		
		skip = {},
		that = {
			check: function () {
				var prop,
					val;
				
				report.badGlobals = [];
				for (prop in window) {
					if (window.hasOwnProperty(prop) && !skip.hasOwnProperty(prop)) {
						val = window[prop];
						if (delete window[prop]) {
							console.warn("Found non-var global %o with value %o", prop, val);
							report.badGlobals.push(prop);
							try {
								window[prop] = val;
							} catch (e) {
								console.error("Oops, there was an error putting %o back :(", prop);
							}
						}
					}
				}
			},
			report: function () {
				return report;
			}
		},		
		prop;
	
	for (prop in window) {
		if (window.hasOwnProperty(prop)) {
			skip[prop] = 1;
		}
	}
	
	return that;
}(window));