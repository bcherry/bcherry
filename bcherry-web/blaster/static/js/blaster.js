;(function(){
	var console = AGD;
	
	var calcParams = {
		width	: 160*4,
		height	: 100*4,
		gfxMode	: "grayscale"
	};
	var calc = new TI.Calculator("ti89", calcParams);
	
	var x = 0;
	var y = 0;
	
	var map = [];
	var screenX = 0;
	var screenY = 0;
	
	var init = function() {
		document.body.appendChild(calc.display.getDomElement());

		calc.keys.listen("up", function() {y-=2;});
		calc.keys.listen("down", function() {y+=2;});
		calc.keys.listen("left", function() {x-=2;});
		calc.keys.listen("right", function() {x+=2;});
		
		for (var col = 0; col < 20; col++) {
			var c = map[col] = [];
			for (var row = 0; row < 12; row++) {
				if (row % 2) {
					c[row] = 1;
				} else {
					c[row] = 2;
				}
			}
		}
		
		var thread = new SimpleThread(main);
		
	};
	
	var main = function() {
		scroll();
		draw();
	};
	
	var scroll = function() {
		screenX++;
	};
	
	var draw = function() {
		calc.display.clear();
		
		drawMap();
		
		var jetSprite = Sprites.jetReg;
		if (calc.keys.isPressed("down")) {
			jetSprite = Sprites.jetDown;
		} else if (calc.keys.isPressed("up")) {
			jetSprite = Sprites.jetUp;
		}

		calc.display.drawSprite(jetSprite.p1, jetSprite.p2, jetSprite.width, x, y);
	};
	
	var drawMap = function() {
		for (var col = 0; col < map.length; col++) {
			if (col * 8 + 7 < screenX || col * 8 > screenX + calc.display.width) {
				continue;
			}

			for (var row = 0; row < map[col].length; row++) {
				if (map[col][row] == 1) {
					calc.display.drawSprite(Sprites.regular.p1, Sprites.regular.p2, Sprites.regular.width, col * 8 - screenX, row * 8 - screenY);
				}
			}
		}
	};
	
	$(init);
})();