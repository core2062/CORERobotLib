

function Waypoint(x, y, theta){
	this.x = x;
	this.y = y;
	this.theta = theta;
}

function Coord(x, y){
	this.x = x;
	this.y = y;
}

function Spline(a, b){
	this.xOff = a.x;
	this.yOff = a.y;
	var delta = Math.sqrt( (b.x - a.x)*(b.x - a.x) + (b.y - a.y)*(b.y - a.y) );
	this.knot = delta;
	this.angleOff = Math.atan2(b.y - a.y, b.x - a.x);

	var a0 = a.theta - this.angleOff;
	while(a0 > 2 * Math.PI){
		a0 -= Math.PI * 2;
	}
	a0 = Math.tan(a0);
	var a1 = b.theta - this.angleOff;
	while(a1 > 2 * Math.PI){
		a1 -= Math.PI * 2;
	}
	a1 = Math.tan(a1);
	this.a = 0;
	this.b = 0;
	this.c = (a0 + a1) / (this.knot * this.knot);
	this.d = -(2 * a0 + a1) / this.knot;
	this.e = a0;
	this.coord = function(percentage){
		percentage = Math.max(Math.min(percentage, 1), 0);
		var x = percentage * this.knot;
		var y = (this.a * x + this.b) * (Math.pow(x,4)) + (this.c*x + this.d) * (x*x) + this.e*x;
		var cosTheta = Math.cos(this.angleOff);
		var sinTheta = Math.sin(this.angleOff);

		return new Coord(
			x * cosTheta - y * sinTheta + this.xOff,
			x * sinTheta + y * cosTheta + this.yOff)
	}
}