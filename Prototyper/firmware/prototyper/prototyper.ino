#define NUM_KEYS 10
#define DEBOUNCE 5

#define LED_PIN 13

enum state {
  LISTEN_STATE,
  FIRST_KEY_STATE,
  SECOND_KEY_STATE
};

enum state theState;
unsigned char firstKey, secondKey;
unsigned long firstTime;

unsigned long bounceTimes[NUM_KEYS];

void setup() {
  Serial.begin(115200);
  theState = LISTEN_STATE;
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  switch(theState) {
    case LISTEN_STATE:
      if(Serial.available() > 0) {
        unsigned char command = Serial.read();
        firstKey = command & 0xF;
        secondKey = command >> 4;
        
        if(firstKey < NUM_KEYS && secondKey < NUM_KEYS) {
          theState = FIRST_KEY_STATE;
          digitalWrite(LED_PIN, HIGH);
        }
      }
      break;
    case FIRST_KEY_STATE:
      if(keyPressed(firstKey)) {
        firstTime = millis();
        theState = SECOND_KEY_STATE;
      }
      
      break;
    case SECOND_KEY_STATE:
      if(keyPressed(secondKey)) {
        unsigned long timePassed = millis() - firstTime;
        
        unsigned char* timeData = (unsigned char*)(&timePassed);
        for(int i=sizeof(unsigned long)-1; i >= 0; i--)
          Serial.write(timeData[i]);
        
        theState = LISTEN_STATE;
        digitalWrite(LED_PIN, LOW);
      }
      
      break;
  }
}

boolean keyPressed(unsigned int key) {
  boolean pressed = false;
  
  static unsigned int last = 0;
  unsigned int keys = readKeys();
  
  if(keys != last) {
    // which keys changed?
    unsigned int diff = keys ^ last;
    
    if((diff & (1 << key)) > 0) {
      // the key's state changed
      unsigned long timeNow = millis();
      if(timeNow - bounceTimes[key] > DEBOUNCE) {
        bounceTimes[key] = timeNow;
        // the key is not bouncing
        
        pressed = (keys & (1 << key)) > 0;
      }
    }
  }
  
  last = keys;
  
  return pressed;
}

unsigned int readKeys() {
  unsigned int keys = 0;
  
  for(int i=0; i < NUM_KEYS; i++) {
    if(digitalRead(i+2)) {
      keys |= 1 << i;
    }
  }
  
  return keys;
}
