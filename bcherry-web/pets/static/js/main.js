;(function(){
	var X_RES = 100;
	var Y_RES = 60;
	var pc = null;

	// demo functions
	var checkerboard = function() {
		for (var y = 0; y < Y_RES; y++) {
			for (var x = y % 2; x < X_RES; x += 2) {
				pc.drawPixel(x, y, "#8cc4ff");
			}
			for (var x = (y + 1) % 2; x < X_RES; x += 2) {
				pc.drawPixel(x, y, "#003b7a");
			}
		}
	};

	var spriteTest = function() {
		var sprite = [
			["#8cc4ff", "#003b7a", "#003b7a", "#8cc4ff"],
			["#003b7a", "#8cc4ff", "#8cc4ff", "#003b7a"],
			["#003b7a", "#8cc4ff", "#8cc4ff", "#003b7a"],
			["#8cc4ff", "#003b7a", "#003b7a", "#8cc4ff"]
		];

		pc.drawSprite(sprite, 2, 2);
	};

$(function(){
	var canvas = $("canvas").get(0);
	pc = new PixelCanvas.Canvas(canvas, X_RES, Y_RES);
	
	//spriteTest();
	checkerboard();
});
})();

