;(function(){
	var console = AGD;
	
	var calcParams = {
		width: 640,
		height: 400
	};
	var calc = new TI.Calculator("ti89", calcParams);
	
	var init = function() {
		document.body.appendChild(calc.display.getDomElement());
		
		calc.display.drawSprite(Sprites.shieldIcon.p1, Sprites.shieldIcon.width, 0, 0);
	};
	
	$(init);
})();