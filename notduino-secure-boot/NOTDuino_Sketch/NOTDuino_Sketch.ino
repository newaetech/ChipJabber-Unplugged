/*
 * NOTDuino Example Sketch.
 * 
 * This is programmed into the MEGA328P by default in addition to the OptiBoot bootloader.
 * 
 */

// requires the Wire library for i2c for EEPROM
#include <Wire.h>

// On-Board LEDs
#define LED_BOOT 8
#define LED_OK   9
#define LED_FAIL 10
#define LED_GLITCH 11
#define LED_NORMAL 12


/* This isn't called by default - if you have your own EEPROM chip, you'll need to program it */
void program() {
  Wire.beginTransmission(0x50);
  Wire.write(0xA7);
  Wire.write(0xDE); //Write 0xDE to 0xA7
  Wire.endTransmission();
}

/* Return if we are passing 'secure boot' checks */
int boot_ok() {

  // Check if SCL line is pegged low, if so abort rest of checks.
  // If you don't check this, the routine just hangs (but you can glitch past that too!).  
  if (digitalRead(A5) == 0) {
    return 0;
  }

  // Read address 0xA7 from the EEPROM device
  Wire.beginTransmission(0x50);
  Wire.write(0xA7);
  Wire.endTransmission();
  Wire.requestFrom(0x50, 1);

  // Should have byte programmed as '0xDE'
  if (Wire.read() != 0xDE) {
    return 0;
  }

  return 1;
}

void setup() {
  // initialize the digital pin as an output.
  digitalWrite(LED_BOOT, LOW);
  pinMode(LED_BOOT, OUTPUT);

  //Turn on i2c for EEPROM
   Wire.begin();

  pinMode(LED_NORMAL, OUTPUT);
  pinMode(LED_GLITCH, OUTPUT);
  pinMode(LED_FAIL, OUTPUT);
  pinMode(LED_OK, OUTPUT);
  digitalWrite(LED_OK, LOW);
  digitalWrite(LED_FAIL, LOW);

  //If you have a blank EEPROM - uncomment following 3 lines, run once,
  //then recommend them again. Don't want to reprogram it every time.
  //delay(1);
  //program();
  //delay(5);

  Serial.begin(9600);

  Serial.println("NOTDuino Secure Boot System Started");

  digitalWrite(LED_BOOT, HIGH);

  if(boot_ok() == 0) {
    digitalWrite(LED_FAIL, HIGH);
    Serial.print("Boot FAILED - application blocked\n");
    while(1);
  } else {
    digitalWrite(LED_OK, HIGH);
    Serial.print("Boot OK\n");
  }
  
  digitalWrite(LED_BOOT, LOW);
}

// the loop routine runs over and over again forever:
void loop() {
  volatile unsigned int i,j,cnt;

  delay(100);

  digitalWrite(LED_NORMAL, LOW);
  digitalWrite(LED_GLITCH, LOW);
  
  cnt = 0;
  for (i=0; i < 200; i++){
    for(j=0; j < 200; j++){
      cnt++;  
    }
  }
  if(cnt == 40000){
    digitalWrite(LED_NORMAL, HIGH);
  } else {
    digitalWrite(LED_GLITCH, HIGH);
  }

  Serial.print("Loop value: ");
  Serial.print(cnt);
  Serial.print("\n");  
}
