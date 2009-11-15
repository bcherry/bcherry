// This file provides a mock JavaScript API for a TI graphing calculator
// You'll need a browser with <canvas> support, of course
// Right now, it assumes a TI-89 variety, but adding support for others would be trivial
;(function(){
	var TI = window.TI = {};
	
	var calcs = {
		ti89	: {
			display	: {
				width	: 160,
				height	: 100,
				bgColor	: "#D7E5D2",
				fgColor	: "#313F42"
			}
		}
	};
	
	TI.Calculator = function(model, params) {
		assert(model in calcs, "Unsupported Calculator");
		
		params = params || {};
		defaultParams = {
			width	: calcs[model].display.width,
			height	: calcs[model].display.height
		};
		for (var key in defaultParams) {
			if (params[key] === undefined) {
				params[key] = defaultParams[key];
			}
		}
		
		this.display = new BasicDisplay(calcs[model].display, params.width, params.height);
		
		this.getDomElement = this.display.getDomElement;
	};
	
	var BasicDisplay = function(params, width, height) {
		var canvas = document.createElement("canvas");
		
		issueWarnings("display", params, width, height);
		canvas.width = width;
		canvas.height = height;
		var pixelWidth = width / params.width;
		var pixelHeight = height / params.height;
		
		canvas.setAttribute("style", "background-color:" + params.bgColor + ";border:solid 1px black;");
		
		this.getDomElement = function() { return canvas; };
		
		this.pixelOn = function(x, y) {
			var ctx = canvas.getContext("2d");
			ctx.fillStyle = params.fgColor;
			ctx.fillRect(getX(x), getY(y), pixelWidth, pixelHeight);
		};
		
		this.pixelOff = function(x, y) {
			var ctx = canvas.getContext("2d");
			ctx.fillStyle = params.bgColor;
			ctx.fillRect(getX(x), getY(y), pixelWidth, pixelHeight);
		};
		
		this.drawSprite = function(sprite, width, x, y, mode) {
			mode = mode || "xor";
			
			for (var row = 0; row < sprite.length; row++) {
				console.log("row %o", row);
				for (var col = 0; col < width; col++) {
					console.log("col %o", col);
					if ((sprite[row] >> (width - col - 1)) & 1) {
						this.pixelOn(x + col, y + row);
					}
				}
			}
		};
		
		var getX = function(x) {
			return x * pixelWidth;
		};
		
		var getY = function(y) {
			return y * pixelHeight;
		};
	};
	
	// extra internal bits
	var assert = function(test, msg) {
		if (!test) {
			throw "TIjs Error: " + msg;
		}
	};
	
	var warn = function(msg) {
		if (window.console) {
			console.warn("TIjs Warning: " + msg);
		}
	};
	
	var issueWarnings = function(component, params, width, height) {
		switch (component.toLowerCase()) {
			case "display":
				if (width < params.width) {
					warn("Display width is too small, some pixels will be lost.");
				} else if (width % params.width !== 0) {
					warn("Display width is not even multiple of native width and will look bad.");
				}
				if (height < params.height) {
					warn("Display height is too small, some pixels will be lost");
				} else if (height % params.height !== 0) {
					warn("Display height is not even multiple of native height and will look bad.");
				}
				if (width / height != params.width / params.height) {
					warn("Display aspect ratio is off, dome distortion will occur.");
				}
				break;
		}
	};
})();