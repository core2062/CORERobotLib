var fieldMousePos = {x:0,y:0};
var fieldMouseButton = {l:false,m:false,r:false};
var fieldMouseOld = {l:false,m:false,r:false};
var fieldMouseRising = {l:false,m:false,r:false};
var fieldMouseFalling = {l:false,m:false,r:false};
var flowMousePos = {x:0,y:0};
var flowMouseButton = {l:false,m:false,r:false};
var flowMouseOld = {l:false,m:false,r:false};
var flowMouseRising = {l:false,m:false,r:false};
var flowMouseFalling = {l:false,m:false,r:false};

var cursors = {
	default:"default",
	crosshair:"crosshair",
	move:"move"
}

var canvas = document.getElementById('windowCanvas');
canvas.oncontextmenu = function(evt){
	evt.preventDefault();
}
canvas.addEventListener('mousemove', function(evt) {
	var cnv = document.getElementById('windowCanvas');
	var rect = cnv.getBoundingClientRect();
	fieldMousePos = {
		x: Math.floor((evt.clientX-rect.left)/(rect.right-rect.left)*cnv.width),
		y: Math.floor((evt.clientY-rect.top)/(rect.bottom-rect.top)*cnv.height)
	};
}, false);
canvas.addEventListener('mousedown', function(evt){
	if(evt.button === 0){
		fieldMouseButton.l = true;
	} else if(evt.button === 1){
		fieldMouseButton.m = true;
	} else if(evt.button === 2){
		fieldMouseButton.r = true;
	}
}, false);
canvas.addEventListener('mouseup', function(evt){
	if(evt.button === 0){
		fieldMouseButton.l = false;
	} else if(evt.button === 1){
		fieldMouseButton.m = false;
	} else if(evt.button === 2){
		fieldMouseButton.r = false;
	}
}, false);

canvas = document.getElementById('toolBarCanvas');
canvas.oncontextmenu = function(evt){
	evt.preventDefault();
}
canvas.addEventListener('mousemove', function(evt) {
	var cnv = document.getElementById('toolBarCanvas');
	var rect = cnv.getBoundingClientRect();
	flowMousePos = {
		x: Math.floor((evt.clientX-rect.left)/(rect.right-rect.left)*cnv.width),
		y: Math.floor((evt.clientY-rect.top)/(rect.bottom-rect.top)*cnv.height)
	};
}, false);
canvas.addEventListener('mousedown', function(evt){
	if(evt.button === 0){
		flowMouseButton.l = true;
	} else if(evt.button === 1){
		flowMouseButton.m = true;
	} else if(evt.button === 2){
		flowMouseButton.r = true;
	}
}, false);
canvas.addEventListener('mouseup', function(evt){
	if(evt.button === 0){
		flowMouseButton.l = false;
	} else if(evt.button === 1){
		flowMouseButton.m = false;
	} else if(evt.button === 2){
		flowMouseButton.r = false;
	}
}, false);

function updateInput(){
	fieldMouseRising.l = fieldMouseButton.l && !fieldMouseOld.l;
	fieldMouseFalling.l = !fieldMouseButton.l && fieldMouseOld.l;
	fieldMouseRising.m = fieldMouseButton.m && !fieldMouseOld.m;
	fieldMouseFalling.m = !fieldMouseButton.m && fieldMouseOld.m;
	fieldMouseRising.r = fieldMouseButton.r && !fieldMouseOld.r;
	fieldMouseFalling.r = !fieldMouseButton.r && fieldMouseOld.r;
	fieldMouseOld.l = fieldMouseButton.l;
	fieldMouseOld.r = fieldMouseButton.r;
	fieldMouseOld.m = fieldMouseButton.m;

	flowMouseRising.l = flowMouseButton.l && !flowMouseOld.l;
	flowMouseFalling.l = !flowMouseButton.l && flowMouseOld.l;
	flowMouseRising.m = flowMouseButton.m && !flowMouseOld.m;
	flowMouseFalling.m = !flowMouseButton.m && flowMouseOld.m;
	flowMouseRising.r = flowMouseButton.r && !flowMouseOld.r;
	flowMouseFalling.r = !flowMouseButton.r && flowMouseOld.r;
	flowMouseOld.l = flowMouseButton.l;
	flowMouseOld.r = flowMouseButton.r;
	flowMouseOld.m = flowMouseButton.m;
}

