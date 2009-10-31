;(function(){
	var canvas = {dom: null, ctx: null, h: null, w: null};
	var gameRes = {w: 2, h: 2};
	var mapPixel = function(x, y) {
		return {
			x: (canvas.w / gameRes.w) * x,
			y: (canvas.h / gameRes.h) * y
		};
	};
$(function(){
	var canvas = {dom:$("canvas").get(0)};
	canvas.ctx = canvas.dom.getContext('2d');
	canvas.w = canvas.dom.width;
	canvas.h = canvas.dom.height;

	console.log(canvas.dom.height);
});
})();

