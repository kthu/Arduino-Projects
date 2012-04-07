/* 
 * TapTap Arduino
 */

#define DEBUG true

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

int get_axis() {
   int silent_duration = 0;
   int value = 0;
   int tick_count = 0;
   boolean tap = false;
   while (true) {
      tap  = (digitalRead(buttonPin) == HIGH);
      if (tick_count == AXISVALTICKS) {
         tick_count = 0;
         if (tap) {
            silent_duration = 0;
            value++;
            tap = false;
         } else {
            if (value > 0) {
               silent_duration++;
               if (silent_duration > SILENTTHRESHHOLD) {
                  return value;
               }
            }
         }
      }

      if (value >= sizeof(alphabet)) {
         return value;
      }

      tick_count++;
      delay (TICKSDELAY);
   }
}

boolean password_ok() {
   return false;
}

void loop() {
   int x = get_axis()-1;
   int y = get_axis()-1;
   char letter = alphabet[x][y];
#ifdef DEBUG
   Serial.print(letter);
#endif

   if (password_ok) {
      digitalWrite(redLedPin, LOW);
      digitalWrite(greenLedPin, HIGH);
   } else {
      digitalWrite(redLedPin, HIGH);
      digitalWrite(greenLedPin, LOW);
   }
}
