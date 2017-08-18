var view = "driver-dashboard";
var updateID;
var width;
var height;

tabButtons = {};

function initialize() {
    fieldCanvas = document.getElementById('windowCanvas');
    fieldContext = fieldCanvas.getContext('2d');
    flowCanvas = document.getElementById('toolBarCanvas');
    flowContext = flowCanvas.getContext('2d');
    windowWidth = window.innerWidth;
    windowHeight = window.innerHeight;
    diagramCreatorInit();
    autonCreatorInit();
	tabButtons.driverDashboard = document.getElementById("driver-dashboard-button");
	tabButtons.debugDashboard = document.getElementById("debug-dashboard-button");
	tabButtons.autonConfig = document.getElementById("auton-config-button");
	tabButtons.autonCreator = document.getElementById("auton-creator-button");
    tabButtons.diagramCreator = document.getElementById("diagram-button");
    loop();
}

function loop(){
    updateInput();
	var onColor = '#ffffff';
	var offColor = '#cccccc';
	tabButtons.driverDashboard.style.color = (view == "driver-dashboard")?onColor:offColor;
	tabButtons.debugDashboard.style.color = (view == "debug-dashboard")?onColor:offColor;
	tabButtons.autonConfig.style.color = (view == "auton-config")?onColor:offColor;
	tabButtons.autonCreator.style.color = (view == "auton-creator")?onColor:offColor;
    tabButtons.autonCreator.style.color = (view == "auton-creator")?onColor:offColor;
    tabButtons.diagramCreator.style.color = (view == "diagram-creator")?onColor:offColor;

    windowWidth = window.innerWidth;
    windowHeight = window.innerHeight;

    fieldContext.clearRect(0, 0, windowWidth, windowHeight);
    fieldContext.rect(0, 0, windowWidth, windowHeight);

    if(view == "auton-creator") {
        autonCreatorLoop();
    } else if (view == "diagram-creator") {
        diagramCreatorLoop();
    }
    requestAnimationFrame(loop);
}
