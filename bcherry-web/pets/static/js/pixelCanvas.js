;(function(){
	this.PixelCanvas = {};

	PixelCanvas.Canvas = function(domCanvas, xRes, yRes) {
		var pixelWidth = domCanvas.width / xRes;
		var pixelHeight = domCanvas.height / yRes;

		this.fillRect = function(x, y, w, h) {
			var context = domCanvas.getContext('2d');
			context.fillRect(
				pixelWidth * x,
				pixelHeight * y,
				pixelWidth * w,
				pixelHeight * h);
		};

		this.drawPixel = function(x, y, style) {
			var context = domCanvas.getContext('2d');
			context.fillStyle = style;
			context.fillRect(pixelWidth * x, pixelHeight * y, pixelWidth, pixelHeight);
		};

		this.drawSprite = function(sprite, x, y) {
			for (var i = 0; i < sprite.length; i++) {
				for (var j = 0; j < sprite[i].length; j++) {
					this.drawPixel(x + j, y + i, sprite[i][j]);
				}
			}
		};
	};
})();
