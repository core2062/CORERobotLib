var roborio = new Image();

var nodes = [];
var connections = [];
var startNode = -1;

function node(x, y, type){
    this.x = x;
    this.y = y;
    this.type = type;
}

function connection(startNode, endNode, type){
    this.startNode = startNode;
    this.endNode = endNode;
    this.type = type;
}

function getTargetNode() {
    var r = 20;
    var ratio = (window.innerWidth - 100) / fieldWidthIn;
    var mX = fieldMousePos.x / ratio;
    var mY = fieldMousePos.y / ratio;
    var closestNode = -1;
    var currentLeastDistance = r;
    for(var i in nodes){
        var distance = hypot(mX, mY, nodes[i].y, nodes[i].x);
        if(distance < currentLeastDistance) {
            closestNode = i;
            currentLeastDistance = distance;
        }
    }
    return closestNode;
}

function diagramCreatorInit() {
	roborio.src = "images/roborio.jpg";
	nodes.push(new node(1080, 462, "PWM"));
}

function diagramCreatorLoop() {
    var fieldWidthPxl = windowWidth - 100;
    fieldContext.canvas.width = fieldWidthPxl;
    fieldContext.canvas.height = windowHeight - 32;

    flowContext.canvas.width = windowWidth - fieldWidthPxl;
    flowContext.canvas.height = windowHeight - 32;

    flowContext.canvas.style.top = "0px";
    flowContext.canvas.style.right = "0px";
    flowContext.canvas.style.position = "absolute";

    var smallerDimension = windowHeight > windowWidth ? windowWidth : windowHeight;

	var roborioSize = smallerDimension * 0.5;

	roborioSize = roborioSize > 400 ? 400 : roborioSize;

	var roborioX = (windowWidth - 100) * 0.5 - roborioSize * 0.5;
    var roborioY = windowHeight * 0.5 - roborioSize * 0.5;

	fieldContext.drawImage(roborio, roborioX, roborioY, roborioSize, roborioSize);
    fieldContext.fillText("X: " + fieldMousePos.x + " Y: " + fieldMousePos.y, 100, 100);
    if(fieldMouseRising.l) {
        var targetNode = getTargetNode();
        if(startNode != -1) {
            if(targetNode != -1) {
                if(targetNode.type == startNode.type) {
                    connections.push(new connection(nodes[startNode], nodes[targetNode], nodes[targetNode].type));
                }
            }
        } else {
            startNode = targetNode;
        }
    }

    if(fieldMouseRising.r) {
        //nodes[getTargetNode()].pop();
    }

    for(var i in nodes) {
        fieldContext.fillStyle = "red";
        fieldContext.beginPath();
        fieldContext.arc(nodes[i].x, nodes[i].y, 5, 0, 2 * Math.PI);
        fieldContext.fill();
        console.log("HERE");
    }
}
