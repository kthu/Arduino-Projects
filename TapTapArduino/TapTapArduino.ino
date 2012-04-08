/* 
 * TapTap Arduino
 * 
 * Listens for tap codes from a button. Lights up led when tap code matches password
 */
#include <stdio.h>

#define DEBUG true
#define AXISVALTICKS 15
#define RESOLUTION 10
#define SILENTTHRESHHOLD 2

#define REDLEDPIN 2
#define GREENLEDPIN 3
#define BUTTONPIN 13
#define PASSWORDLENGTH 5

static String password = "HELLO";
static char alphabet[5][5] = {{'A','B','C','D','E' },{'F','G','H','I','J'},{'L','M','N','O','P'},{'Q','R','S','T','U'},{'V','W','X','Y','Z'}};
   
struct queue {
   char code[PASSWORDLENGTH+1];
   int insert_point;  
} code_queue;

void debug(char* message) {
#ifdef DEBUG
   Serial.println(message);
#endif
}

void set_status_ok() {
   debug("Recieved correct tap code");
   digitalWrite(REDLEDPIN, LOW);
   digitalWrite(GREENLEDPIN, HIGH);
}

void set_status_not_ok() {
   debug("No matching tap code recieved");
   digitalWrite(REDLEDPIN, HIGH);
   digitalWrite(GREENLEDPIN, LOW);
}

int get_axis_value() {
   int silent_duration = 0;
   int value = 0;
   int tick_count = 0;
   boolean tap = false;
   while (true) {
      tap  = (digitalRead(BUTTONPIN) == LOW);
      if (tick_count == AXISVALTICKS) {
         tick_count = 0;
         if (tap) {
            silent_duration = 0;
            value++;
            tap = false;
            debug("tap");
            while (digitalRead(BUTTONPIN) == LOW) {
               //Wait for "off" signal to prevent repeats
            }
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
      delay (RESOLUTION);
   }
}

String get_entered_code() {
   int pointer = code_queue.insert_point;
   if (pointer >= PASSWORDLENGTH) pointer = 0;
   String queue = String(code_queue.code);
   String entered = queue.substring(pointer+1) + queue.substring(0, pointer+1);
#ifdef DEBUG
   char entered_char_array[entered.length()+1];
   entered.toCharArray(entered_char_array, entered.length()+1);
   debug(entered_char_array);
#endif
   return entered;
}

boolean password_ok(String entered_code) {
   return entered_code.equals(password);
}

/* Adds a character to the circular queue */
void add_char_to_code(char letter) {
   if (code_queue.insert_point == PASSWORDLENGTH - 1) {    /*step-1*/
      code_queue.insert_point = 0;    /*step-2*/
   } else {
      code_queue.insert_point++;   /*step-3*/
   }

   code_queue.code[code_queue.insert_point] = letter; /*step-b*/
}

void setup() {
#ifdef DEBUG
   Serial.begin(9600);
#endif
   pinMode(GREENLEDPIN, OUTPUT);
   pinMode(REDLEDPIN, OUTPUT);
   pinMode(BUTTONPIN, INPUT);

   //Initialize code queue;
   for (int i=0; i <= PASSWORDLENGTH; i++) {
      add_char_to_code('_');
   }
   code_queue.code[PASSWORDLENGTH] = NULL;
   code_queue.insert_point=-1;

   set_status_not_ok();
}

void loop() {
   debug("X");
   int x = get_axis_value();
   debug("Y");
   int y = get_axis_value();
   char letter = alphabet[x-1][y-1];
   add_char_to_code(letter);

   if (password_ok(get_entered_code()) == true) {
      set_status_ok();
   } else {
      set_status_not_ok();
   }
}
