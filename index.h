const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
    <head>
        <title>Great Projects</title>
        <script>
      function DisplayCurrentTime() {
          var dt = new Date();
  var weekday = new Array(7);
  weekday[0] = "Sunday";
  weekday[1] = "Monday";
  weekday[2] = "Tuesday";
  weekday[3] = "Wednesday";
  weekday[4] = "Thursday";
  weekday[5] = "Friday";
  weekday[6] = "Saturday";
  var dow = weekday[dt.getDay()];
  document.getElementById("datetime").innerHTML = (dow) +" "+ (dt.toLocaleString());
        setTimeout('DisplayCurrentTime()', 1000);
      }
      document.addEventListener('DOMContentLoaded', function() {
        DisplayCurrentTime();
      }, false);
    </script>
      <style>
body {font-family: Arial, Helvetica, sans-serif;}

/* Full-width input fields */
input[type=text], input[type=password] {
  width: 100%;
  padding: 12px 20px;
  margin: 8px 0;
  display: inline-block;
  border: 1px solid #ccc;
  box-sizing: border-box;
}
.container {
  padding: 16px;
}

span.psw {
  float: right;
  padding-top: 16px;
}

/* The Modal (background) */
.modal {
  display: none; /* Hidden by default */
  position: fixed; /* Stay in place */
  z-index: 1; /* Sit on top */
  left: 0;
  top: 0;
  width: 100%; /* Full width */
  height: 100%; /* Full height */
  overflow: auto; /* Enable scroll if needed */
  background-color: rgb(0,0,0); /* Fallback color */
  background-color: rgba(0,0,0,0.4); /* Black w/ opacity */
  padding-top: 60px;
}

/* Set a style for all buttons */
button {
  background-color: #4CAF50;
  color: white;
  padding: 14px 20px;
  margin: 8px 0;
  border: none;
  cursor: pointer;
  width: 100%;
}

button:hover {
  opacity: 0.8;
}

/* Modal Content/Box */
.modal-content {
  background-color: #fefefe;
  margin: 5% auto 15% auto; /* 5% from the top, 15% from the bottom and centered */
  border: 1px solid #888;
  width: 40%; /* Could be more or less, depending on screen size */
}
/* Add Zoom Animation */
.animate {
  -webkit-animation: animatezoom 0.6s;
  animation: animatezoom 0.6s
}

@-webkit-keyframes animatezoom {
  from {-webkit-transform: scale(0)} 
  to {-webkit-transform: scale(1)}
}
  
@keyframes animatezoom {
  from {transform: scale(0)} 
  to {transform: scale(1)}
}

/* Change styles for span and cancel button on extra small screens */
@media screen and (max-width: 300px) {
  span.psw {
     display: block;
     float: none;
  }
  .cancelbtn {
     width: 100%;
  }
}
.slider {
  -webkit-appearance: none;
  width: 100%;
  height: 15px;
  background: #ffff00;
  outline: none;
  opacity: 0.7;
  -webkit-transition: .2s;
  transition: opacity .2s;
}

.slider:hover {
  opacity: 1;
}


.slider::-moz-range-thumb {
  width: 15px;
  height: 25px;
  background: #FF0F50;
  cursor: pointer;
}
  .data-input {
  text-align: center;
  background-color: #bbbbff;
  font-size:26px;
  color:red;
  border: 5px solid #444444;
  -moz-border-radius: 7px;
  -webkit-border-radius: 7px;
  }
  .button-blue {
  padding: 5px 5px 5px 5px;
  width: 100%;
  border: #fbfb00 solid 3px;
  background-color: #4444ff;
  color:white;
  font-size:20px;
  padding-bottom:5px;
  font-weight:700;
  -moz-border-radius: 7px;
  -webkit-border-radius: 7px;
  }
  .button-blue:hover {
  background-color: #2222aa;
  color: #ff93bd;
  }
  .text {
  background-color: #ff0000;
  font-size:76px;
  color: #ffff99;
  }
      marquee{
  width: 100%;
      font-size: 30px;
  font-weight:700;
      color: #ff0000;
      font-family: sans-serif;
      }
  table {
  text-align: center;
  width:100%;
  color: #0000ff;
  -moz-border-radius: 7px;
  -webkit-border-radius: 7px;
  }
  h1 {
  color: #ff0000;
  background-color: #ffff00;
  -moz-border-radius: 7px;
  -webkit-border-radius: 7px;
  }
  div {
  border: 2px solid #ff0000;
  background-color: #bbbbff;
  color: #ff0000;
  -moz-border-radius: 7px;
  -webkit-border-radius: 7px;
  }
  </style>
    </head>
    <body>
   <tr><div class="data-input">
   <b style= "color: #ff0000;font-size:35px"><span id="datetime"></span></b>
  <button onclick="document.getElementById('id01').style.display='block'" class="button-blue" style="width:auto">Connect to Wi-Fi</button>
