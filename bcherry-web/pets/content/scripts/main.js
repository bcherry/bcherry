;(function(){
$(function(){
	var canvas = $("canvas").get(0);
	var pc = new PixelCanvas.Canvas(canvas, 10, 6);
	for (var y = 0; y < 6; y++) {
		for (var x = y % 2; x < 10; x+=2) {
			pc.fillRect(x, y, 1, 1);
		}
	}
});
})();

