
const char MAIN_page[] PROGMEM = R"=====(

<style>
  body {background-color: #FF0000; font-family: Times; font-size: 400% ; text-align: center; margin:1px auto; padding-top: 300px; width:auto; user-select: none;}
</style>
<body>     
 
  <h1 style="font-size:80%;">Perky Needs a Bluetooth Controller - None Connected  press @B on the bluetooth controller then another button</h1>


<script type="text/javascript">

// global gamepad object
let gamepadIndex;
window.addEventListener('gamepadconnected', (event) => {
  gamepadIndex = event.gamepad.index;
});

  let paused = false;
  let shifted = false;
  let shiftedfast = false;
  let Forward = true;


setInterval(() => {

  if(gamepadIndex !== undefined) {
    // a gamepad is connected and has an index
   
    const myGamepad = navigator.getGamepads()[gamepadIndex];
    document.body.style.backgroundColor = "green" ;
    document.body.innerHTML = "  Perky Goes Wild!"; // reset page
 
    let x = myGamepad.axes[0];
    let y = myGamepad.axes[1];
        
    if(y > 0.5 && !paused){toggle("L");}  // steer left
           
    if(y < -.5 && !paused){toggle("R");}  // steer Right


    if(x > 0.5 && !paused){                          //  joystick back pressed
          if(!shifted){toggle("C"); shifted = true; Forward = false;}  //  back slow
          else{if(Forward){ 
                     if(shiftedfast){toggle("B");  shiftedfast = false;}  //  forward slow
                     else {toggle("off"); shifted = false; } // stopped
                          }
               else      
                     {toggle("D"); shiftedfast = true;} // back fast
              }        
     }

    if(x < -0.5 && !paused){                        //  joystick forward pressed
          if(!shifted){toggle("B"); shifted = true; Forward = true;}  //  forward slow
          else{if(!Forward){ 
                     if(shiftedfast){toggle("C"); shiftedfast = false;}  //  back slow
                     else {toggle("off"); shifted = false; }             // stopped
                          }
               else      
                     {toggle("A"); shiftedfast = true;} // forward fast
              }        
     }


              
    myGamepad.buttons.map(e => e.pressed).forEach((isPressed, buttonIndex) => { 
      if(isPressed) {
     
        if(buttonIndex == 1){ toggle("FO");}                   //  lights off and on
        if(buttonIndex == 2){ toggle("Flon");}
      
        if(buttonIndex == 4){ toggle("off"); shifted = false; shiftedfast = false;}                 // stop the motors
        if(buttonIndex == 5){                                   
                            if( myGamepad.axes[1] < 0.2 && myGamepad.axes[1] > -0.2){ paused = false;}   // keep steering the car
                            else {if( myGamepad.axes[1] > 0.5){toggle("HL"); paused = true;}  // turn head lefter
                                     else {toggle("HR"); paused = true;}          // turn head righter 
                                 }                 
                            }     
      }
    }
   
   
  }
  
}, 100) // print buttons that are pressed 10 times per second

   function toggle(x) {
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/" + x, true);
     xhr.send();
   }

</script>
</body>
</html>

 )====="; 

