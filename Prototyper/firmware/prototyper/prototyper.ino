#define NUM_KEYS 5
#define DEBOUNCE 5

unsigned long bounceTimes[NUM_KEYS];

void setup() {
  Serial.begin(115200);
}

unsigned int last = 0;
void loop() {
  unsigned int keys = readKeys();
  
  if(keys != last) {
    unsigned int diff = keys ^ last;
    unsigned int keybuff = keys;
    unsigned int updates = 0;
    
    for(int i=0; diff != 0; i++, diff >>= 1, keybuff >>= 1) {
      if(diff & 1) {
        unsigned long timeNow = millis();
        if(timeNow - bounceTimes[i] > DEBOUNCE) {
          bounceTimes[i] = timeNow;
          updates |= (keybuff & 1) << i;
        }
      }
    }
    Serial.write(updates & 0xff);
  }
  last = keys;
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
