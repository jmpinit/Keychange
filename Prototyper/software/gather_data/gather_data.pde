import processing.serial.*;
import java.nio.ByteBuffer;
import java.util.Vector;

final int NUM_KEYS = 10;

Serial typerPort;

Thread collectorThread;
int firstKey, secondKey;
boolean collecting = true;
Table dataTable;

void setup () {
  size(300, 200);
  
  dataTable = new Table();
  dataTable.addColumn("from");
  dataTable.addColumn("to");
  dataTable.addColumn("millis");
  
  println(Serial.list());
  typerPort = new Serial(this, Serial.list()[2], 115200);
  typerPort.write(0xff);
  
  collectorThread = new Thread(new Runnable() {
    public void run() {
      while(collecting) {
        // pick two keys
        firstKey = int(random(10));
        secondKey = int(random(10));
        while(firstKey == secondKey)
          secondKey = int(random(10));
  
        // ask hardware to measure the time it takes to press them in order
        // and add data to the data table
        try {
          TableRow newRow = dataTable.addRow();
          newRow.setInt("from", firstKey);
          newRow.setInt("to", secondKey);
          newRow.setInt("millis", sampleKeyPair(firstKey, secondKey));
        } catch(Exception e) {
          e.printStackTrace();
        }
        
        println("sampled keys");
      }
    }
  });
}

void draw() {
  // reformat key indices
  int[] leftStates = new int[5];
  int[] rightStates = new int[5];
  
  if(firstKey < 5)
    rightStates[5-firstKey-1] = -1;
  else
    leftStates[firstKey%5] = -1;
    
  if(secondKey < 5)
    rightStates[5-secondKey-1] = 1;
  else
    leftStates[secondKey%5] = 1;
    
  // render
  background(255);
  
  pushMatrix();
  translate(width/2, height);
  
  pushMatrix();
  translate(-100, 0);
  scale(-1, 1);
  drawKeyOutlines(leftStates);
  popMatrix();
  
  pushMatrix();
  translate(100, 0);
  drawKeyOutlines(rightStates);
  popMatrix();
  
  popMatrix();
}

void drawKeyOutlines(int[] keyStates) {
  Pt[] fingers = {
    new Pt(-88, -80),
    new Pt(-65, -136),
    new Pt(-38, -145),
    new Pt(-12, -135),
    new Pt(10, -110)
  };
  
  noStroke();
  for(int i=0; i < fingers.length; i++) {
    Pt pt = fingers[i];
    
    switch(keyStates[i]) {
      case -1:
        fill(0, 255, 0);
        break;
      case 1:
        fill(255, 0, 0);
        break;
      default:
        fill(0);
    }
    
    rect(pt.x, pt.y, 20, 20);
  }
}

void keyPressed() {
  if(collecting) {
    try {
      collecting = false;
      collectorThread.join();
      
      saveTable(dataTable, "data/keyData.csv");
      println("saved data to csv file");
    } catch(Exception e) {
      e.printStackTrace();
    }
  }
}

void mousePressed() {
  if(!collectorThread.isAlive() && collecting)
    collectorThread.start();
}

int sampleKeyPair(int firstKey, int secondKey) throws Exception {
  println("sampling");
  if(firstKey < NUM_KEYS && secondKey < NUM_KEYS) {
    int command = (secondKey << 4) | firstKey;
    
    typerPort.write((byte)command);
    while(typerPort.available() == 0) {}
    
    byte[] data = new byte[4];
    int len = typerPort.readBytes(data);
    
    if(len == 4) {
      ByteBuffer buffer = ByteBuffer.allocate(4);
      buffer.put(data);
      buffer.flip();
      return buffer.getInt();
    } else {
      throw new Exception("Prototyper returned " + len + " bytes instead of the expected 4.");
    }
  } else {
    throw new Exception("Key does not exist.");
  }
}

class KeyRelation {
  private int millis;
  private int first, second;
  
  public KeyRelation(int t, int a, int b) {
    millis = t;
    first = a;
    second = b;
  }
}

class Pt {
  public float x, y;
  public Pt(float _x, float _y) {
    x = _x;
    y = _y;
  }
}
