 const char MAIN_page[] PROGMEM = R"=====( 


<style>
  body {background-color: #FF0000; font-family: Times; font-size: 400% ; text-align: center; margin:1px auto; padding-top: 300px; width:auto; user-select: none;}
</style>
<body>     
 
  <h1 style="font-size:80%;">Perky Needs a Bluetooth Controller - None Connected  press @B on the bluetooth controller then another button</h1>
</body>

<script type="text/javascript">

// global gamepad object
let gamepadIndex;
window.addEventListener('gamepadconnected', (event) => {
  gamepadIndex = event.gamepad.index;
});

  let paused = true;


setInterval(() => {

  if(gamepadIndex !== undefined) {
    // a gamepad is connected and has an index
   
    const myGamepad = navigator.getGamepads()[gamepadIndex];
    document.body.style.backgroundColor = "green" ;
    document.body.innerHTML = "  Perky Goes Wild!"; // reset page
 
    let x = myGamepad.axes[0];
    let y = myGamepad.axes[1];
        
    if(y > 0.5 && !paused){toggle("I");}  // spin left
       
    if(y < -.5 && !paused){toggle("J");}  // spin right
         
    if(x > 0.5 && !paused){toggle("G");}  //  back
     
    if(x < -.5 && !paused){toggle("B");}  //  forward
     
  
   
    myGamepad.buttons.map(e => e.pressed).forEach((isPressed, buttonIndex) => { 
      if(isPressed) {
     
        if(buttonIndex == 1){ toggle("FO");}                   //  lights off and on
        if(buttonIndex == 2){ toggle("Flon");}
      
        if(buttonIndex == 4){ toggle("off");}                 // stop the motors
        if(buttonIndex == 5){ paused = true;}                 // switch to head movement  
          else { paused = false}

        if(buttonIndex == 5 && myGamepad.axes[1] > 0.5){toggle("A");}   // turn head lefter
        if(buttonIndex == 5 && myGamepad.axes[1] < -0.5){toggle("C");}  // turn head righter 
        
        // button is pressed; indicate this on the page.  Uncomment the next two lines, very handy for debugging  and identifying the buttons.

        //  document.body.style.backgroundColor = "blue" ;
        //  document.body.innerHTML += `<h1>Button ${buttonIndex} is pressed  </h1>`;
      
      }
    })
   
   
  }
}, 100) // print buttons that are pressed 10 times per second

   function toggle(x) {
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/" + x, true);
     xhr.send();
   }

</script>

 )====="; 
