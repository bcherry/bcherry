;(function(){
	var console = AGD;
	
	var calcParams = {
		width	: 640,
		height	: 400,
		gfxMode	: "grayscale"
	};
	var calc = new TI.Calculator("ti89", calcParams);
	
	var init = function() {
		document.body.appendChild(calc.display.getDomElement());
		
		calc.display.drawSprite(Sprites.jetReg.p1, Sprites.jetReg.p2, Sprites.jetReg.width, 0, 0);
		
		calc.display.pixelOn(10, 20, 0);
		calc.display.pixelOn(11, 20, 1);
		calc.display.pixelOn(12, 20, 2);
		calc.display.pixelOn(13, 20, 3);
	};
	
	$(init);
})();