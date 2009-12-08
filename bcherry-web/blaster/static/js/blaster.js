/*jslint white: true, onevar: true, browser: true, devel: true, undef: true, nomen: true, eqeqeq: true, plusplus: true, bitwise: true, regexp: true, strict: true, newcap: true, immed: true */
/*globals JB: false, TI: false, SimpleThread: false, jQuery: false, consul: false	 */
"use strict";
(function ($) {
	// Data Imports
	var	Data = JB.Data,
		Sprites = JB.Data.Sprites,
		Blocks = JB.Data.Blocks,
		
	// Functions in this module
		buildGame,
		buildMap,
		
	// Global config stuff
		calcParams = {
			width	: 160 * 4,
			height	: 100 * 4,
			gfxMode	: "grayscale"
		},
		
	// The game
		game;
		
	buildGame = function buildGame(spec) {
		var	that,
		
		// Functions in this module
			start,
			play,
			pause,
			main,
			keyListeners,
			scroll,
			draw,
			Shot,
			doShots,
			
		// Calc API
			calc = new TI.Calculator("ti89", calcParams), // FIXME: fix TI.Calculator to not use "new"
			
		// Game data
			jet = {x: 0, y: 0},
			map = buildMap({}), // FIXME: pass in spec
			shots = [], // FIXME: Make Array.dim and use here
			screen = {x: -100, y: 0},
			
		// Control vars
			thread,
			scrollInterval,
			started = false;
		
		that = {};
		
		// Control methods
		start = that.start = function start() {
			if (started) {
				return false;
			}
			started = true;
			
			// Load the display
			document.body.appendChild(calc.display.getDomElement()); // FIXME: should pass in dom location in spec
			
			
			// Set up key listeners
			keyListeners();
			
			// Set up the game thread, and start the game
			thread = new SimpleThread(main, {autoStart: false});
			that.play();
		};
		play = that.play = function play() {
			if (thread.isRunning()) {
				return;
			}
			thread.start();
			scrollInterval = setInterval(scroll, 20);
		};
		pause = that.pause = function pause() {
			if (!thread.isRunning()) {
				return;
			}
			thread.stop();
			clearInterval(scrollInterval);
		};
		
		// Game methods
		main = function main() {
			draw();
			doShots();
		};
		keyListeners = function keyListeners() {
			// TODO: Clean these up (seriously!)
			calc.keys.listen("up", function () {
				jet.y = jet.y - 2;
			});
			calc.keys.listen("down", function () {
				jet.y = jet.y + 2;
			});
			calc.keys.listen("left", function () {
				jet.x = jet.x - 2;
			});
			calc.keys.listen("right", function () {
				jet.x = jet.x + 2;
			});
			calc.keys.listen("2nd", function () {
				var shot = new Shot(jet.x, jet.y);
			});
		};
		scroll = function scroll() {
			screen.x = screen.x + 1;
		};
		draw = function draw() {
			var	i,
				shot,
				jetSprite;
			
			calc.display.clear();
			
			map.drawTo(calc.display, screen);
			
			jetSprite = Sprites.jetReg;
			if (calc.keys.isPressed("down")) {
				jetSprite = Sprites.jetDown;
			} else if (calc.keys.isPressed("up")) {
				jetSprite = Sprites.jetUp;
			}
			
			calc.display.drawSprite(jetSprite.p1, jetSprite.p2, jetSprite.width, jet.x, jet.y);
			
			for (i = 0; i < shots.length; i = i + 1) {
				shot = shots[i];
				calc.display.drawSprite(Sprites.cannon.p1, Sprites.cannon.p2, Sprites.cannon.width, shot.x, shot.y);
			}
		};
		Shot = function Shot(shipX, shipY) {
			this.x = shipX + 16;
			this.y = shipY;
			this.strength = 1; // TODO
			
			shots.push(this);
		};
		doShots = function doShots() {
			var i,
				j,
				shot,
				collisions;
			for (i = 0; i < shots.length; i = i + 1) {
				shot = shots[i];
				shot.x = shot.x + 4;
				
				
				// Once they've run off the map we can get rid of them
				if (map.outOfBounds(shot.x + screen.x, shot.y + screen.y, 8, 8, "right")) {
					shots.splice(i, 1);
					continue;
				}
				
				collisions = map.collision(shot.x + screen.x, shot.y + screen.y, 8, 8);
				if (collisions.length > 0) {
					for (j = 0; j < collisions.length; j = j + 1) {
						map.damage(collisions[j], shot.strength);
					}
					shots.splice(i, 1);
				}
			}
		};
		
		return that;
	};
	
	buildMap = function buildMap(spec) {
		var that,
			
		// Functions in this module
			drawTo,
			collision,
			damage,
			outOfBounds,
			
		// Local data
			map = [], // TODO: make Array.matrix and use here
			proportions = { // TODO: take these as parameter
				none			: 70,
				regular			: 85,
				indestructible	: 90,
				mine			: 100
			},
			
		// Loop iterators
			col,
			row,
			i;
		
		that = {};
		
		// Populate our map
		// TODO: make map size configurable
		for (col = 0; col < 160; col = col + 1) {
			map[col] = [];
			for (row = 0; row < 12; row = row + 1) {
				i = Math.floor(Math.random() * 100);
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
		
		drawTo = that.drawTo = function drawTo(display, offset) {
			var col,
				row,
				block,
				sprite;
			
			for (col = 0; col < map.length; col = col + 1) {
				if (col * 8 + 7 < offset.x || col * 8 > offset.x + display.width) {
					continue;
				}
				
				for (row = 0; row < map[col].length; row = row + 1) {
					block = map[col][row];
					if (block !== 0) {
						sprite = Blocks.Sprites[block];
						display.drawSprite(sprite.p1, sprite.p2, sprite.width, col * 8 - offset.x, row * 8 - offset.y);
					}
				}
			}
		};
		
		collision = that.collision = function collision(x, y, w, h) {
			var firstCol = Math.floor(x / 8),
				lastCol = Math.floor((x + w - 1) / 8),
				firstRow = Math.floor(y / 8),
				lastRow = Math.floor((y + h - 1) / 8),
				
				collisions = [],
				col,
				row,
				block;
			
			for (col = firstCol; col <= lastCol; col = col + 1) {
				if (col < 0 || col >= map.length) {
					continue;
				}
				for (row = firstRow; row <= lastRow; row = row + 1) {
					if (row < 0 || row >= map[col].length) {
						continue;
					}
					block = map[col][row];
					if (block !== 0) {
						collisions.push({
							col: col,
							row: row
						});
					}
				}
			}
			return collisions;
		};
		
		outOfBounds = that.outOfBounds = function outOfBounds(x, y, w, h, dirs) {
			var firstCol = Math.floor(x / 8),
				lastCol = Math.floor((x + w - 1) / 8),
				firstRow = Math.floor(y / 8),
				lastRow = Math.floor((y + h - 1) / 8),
				checkLeft,
				checkRight,
				checkUp,
				checkDown;
			
			dirs = dirs || "all";
			
			// TODO: let dirs be an array
			checkLeft = dirs === "all" || dirs === "x" || dirs === "left";
			checkRight = dirs === "all" || dirs === "x" || dirs === "right";
			checkUp = dirs === "all" || dirs === "y" || dirs === "up";
			checkDown = dirs === "all" || dirs === "y" || dirs === "down";
			
			return (checkLeft && lastCol < 0) || (checkRight && firstCol >= map.length) || (checkUp && lastRow < 0) || (checkDown && firstRow >= map[0].length);
		};
		
		damage = that.damage = function damage(block, strength) {
			var newblock = map[block.col][block.row] - 1;
			if (newblock < 0) {
				newblock = 0;
			}
			map[block.col][block.row] = newblock;
		};
		
		return that;
	};
	
	game = JB.game = buildGame({
		calcParams: calcParams
	});
	$(game.start);
}(jQuery));
