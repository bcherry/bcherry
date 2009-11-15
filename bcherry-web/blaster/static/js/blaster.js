;(function(){
	var console = AGD;
	
	var calcParams = {
		width	: 640,
		height	: 400,
		gfxMode	: "grayscale"
	};
	var calc = new TI.Calculator("ti89", calcParams);
	
	var x = 0;
	var y = 0;
	
	var init = function() {
		document.body.appendChild(calc.display.getDomElement());

		calc.keys.listen("up", function() {y-=2;});
		calc.keys.listen("down", function() {y+=2;});
		calc.keys.listen("left", function() {x-=2;});
		calc.keys.listen("right", function() {x+=2;});
		
		setInterval(draw, 16);
	};
	
	var draw = function() {
		calc.display.clear();
		var jetSprite = Sprites.jetReg;
		if (calc.keys.isPressed("down")) {
			jetSprite = Sprites.jetDown;
		} else if (calc.keys.isPressed("up")) {
			jetSprite = Sprites.jetUp;
		}
		calc.display.drawSprite(jetSprite.p1, jetSprite.p2, jetSprite.width, x, y);
	};
	
	$(init);
})();