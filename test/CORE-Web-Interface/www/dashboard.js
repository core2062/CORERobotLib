var dataSocket = new WebSocket("ws://" + document.location.host + "/data");

dataSocket.onmessage = function(data) {
    console.log(data.data);
};

function ReadData() {
    dataSocket.send("debug");
}
var intervalID = setInterval(ReadData, 100);