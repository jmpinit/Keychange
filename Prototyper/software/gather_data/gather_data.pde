import processing.serial.*;
import java.nio.ByteBuffer;

final int NUM_KEYS = 10;

Serial typerPort;
 
void setup () {
  size(400, 300);
  
  println(Serial.list());
  typerPort = new Serial(this, Serial.list()[2], 115200);
  typerPort.write(0xff);
}

void draw() {}

void keyPressed() {
  try {
    println((float)sampleKeyPair(0, 1)/1000.f, " seconds between keypresses.");
  } catch(Exception e) {
    e.printStackTrace();
  }
}

int sampleKeyPair(int firstKey, int secondKey) throws Exception {
  println("writing");
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
