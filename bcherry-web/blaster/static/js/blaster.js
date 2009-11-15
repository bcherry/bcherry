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
		
		calc.keys.listen("2nd", function(){AGD.log("pressed 2nd");});
	};
	
	$(init);
})();