;(function($){
// Data Imports
var	Data = JB.Data,
	Sprites = JB.Data.Sprites,
	Blocks = JB.Data.Blocks,
	calcParams = {
		width	: 160*4,
		height	: 100*4,
		gfxMode	: "grayscale"
	};
	
JB.load = function () {
	
	var calc = new TI.Calculator("ti89", calcParams),
		x = 0, // FIXME
		y = 0, // FIXME
		map = [],
		shots = [],
		screenX = -100,  // start before the map
		screenY = 0,
		thread,
		scrollInterval;

	
	function init() {
		document.body.appendChild(calc.display.getDomElement());

		calc.keys.listen("up", function() {y-=2;});
		calc.keys.listen("down", function() {y+=2;});
		calc.keys.listen("left", function() {x-=2;});
		calc.keys.listen("right", function() {x+=2;});
		calc.keys.listen("2nd", function() {var shot = new Shot(x, y);});
		buildMap();
		
		thread = new SimpleThread(main, {autoStart: false});
		
		play();
	}
	
	function main() {
		draw();
		doShots();
	}
	
	function scroll() {
		screenX++;
	}
	
	function play() {
		if (thread.isRunning()) {
			return;
		}
		thread.start();
		scrollInterval = setInterval(scroll, 20);
	}
	
	function pause() {
		if (!thread.isRunning()) {
			return;
		}
		thread.stop();
		clearInterval(scrollInterval);
	}
	
	function draw() {
		calc.display.clear();
		
		drawMap();
		
		var jetSprite = Sprites.jetReg;
		if (calc.keys.isPressed("down")) {
			jetSprite = Sprites.jetDown;
		} else if (calc.keys.isPressed("up")) {
			jetSprite = Sprites.jetUp;
		}

		calc.display.drawSprite(jetSprite.p1, jetSprite.p2, jetSprite.width, x, y);
		
		for (var i = 0; i < shots.length; i++) {
			var shot = shots[i];
			calc.display.drawSprite(Sprites.cannon.p1, Sprites.cannon.p2, Sprites.cannon.width, shot.x, shot.y);
		}
	}
	
	function Shot(shipX, shipY) {
		this.x = shipX + 16;
		this.y = shipY;
		this.strength = 1; // TODO
		
		shots.unshift(this);
	}
	
	function doShots() {
		for (var i = 0; i < shots.length; i++) {
			var shot = shots[i];
			shot.x+=4;
			
			
			// Once they've run off the map we can get rid of them
			if (shot.x > map.length * 8) {
				shots.splice(i, 1);
				continue;
			}
			
			var collisions = mapCollide(shot.x, shot.y, 8, 8);
			if (collisions.length > 0) {
				for (var j = 0; j < collisions.length; j++) {
					damageBlock(collisions[j], shot.strength);
				}
				shots.splice(i, 1);
			}
		}
	}
	
	function damageBlock(block, strength) {
		var newblock = block.block - 1;
		if (newblock < 0) {
			newblock = 0;
		}
		map[block.col][block.row] = newblock;
	}
	
	function drawMap() {
		for (var col = 0; col < map.length; col++) {
			if (col * 8 + 7 < screenX || col * 8 > screenX + calc.display.width) {
				continue;
			}

			for (var row = 0; row < map[col].length; row++) {
				var block = map[col][row];
				if (block !== 0) {
					var sprite = Blocks.Sprites[block];
					calc.display.drawSprite(sprite.p1, sprite.p2, sprite.width, col * 8 - screenX, row * 8 - screenY);
				}
			}
		}
	}
	
	function mapCollide(x, y, w, h) {
		var firstCol = Math.floor((screenX + x) / 8);
		var lastCol = Math.floor((screenX + x + w - 1) / 8);
		var firstRow = Math.floor(y / 8);
		var lastRow = Math.floor((y + h - 1) / 8);
		
		var collisions = [];
		for (var col = firstCol; col <= lastCol; col++) {
			if (col < 0 || col >= map.length) {
				continue;
			}
			for (var row = firstRow; row <= lastRow; row++) {
				if (row < 0 || row >= map[col].length) {
					continue;
				}
				var block = map[col][row];
				if (block !== 0) {
					collisions.push({col:col, row:row, block:block});
				}
			}
		}
		return collisions;
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
						map[col][row] = Blocks.mine1;
					}
				} else {
					map[col][row] = Blocks.none;
				}
			}
		}
	}
	
	init();
};
$(JB.load);
})(jQuery);
