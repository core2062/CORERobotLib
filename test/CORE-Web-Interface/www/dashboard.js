var dataSocket = new WebSocket("ws://" + document.location.host + "/data");

dataSocket.onmessage = function(data) {
    //console.log(data.data);
    processData(data.data);
    dataSocket.send("{}");
};

function processData(data) {
    var jsonObject = JSON.parse(data);
    for(key in jsonObject.driver) {
        console.log(jsonObject.driver[key]);
    }
}
// var intervalID = setInterval(ReadData, 100);