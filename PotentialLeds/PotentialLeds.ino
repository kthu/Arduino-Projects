/* Analog Read to LED
 * ------------------ 
 *
 * Enables a number of leds according to a potentiometer 
 * on pin 2. Blinks them like a maniac when value gets 
 * over 50% of potentiometers potential 
 */

int potPin = 2;
int ledPins [] = { 7, 9, 11, 13 };

static int POTMIN = 0;
static int POTMAX = 750;

static int INCREASE = 1;
static int DECREASE = -1;

inc direction = INCREASE;

int numberOfLeds (sizeof(ledPins) / sizeof(int));
int highestLedPotVal = (POTMAX - (POTMAX / numverOfLeds));
boolean alert = false;
int alertLed = 0;
int val = 0; 

void setup() {
  for (int i=0; i < numberOfLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  
  Serial.begin(9600);
}

void loop() {
  val = analogRead(potPin);    // read the value from the sensor
  Serial.println(val);   
  
  alert = (val > (POTMAX));
    
  if (alertLed == numberOfLeds) {
      direction = DECREASE;
  }
  
  if (alertLed == -1) {
      direction = INCREASE;
  }
  
  alertLed = alertLed + direction;

  if (alert) {
    for (int i=0; i < numberOfLeds); i++) {
      if (i == alertLed) {
           digitalWrite(ledPins[i], HIGH);    
      } else {
           digitalWrite(ledPins[i], LOW);    
      }
    }
    

  } else {
    for (int i=0; i < numberOfLeds; i++) {    
      if (val > (highestLedPotVal / (i + 1)) ) {  
        digitalWrite(ledPins[i], HIGH);
      } else {
        digitalWrite(ledPins[i], LOW);      
      }
    }
  }  
    
  delay(100);          // not so fast!
}
