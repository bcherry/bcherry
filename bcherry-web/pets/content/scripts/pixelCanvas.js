;(function(){
	this.PixelCanvas = {};

	PixelCanvas.Canvas = function(domCanvas, xRes, yRes) {
		var pixelWidth = domCanvas.width / xRes;
		var pixelHeight = domCanvas.height / yRes;

		this.fillRect = function(x, y, w, h) {
			domCanvas.getContext('2d').fillRect(
				pixelWidth * x,
				pixelHeight * y,
				pixelWidth * w,
				pixelHeight * h);
		};
	};
})();
