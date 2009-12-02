// This file provides a mock JavaScript API for a TI graphing calculator
// It doesn't work in IE, because you need canvas and reasonable key detection
// Right now, it only supports the TI-89 family, but adding support for others would be trivial
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
			height	: calcs[model].display.height,
			gfxMode	: "monochrome"
		};
		for (var key in defaultParams) {
			if (params[key] === undefined) {
				params[key] = defaultParams[key];
			}
		}
		
		assert(params.gfxMode in {"monochrome":1, "grayscale":1}, "Unsupported graphics mode.");
		if (params.gfxMode == "monochrome") {
			this.display = new BasicDisplay(calcs[model].display, params.width, params.height);
		} else if (params.gfxMode == "grayscale") {
			this.display = new GrayDisplay(calcs[model].display, params.width, params.height);
		}
		
		this.getDomElement = this.display.getDomElement;
		
		this.keys = new Keys();
	};
	
	var BasicDisplay = function(params, width, height, noWarn) {
		noWarn || issueWarnings("display", params, width, height);
		
		var canvas = document.createElement("canvas");
		canvas.width = width;
		canvas.height = height;
		var pixelWidth = width / params.width;
		var pixelHeight = height / params.height;
		
		canvas.setAttribute("style", "background-color:" + params.bgColor + ";border:solid 1px black;");
		
		this.getDomElement = function() { return canvas; };
		
		this.pixelOn = function(x, y, ctx) {
			ctx = ctx || canvas.getContext("2d");
			ctx.fillStyle = params.fgColor;
			ctx.fillRect(getX(x), getY(y), pixelWidth, pixelHeight);
		};
		
		this.pixelOff = function(x, y, ctx) {
			ctx = ctx || canvas.getContext("2d");
			ctx.clearRect(getX(x), getY(y), pixelWidth, pixelHeight);
		};
		
		this.clear = function() {
			var ctx = canvas.getContext("2d");
			ctx.clearRect(0, 0, width, height);
		};
		
		this.drawSprite = function(sprite, width, x, y) {
			var ctx = canvas.getContext("2d");
			
			var clipX = 0;
			var clipY = 0;
			var clippedX = x;
			var clippedY = y;
			var clippedWidth = width;
			var clippedHeight = sprite.length;
			
			if (x < 0) {
				clipX = 0 - x;
				clippedX = 0;
				clippedWidth = width - clipX;
			} else if (x + width > this.width) {
				clippedWidth = this.width - x;
			}
			if (y < 0) {
				clipY = 0 - y;
				clippedY = 0;
				clippedHeight = sprite.length - clipY;
			} else if (y + sprite.length > this.height) {
				clippedHeight = this.height - y;
			}
			
			if (clippedWidth <= 0 || clippedHeight <= 0) {
				return;
			}

			var imageData;
			if (ctx.createImageData) {
				imageData = ctx.createImageData(clippedWidth, clippedHeight);
			} else if (ctx.getImageData) {
				imageData = ctx.getImageData(clippedX, clippedY, clippedWidth, clippedHeight);
			} else {
				imageData = {'width' : clippedWidth, 'height' : clippedHeight, 'data' : new Array(clippedWidth * clippedHeight *4)};
			}
			var pixels = imageData.data;
			for (var row = 0; row < clippedHeight; row++) {
				for (var col = 0; col < clippedWidth; col++) {
					if ((sprite[row + clipY] >> (width - (col + clipX) - 1)) & 1) {
						var p = row * clippedWidth * 4 + col * 4;
						pixels[p] = 49;
						pixels[p + 1] = 63;
						pixels[p + 2] = 66;
						pixels[p + 3] = 255;
					}
				}
			}
			
			ctx.putImageData(imageData, clippedX, clippedY);
		};
		
		var getX = function(x) {
			return x * pixelWidth;
		};
		
		var getY = function(y) {
			return y * pixelHeight;
		};
		
		this.width = width;
		this.height = height;
	};
	
	var GrayDisplay = function(params, width, height, noWarn) {
		noWarn || issueWarnings("display", params, width, height);
		
		var div = document.createElement("div");
		div.setAttribute("style", "width:" + width + ";height:" + height + ";background-color:" + params.bgColor + ";border:solid 1px black;position:relative;");
		
		var canvas = document.createElement("canvas");
		canvas.width = params.width;
		canvas.height = params.height;
		
		canvas.setAttribute("style", "width:100%;height:100%;");
		
		div.appendChild(canvas);
		
		this.getDomElement = function() { return div; };
		
		this.pixelOn = function(x, y, ctx) {
			ctx = ctx || canvas.getContext("2d");
			ctx.fillStyle = params.fgColor;
			ctx.fillRect(getX(x), getY(y), pixelWidth, pixelHeight);
		};
		
		this.pixelOff = function(x, y, ctx) {
			ctx = ctx || canvas.getContext("2d");
			ctx.clearRect(getX(x), getY(y), pixelWidth, pixelHeight);
		};
		
		this.clear = function() {
			var ctx = canvas.getContext("2d");
			ctx.clearRect(0, 0, width, height);
		};
		
		this.drawSprite = function(layer1, layer2, width, x, y, mode) {
			var ctx = canvas.getContext("2d");
			
			var clipX = 0;
			var clipY = 0;
			var clippedX = x;
			var clippedY = y;
			var clippedWidth = width;
			var clippedHeight = layer1.length;
			
			if (x < 0) {
				clipX = 0 - x;
				clippedX = 0;
				clippedWidth = width - clipX;
			} else if (x + width > this.width) {
				clippedWidth = this.width - x;
			}
			if (y < 0) {
				clipY = 0 - y;
				clippedY = 0;
				clippedHeight = layer1.length - clipY;
			} else if (y + layer1.length > this.height) {
				clippedHeight = this.height - y;
			}
			
			if (clippedWidth <= 0 || clippedHeight <= 0) {
				return;
			}

			var imageData;
			if (ctx.getImageData) {
				imageData = ctx.getImageData(clippedX, clippedY, clippedWidth, clippedHeight);
			} else {
				imageData = {'width' : clippedWidth, 'height' : clippedHeight, 'data' : new Array(clippedWidth * clippedHeight *4)};
			}
			var pixels = imageData.data;
			for (var row = 0; row < clippedHeight; row++) {
				for (var col = 0; col < clippedWidth; col++) {
					var l1 = (layer1[row + clipY] >> (width - (col + clipX) - 1)) & 1;
					var l2 = (layer2[row + clipY] >> (width - (col + clipX) - 1)) & 1;
					
					var alpha = l1 * 85 + l2 * 85 * 2;
					
					if (alpha) {
						var p = row * clippedWidth * 4 + col * 4;
						pixels[p] = 49;
						pixels[p + 1] = 63;
						pixels[p + 2] = 66;
						pixels[p + 3] = alpha;
					}
				}
			}
			
			ctx.putImageData(imageData, clippedX, clippedY);
		};
		
		this.width = params.width;
		this.height = params.height;
	};
	
	var Keys = function() {
		var _this = this;
		
		var keyMap = {
			16	: "shift", 		// Shift
			32	: "2nd",		// Space
			17	: "diamond",	// Control
			13	: "enter",		// Enter/Return
			38	: "up",			// Up
			40	: "down",		// Down
			37	: "left",		// Left
			39	: "right",		// Right
			27	: "esc",		// Escape
			112	: "f1",			// F1
			113	: "f2",			// F2
			114 : "f3",			// F3
			115 : "f4",			// F4
			116 : "f5",			// F5  
		};
		var events = {
			"2nd"		: function(){return;},
			"diamond"	: function(){return;},
			"shift"		: function(){return;},
			"up"		: function(){return;},
			"down"		: function(){return;},
			"left"		: function(){return;},
			"right"		: function(){return;}
		};
		this.listen = function(name, func, repeat) {
			assert(name in events, "Unsupported key.");
			
			var oldfunc = events[name];
			events[name] = function() {
				if (!repeat) {
					
				}
				if (func() === false) {
					return false;
				}
				return oldfunc();
			};
		};
		this.press = function(name) {
			assert(name in events, "Unsupported key.");
				
			events[name]();
		};
		
		this.isPressed = function(name) {
			assert(name in events, "Unsupported key.");
			
			return name in pressedKeys;
		};
		
		var translateKeyCode = function(code) {
			if (!(code in keyMap)) {
				return false;
			}
			
			return keyMap[code];
		};
		
		var pressedKeys = {};
		var watchKeys = function() {
			for (var key in pressedKeys) {
				events[key]();
			}
		};
		
		var oldfunc = document.onkeydown;
		document.onkeydown = function(e) {
			var keyCode = (window.event) ? event.keyCode : e.keyCode;
			var key = translateKeyCode(keyCode);
			if (key) {
				pressedKeys[key] = true;
			}
			
			if (oldfunc) {
				return oldfunc();
			}
		};
		
		var oldup = document.onkeyup;
		document.onkeyup = function(e) {
			var keyCode = (window.event) ? event.keyCode : e.keyCode;
			var key = translateKeyCode(keyCode);
			if (key && pressedKeys[key] !== undefined) {
				delete pressedKeys[key];
			}
			
			if (oldup) {
				return oldup();
			}
		};
		
		setInterval(watchKeys, 100);
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