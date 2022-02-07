var XMLHttpRequest = require('xhr2');
let request = new XMLHttpRequest();
request.open("POST", "http://localhost:8080");
request.setRequestHeader("Accept", "application/json");
request.setRequestHeader("Content-Type", "application/json");
var data = `{
  "Id": 78912,
  "Customer": "Jason Sweet",
  "Quantity": 1,
  "Price": 18.00
}`;
request.send(data);


// var blob = ['GmnQPBU+nyRGER4JPAW4DjDQC19D'];
// var xhr = new XMLHttpRequest();
// // Add any event handlers here...
// xhr.open('POST', "http://localhost:8080");
// xhr.send(blob);