<div id="id01" class="modal">
  <form class="modal-content animate" action="/Mywifi" method="POST">
    <div  class="data-input">
      <label for="usname"><b>Wifi Ssid</b></label>
      <input type="text" placeholder="Enter Ssid" name="usname" required>
      <label for="pssw"><b>Password</b></label>
      <input type="password" placeholder="Enter Password" name="pssw">
      <button type="submit" class="button-blue"><b>Connect</b></button>
    </div>
  </form>
</div>

<script>
// Get the modal
var modal = document.getElementById('id01');

// When the user clicks anywhere outside of the modal, close it
window.onclick = function(event) {
    if (event.target == modal) {
        modal.style.display = "none";
    }
}
</script>
  </div>
  </tr>
   <tr>
<form action="/MesSpeed2" method="POST"><div class="data-input">
<div class="slidecontainer">
  <input type="range" name="Speed2" min="0" max="100" value="50" class="slider" style="width:60%" id="mySpeed2">
  <b> <span id="speed2"></span></b>
  <input type="submit" value="Set" style="width:20%" class="button-blue">
</div>
  </div><script>
var slider2 = document.getElementById("mySpeed2");
var output2 = document.getElementById("speed2");
output2.innerHTML = slider2.value;

slider2.oninput = function() {
  output2.innerHTML = this.value;
}
</script>
</form>
  </tr>
   <tr>
<form action="/MesSpeed3" method="POST">
  <div class="data-input">
<div class="slidecontainer">
  <input type="range" name="Speed3" min="0" max="50" value="10" class="slider" style="width:60%" id="mySpeed3">
  <b> <span id="speed3"></span></b>
  <input type="submit" value="Set" style="width:20%" class="button-blue">
  </div></div>
<script>
var slider3 = document.getElementById("mySpeed3");
var output3 = document.getElementById("speed3");
output3.innerHTML = slider3.value;

slider3.oninput = function() {
  output3.innerHTML = this.value;
}
</script>
</form>
  </tr>
   <tr>
<form action="/MesSpeed5" method="POST">
  <div class="data-input">
<div class="slidecontainer">
  <input type="range" name="Speed5" min="0" max="20" value="10" class="slider" style="width:60%" id="mySpeed5">
  <b> <span id="speed5"></span></b>
  <input type="submit" value="Set" style="width:20%" class="button-blue">
  </div></div>
<script>
var slider5 = document.getElementById("mySpeed5");
var output5 = document.getElementById("speed5");
output5.innerHTML = slider5.value;

slider5.oninput = function() {
  output5.innerHTML = this.value;
}
</script>
</form>
  </tr>
   <tr>
<form action="/MesSpeed4" method="POST"><div class="data-input">
<div class="slidecontainer">
  <input type="range" name="Speed4" min="1" max="20" value="10" class="slider" style="width:60%" id="mySpeed4">
  <b> <span id="speed4"></span></b>
  <input type="submit" value="Set" style="width:20%" class="button-blue">
</div>
  </div><script>
var slider4 = document.getElementById("mySpeed4");
var output4 = document.getElementById("speed4");
output4.innerHTML = slider4.value;

slider4.oninput = function() {
  output4.innerHTML = this.value;
}
</script>
</form>
  </tr>
    </body>
</html>
)=====";
