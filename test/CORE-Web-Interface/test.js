var field = new Image();
var robot = new Image();
var width;
var height;

var fieldCanvas;
var fieldContext;
var flowCanvas;
var flowContext;

var rotTarget = -1;
var moveTarget = -1;

//properties
var fieldWidthIn = 324;
var fieldHeightIn = 652;
var robotWidthIn = 39.5;
var robotHeightIn = 35.5;
var robotCenterIn = 19;

var splines = [];
var samples = 5;

function Robot(x, y, rot){
	this.x = x;
	this.y = y;
	this.rot = rot;
}

var robots = [];
var oldRobots = [];

function getTarget(r = 20){
	var ratio = (window.innerWidth * .75) / fieldWidthIn;
	var mX = fieldMousePos.x / ratio;
	var mY = fieldMousePos.y / ratio;
	for(var i in robots){
		if(hypot(mX, mY, robots[i].y, robots[i].x) < r){
			return i;
		}
	}
	return -1;
}

function autonCreatorInit(){
	fieldCanvas = document.getElementById('fieldCanvas');
	fieldContext = fieldCanvas.getContext('2d');
	flowCanvas = document.getElementById('flowCanvas');
	flowContext = flowCanvas.getContext('2d');

	field.src = "images/field.png";
	robot.src = "images/robot.png";
	robots.push(new Robot(75, 120, 0));
	robots.push(new Robot(45, 220, 45*(Math.PI/180)));

	//window.requestAnimationFrame(loop);
}

function autonCreatorStart(){
	view = 'auton-creator'
	if(fieldCanvas == undefined){
		autonCreatorInit();
	}
	window.requestAnimationFrame(loop);
}

function loop(){
	updateInput();

	windowWidth = window.innerWidth;
	windowHeight = window.innerHeight;

	fieldContext.canvas.width = windowWidth * .75;
	fieldContext.canvas.height = windowHeight * .90;
	var fieldWidthPxl = windowWidth * .75;

	fieldContext.clearRect(0,0,fieldWidthPxl,windowHeight);

	var ratio = fieldWidthPxl / fieldWidthIn;
	var robotWidthPxl = robotWidthIn * ratio;
	var robotHeightPxl = robotHeightIn * ratio;
	var robotCenterPxl = robotCenterIn * ratio;
	var fieldHeightPxl = fieldHeightIn * ratio;

	fieldContext.drawImage(field, 0, 0, fieldWidthPxl, fieldHeightPxl);

	if(fieldMouseRising.l){
		moveTarget = getTarget();
	}

	if(fieldMouseFalling.l){
		moveTarget = -1;
	}

	if(fieldMouseRising.r){
		rotTarget = getTarget();
	}

	if(fieldMouseFalling.r){
		rotTarget = -1;
	}

	if(moveTarget != -1){
		robots[moveTarget].y = fieldMousePos.x / ratio;
		robots[moveTarget].x = fieldMousePos.y / ratio;
		fieldCanvas.style.cursor = cursors.move;
	}
	else if(rotTarget != -1){
		var angle = Math.atan2((fieldMousePos.y - robots[rotTarget].x * ratio) ,
			(fieldMousePos.x - robots[rotTarget].y * ratio));
		robots[rotTarget].rot = -angle + Math.PI * .5;
		fieldCanvas.style.cursor = cursors.crosshair;
	} else {
		fieldCanvas.style.cursor = cursors.default;
	}

	var waypoints = [];

	for(var i in robots){
		var robotPosXPxl = robots[i].x * ratio;
		var robotPosYPxl = robots[i].y * ratio;
		fieldContext.save();
		fieldContext.translate(Math.floor(robotPosYPxl), Math.floor(robotPosXPxl));
		fieldContext.rotate(-robots[i].rot);
		fieldContext.drawImage(robot, Math.floor(-robotWidthPxl * .5),
		Math.floor(-robotCenterPxl), Math.floor(robotWidthPxl), Math.floor(robotHeightPxl));
		fieldContext.restore();

		waypoints.push(new Waypoint(robots[i].x, robots[i].y, robots[i].rot));		
	}

	newSplines = [];
	for(var i = 0; i < waypoints.length-1; i++){
		newSplines.push(new Spline(waypoints[i], waypoints[i+1]));
	}

	if(newSplines.length != splines.length){
		splines = newSplines;
		samples = 5;
	} else {
		for(var i in newSplines){
			var n = newSplines[i];
			var o = splines[i];
			if((n.e != o.e || n.c != o.c) || n.d != o.d){
				splines = newSplines;
				samples = 5;
				break;
			}
		}
	}
	if(newSplines !== splines){
		if(samples < 50){
			samples*=1.1;
		}
	}

	var a = splines[0].coord(0);
	fieldContext.moveTo(a.y * ratio, a.x * ratio);
	fieldContext.beginPath();
	fieldContext.lineWidth = Math.floor(windowWidth * .005);
	fieldContext.strokeStyle = "#00ffff";
	var inc = 1 / samples;
	for(var s in splines){
		var c = splines[s].coord(0);
		fieldContext.moveTo(c.y * ratio, c.x * ratio);
		for(var i = 0; i <1; i += inc){
			c = splines[s].coord(i);
			fieldContext.lineTo(Math.floor(c.y * ratio), Math.floor(c.x * ratio));
		}
		c = splines[s].coord(1);
		fieldContext.lineTo(Math.floor(c.y * ratio), Math.floor(c.x * ratio));
	}

	fieldContext.stroke();
	var flowWidth = windowWidth*.25;
	flowContext.canvas.width = windowWidth * .25;
	flowContext.canvas.height = windowHeight * .9;
	flowContext.clearRect(0,0,flowWidth,windowHeight);
	flowContext.fillRect(0,0, flowWidth, windowHeight)

	oldRobots = robots;
	if(view == 'auton-creator'){
		fieldCanvas.style.visibility = 'visible';
		window.requestAnimationFrame(loop);
	} else {
		fieldCanvas.style.visibility = 'hidden';
	}
}
