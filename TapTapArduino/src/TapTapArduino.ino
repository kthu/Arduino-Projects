/* 
 * TapTap Arduino
 * 
 * Listens for tap codes from a button. Lights up led when tap code matches password
 */
#include <stdio.h>

#define DEBUG true
#define AXISVALTICKS 15
#define RESOLUTION 2
#define SILENTTHRESHHOLD 20
#define BLINKDURATION 100

#define REDLEDPIN 2
#define GREENLEDPIN 3
#define BUTTONPIN 13

static String password = "HALLO";
int passwordLength = 0;
static char alphabet[5][5] = {{'A','B','C','D','E' },{'F','G','H','I','J'},{'L','M','N','O','P'},{'Q','R','S','T','U'},{'V','W','X','Y','Z'}};
   
struct queue {
   char* code;
   int insertPoint;  
} codeQueue;

void debug(char* message) {
#ifdef DEBUG
   Serial.println(message);
#endif
}

void setStatusOk() {
   debug("Recieved correct tap code");
   digitalWrite(REDLEDPIN, LOW);
   digitalWrite(GREENLEDPIN, HIGH);
}

void tapFeedback() {
   debug("tap");
   digitalWrite(GREENLEDPIN, HIGH);
   delay(BLINKDURATION);
   digitalWrite(GREENLEDPIN, LOW);
   digitalWrite(REDLEDPIN, HIGH);
}

void setStatusNotOk() {
   debug("No matching tap code recieved");
   digitalWrite(REDLEDPIN, HIGH);
   digitalWrite(GREENLEDPIN, LOW);
}

int getAxisValue() {
   int silentDuration = 0;
   int value = 0;
   int tickCount = 0;
   int tapVal=0;
   boolean tap = false;
   while (true) {
      tap  = (digitalRead(BUTTONPIN) == LOW);
      tapVal = (analogRead(BUTTONPIN));
#ifdef DEBUG
      Serial.println(tapVal);
#endif
      if (tickCount == AXISVALTICKS) {
         tickCount = 0;
         if (tap) {
            silentDuration = 0;
            value++;
            tap = false;
            tapFeedback();
            while (digitalRead(BUTTONPIN) == LOW) {
               //Wait for "off" signal to prevent repeats
            }
         } else {
            if (value > 0) {
               silentDuration++;
               if (silentDuration > SILENTTHRESHHOLD) {
                  return value;
               }
            }
         }
      }

      if (value >= sizeof(alphabet[0])) {
         return value;
      }

      tickCount++;
      delay (RESOLUTION);
   }
}

String getEnteredCode() {
   int pointer = codeQueue.insertPoint;
   if (pointer >= passwordLength) pointer = 0;
   String queue = String(codeQueue.code);
   String entered = queue.substring(pointer+1) + queue.substring(0, pointer+1);
#ifdef DEBUG
   char enteredCharArray[entered.length()+1];
   entered.toCharArray(enteredCharArray, entered.length()+1);
   debug(enteredCharArray);
#endif
   return entered;
}

boolean passwordOk(String enteredCode) {
   return enteredCode.equals(password);
}

/* Adds a character to the circular queue */
void addCharToCode(char character) {
   if (codeQueue.insertPoint == passwordLength - 1) {    /*step-1*/
      codeQueue.insertPoint = 0;    /*step-2*/
   } else {
      codeQueue.insertPoint++;   /*step-3*/
   }

   codeQueue.code[codeQueue.insertPoint] = character; /*step-b*/
}

void setup() {
#ifdef DEBUG
   Serial.begin(9600);
#endif
   pinMode(GREENLEDPIN, OUTPUT);
   pinMode(REDLEDPIN, OUTPUT);
   pinMode(BUTTONPIN, INPUT);

   //Initialize code queue;
   passwordLength = password.length(); 
   codeQueue.code = (char*) malloc(passwordLength + 1);
   for (int i=0; i <= passwordLength; i++) {
      addCharToCode('_');
   }
   codeQueue.code[passwordLength] = NULL;
   codeQueue.insertPoint=-1;

   setStatusNotOk();
}

void loop() {
   debug("ROW");
   int x = getAxisValue();
   debug("COLUMN");
   int y = getAxisValue();
   char letter = alphabet[x-1][y-1];
   addCharToCode(letter);

   if (passwordOk(getEnteredCode()) == true) {
      setStatusOk();
   } else {
      setStatusNotOk();
   }
}
