/* 
 * TapTap Arduino
 * 
 * Listens for tap codes from a button. Lights up led when tap code matches password
 */

#define DEBUG true
#define AXISVALTICKS 10
#define RESOLUTION 20
#define SILENTTHRESHHOLD 2

#define REDLEDPIN 2
#define GREENLEDPIN 3
#define BUTTONPIN 13
#define PASSWORDLENGTH 5

static char password[] = "HALLO";
static char alphabet[5][5] = {{'A','B','C','D','E' },{'F','G','H','I','J'},{'L','M','N','O','P'},{'Q','R','S','T','U'},{'V','W','X','Y','Z'}};
   
struct queue {
   char code[PASSWORDLENGTH+1];
   int insert_point;  
} code_queue;

void set_status_ok() {
   digitalWrite(REDLEDPIN, LOW);
   digitalWrite(GREENLEDPIN, HIGH);
}

void set_status_not_ok() {
   digitalWrite(REDLEDPIN, HIGH);
   digitalWrite(GREENLEDPIN, LOW);
}

void debug(char* message) {
#ifdef DEBUG
   Serial.println(message);
#endif
}

int get_axis() {
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
            }
            debug("released");
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

boolean password_ok(char* password) {
   char entered_code[PASSWORDLENGTH+1];
   sprintf(entered_code, "%.*s%.*s", PASSWORDLENGTH - 1 - code_queue.insert_point, code_queue.code[code_queue.insert_point], code_queue.insert_point, code_queue.code);
   debug(code_queue.code);
   debug(entered_code);
   
   return false;
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

   code_queue.code[PASSWORDLENGTH] = NULL;
   code_queue.insert_point=-1;
   set_status_not_ok();
}

void loop() {
   int x = get_axis()-1;
   int y = get_axis()-1;
   char letter = alphabet[x][y];
   add_char_to_code(letter);

   if (password_ok(code_queue.code) == true) {
      set_status_ok();
   } else {
      set_status_not_ok();
   }
}
