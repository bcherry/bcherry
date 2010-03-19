// Tests
//	Default value variations
//		x || default
//		x != null ? x : default
//		(x !== undefined && x !== null) ? x : default
//		x !== undefined ? x : default
//		typeof x !== "undefined" ? x : default
//		object properties
//	How the above perform with 1, 5, 10, 100, 1000 parameters
//	How the above perform at different scope depths

var log = (function () {
	var lastTime = (new Date).getTime();
	
	return function (msg) {
		var newTime = (new Date).getTime(),
			string = Array.prototype.join.call(arguments, " ") + " | Time Elapsed: " + ((newTime - lastTime));
		
		$("body").append(string + "<br />");
		lastTime = newTime;
	};
}());

var tests = {};

tests.defaultParameters = function () {
	var funcs = {
			eqUndefined: function(x) {
				return x !== undefined ? x : "default";
			},
			
			eqNullAndEqUndefined: function(x) {
				return x !== undefined && x !== null ? x : "default";
			},
			
			eqNull: function(x) {
				return x != null ? x : "default";
			},
	
			typeofEqUndefined: function(x) {
				return typeof x !== "undefined" ? x : "default";
			},
	
			orDefault: function(x) {
				return x || "default";
			}
		},
		key,
		i,
		iterations = 10000000,
		func;
	
	for (key in funcs) {
		if (funcs.hasOwnProperty(key)) {
			func = funcs[key];
			log("starting value passed test of ", key);
			for (i = 0; i < iterations; i += 1) {
				func(10);
			}
			log("ending value passed test of ", key);
			log("starting no value passed test of ", key);
			for (i = 0; i < iterations; i += 1) {
				func();
			}
			log("ending no value passed test of ", key);
		}
	}
	(function () {
		(function () {
			(function () {
				(function () {
					(function () {
						var func = funcs.eqUndefined;
						log("starting deep scope test (no value)");
						for (i = 0; i < iterations; i += 1) {
							func();
						}
						log("ending deep scope test (no value)");
						log("starting deep scope test (with value)");
						for (i = 0; i < iterations; i += 1) {
							func(10);
						}
						log("ending deep scope test (with value)");
					}());
				}());
			}());
		}());
	}());
};

tests.argumentsNumbers = function () {
	function fiveArgs(a0, a1, a2, a3, a4) {
		return a4;
	}
	function fiftyArgs(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49) {
		return a49;
	}
	
	var fiveElems = [0, 1, 2, 3, 4],
		fiftyElems = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49];
	
	var iterations = 1000000,
		i;

	log("starting five args run test");
	for (i = 0; i < iterations; i++) {
		fiveArgs.apply(this, fiveElems);
	}
	log("ending five args run test");
	
	log("starting fifty args run test");
	for (i = 0; i < iterations; i++) {
		fiftyArgs.apply(this, fiftyElems);
	}
	log("ending fifty args run test");
	
	
	log("starting five args init test");
	for (i = 0; i < iterations; i++) {
		(function () {
			function fiveArgs(a0, a1, a2, a3, a4) {
				return a4;
			}
		}());
	}
	log("ending five args init test");
	
	log("starting fifty args init test");
	for (i = 0; i < iterations; i++) {
		(function () {
			function fiftyArgs(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49) {
				return a49;
			}
		}());
	}
	log("ending fifty args init test");

};
