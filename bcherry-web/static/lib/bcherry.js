Function.prototype.method = function (name, fn) {
	this.prototype[name] = fn;
	return this;
};

var isNumber = function isNumber(value) {
	return typeof value === "number" && isFinite(value);
}

var isArray = function isArray(value, strict) {
	if (strict) {
		return value && typeof value === "object" && typeof value.length === "number" && !(value.propertyIsEnumerable("length"));
	}
	return value && typeof value === "object" && value.constructor === Array;
}