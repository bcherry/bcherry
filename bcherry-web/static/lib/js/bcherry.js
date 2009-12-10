// The stuff in this file is culled from lots of sources, and from experience
// In particular, a lot of it comes from JavaScript: The Good Parts, by Douglas Crockford

/*jslint white: true, onevar: true, devel: true, undef: true, nomen: true, eqeqeq: true, plusplus: true, bitwise: true, regexp: true, strict: true, newcap: true, immed: true */
"use strict";

Function.prototype.method = function (name, func) {
	if (!this.prototype[name]) {
		this.prototype[name] = func;
		return this;
	}
};

Array.dim = function (dimension, initial) {
	var a = [], i;
	for (i = 0; i < dimension; i += 1) {
		a[i] = initial;
	}
	return a;
};

Array.matrix = function (m, n, initial) {
	var a, i, j, mat = [];
	for (i = 0; i < m; i += 1) {
		a = [];
		for (j = 0; j < n; j += 1) {
			a[j] = initial;
		}
		mat[i] = a;
	}
	return mat;
};

Number.method("integer", function () {
	return Math[this < 0 ? "ceil" : "floor"](this);
});

String.method("trim", function () {
	return this.replace(/^\s+|\s+$/g, "");
});

Math.randInt = function (range) {
	return (Math.random() * range).integer();
};