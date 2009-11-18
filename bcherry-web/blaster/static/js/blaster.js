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
	var screenX = -100;  // start before the map
	var screenY = 0;
	
	var init = function() {
		document.body.appendChild(calc.display.getDomElement());

		calc.keys.listen("up", function() {y-=2;});
		calc.keys.listen("down", function() {y+=2;});
		calc.keys.listen("left", function() {x-=2;});
		calc.keys.listen("right", function() {x+=2;});
		
		buildMap();
		
		var thread = new SimpleThread(main);
		
		setInterval(scroll, 20);
		
	};
	
	var main = function() {
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
	
	function drawMap() {
		for (var col = 0; col < map.length; col++) {
			if (col * 8 + 7 < screenX || col * 8 > screenX + calc.display.width) {
				continue;
			}

			for (var row = 0; row < map[col].length; row++) {
				if (map[col][row] !== 0) {
					var sprite;
					if (map[col][row] == Blocks.regular) {
						sprite = Sprites.regular;
					} else if (map[col][row] == Blocks.indestructible) {
						sprite = Sprites.indestructible;
					} else if (map[col][row] == Blocks.mine) {
						sprite = Sprites.mine1;
					}
				
					calc.display.drawSprite(sprite.p1, sprite.p2, sprite.width, col * 8 - screenX, row * 8 - screenY);
				}
			}
		}
	}
	
	function buildMap() {
		var proportions = {
			none			: 70,
			regular			: 85,
			indestructible	: 90,
			mine			: 100
		};
		for (var col = 0; col < 160; col++) {
			map[col] = [];
			for (var row = 0; row < 12; row++) {
				var i = Math.floor(Math.random() * 100);
				if (i > proportions.none) {
					if (i < proportions.regular) {
						map[col][row] = Blocks.regular;
					} else if (i < proportions.indestructible) {
						map[col][row] = Blocks.indestructible;
					} else {
						map[col][row] = Blocks.mine;
					}
				} else {
					map[col][row] = 0;
				}
			}
		}
	}
	
	$(init);
})();