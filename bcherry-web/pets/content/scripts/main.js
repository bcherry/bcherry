;(function(){
	var canvas = {dom: null, ctx: null, h: null, w: null};
	var gameRes = {w: 2, h: 2};
	var mapPixel = function(x, y) {
		return {
			x: (canvas.w / gameRes.w) * x,
			y: (canvas.h / gameRes.h) * y
		};
	};
	var getWidth = function() { return canvas.w / gameRes.w; };
	var getHeight = function() { return canvas.h / gameRes.h; };
	var fillPixel = function(x, y) {
		canvas.ctx.fillRect(mapPixel(x), mapPixel(y), getWidth(), getHeight());
	};
$(function(){
	var canvas = {dom:$("canvas").get(0)};
	canvas.ctx = canvas.dom.getContext('2d');
	canvas.w = canvas.dom.width;
	canvas.h = canvas.dom.height;

	fillPixel(0,1);
	
});
})();

