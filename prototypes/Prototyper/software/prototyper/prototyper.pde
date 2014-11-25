import processing.serial.*;
import java.awt.Robot;
import java.awt.event.KeyEvent;

Serial myPort;
Robot rob;

String symbols = "aeorisn1tl2md0cp3hbuk45g9687yfwjvzxqASERBTMLNPOIDCHGKFJUW.!Y*@V-ZQX_$#,/+?;^ %~=&`\)][:<(æ>\"|{'}"

void setup () {
  size(400, 300);        
  
  myPort = new Serial(this, "COM13", 115200);
  
  try {
    rob = new Robot();
  } catch(Exception e) {
    e.printStackTrace();
  }
  
  // set inital background:
  background(0);
}

int last = 0;
void draw () {
  while(myPort.available() > 0) {
    int keys = myPort.readChar();
    
    if(keys != last) {
      int diff = keys ^ last;
      
      println(keys);
      for(int i=0; diff != 0; i++) {
        if((diff & 1) != 0) {
          if((keys & 1) != 0) {
            // press
            switch(i) {
              case 0:
                rob.keyPress(KeyEvent.VK_A);
                break;
              case 1:
                rob.keyPress(KeyEvent.VK_B);
                break;
              case 2:
                rob.keyPress(KeyEvent.VK_C);
                break;
              case 3:
                rob.keyPress(KeyEvent.VK_D);
                break;
              case 4:
                rob.keyPress(KeyEvent.VK_E);
                break;
            }
          } else {
            // release
            switch(i) {
              case 0:
                rob.keyRelease(KeyEvent.VK_A);
                break;
              case 1:
                rob.keyRelease(KeyEvent.VK_B);
                break;
              case 2:
                rob.keyRelease(KeyEvent.VK_C);
                break;
              case 3:
                rob.keyRelease(KeyEvent.VK_D);
                break;
              case 4:
                rob.keyRelease(KeyEvent.VK_E);
                break;
            }
          }
        }
        
        diff = diff >>> 1;
        keys = keys >>> 1;
      }
      
      last = keys;
    }
  }
}
