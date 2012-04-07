/* 
 * TapTap Arduino
 */

int redLedPin = 3;
int greenLedPin = 2;
int buttonPin = 13;

static int AXISVALTICKS=10;
static int TICKSDELAY=100;
static int SILENTTHRESHHOLD=2;
static char PASSWORD [3] = "GN";

static char alphabet [5][5]= {{'A','B','C','D','E' },{'F','G','H','I','J'},{'L','M','N','O','P'},{'Q','R','S','T','U'},{'V','W','X','Y','Z'}};

void setup() {
#ifdef DEBUG
   Serial.begin(9600);
#endif
   pinMode(greenLedPin, OUTPUT);
   pinMode(redLedPin, OUTPUT);
   pinMode(buttonPin, INPUT);
}

void loop() {
   int val = digitalRead(buttonPin);
   if (val == LOW) {
      digitalWrite(redLedPin, LOW);
      digitalWrite(greenLedPin, HIGH);
   } else {
      digitalWrite(redLedPin, HIGH);
      digitalWrite(greenLedPin, LOW);
   }
   delay(TICKSDELAY);          // not so fast!
}
