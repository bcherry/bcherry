// This file provides a mock JavaScript API for a TI graphing calculator
// You'll need a browser with <canvas> support, of course
// Right now, it assumes a TI-89 variety, but adding support for others would be trivial
;(function(){
	var TI = window.TI = {};
	
	var calcs = {
		ti89	: {
			display	: {
				height	: 100,
				width	: 160,
				bgColor	: "#D7E5D2",
				fgColor	: "#313F42"
			}
		}
	};
	
	TI.Calculator = function(model) {
		assert(model in calcs, "Unsupported Calculator");
		
		this.display = new Display(calcs[model].display);
		
		this.getDomElement = this.display.getDomElement;
	};
	
	var Display = function(params) {
		var canvas = document.createElement("canvas");
		canvas.height = params.height;
		canvas.width = params.width;
		canvas.setAttribute("style", "background-color:" + params.bgColor + ";border:solid 1px black;");
		
		this.getDomElement = function() { return canvas; };
	};
	
	// extra internal bits
	var assert = function(test, msg) {
		if (!test) {
			throw "TIjs Error: " + msg;
		}
	};
})();