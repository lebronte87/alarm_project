#include <Keypad.h>
#include <Password.h>
 
String newPasswordString; //hold the new password
char newPassword[6]; //charater string of newPasswordString
 
//initialize password to 1234
//you can use password.set(newPassword) to overwrite it
Password password = Password("1234");
 
byte maxPasswordLength = 6; 
byte currentPasswordLength = 0;
const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns
 
//Define the keymap
char keys[ROWS][COLS] = {
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};
 
//// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = {A0, A1, A2, A3}; //connect to row pinouts
 
// Connect keypad COL0, COL1, COL2 and COL3 to these Arduino pins.
byte colPins[COLS] = {A4, A5, A6, A7}; //connect to column pinouts
 
// Create the Keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
 

int x = 0;
unsigned long time_now = 0;
int period = 1000;

int ON_OFF = 2;
int ON_OFF_STATE = 0;
int ENTRY_EXIT = 11;
int STATE_ENTRY_EXIT = 0;
int ZONE_1 = 12;
int STATE_ZONE_1 = 0;
int Triggered = 0;
int BUZZER_PIN = 4;


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ON_OFF, INPUT_PULLUP);
  pinMode(ENTRY_EXIT, INPUT_PULLUP);
  pinMode(ZONE_1, INPUT_PULLUP);
  Serial.begin(9600);
  my_key();
  
  while(!ON_OFF_STATE){
    my_key();
  }
  for(int x=5; x>0; x--)
    { Serial.print("Alarm will arm in ");
      Serial.print(x);
      Serial.println(" seconds. ");
      delay(1000);
    }
  Serial.println("ARMED");

}

// the loop function runs over and over again forever
void loop() {

  STATE_ZONE_1 = digitalRead(ZONE_1);
  STATE_ENTRY_EXIT = digitalRead(ENTRY_EXIT);
/*************************************************/
  if(STATE_ZONE_1){
    Triggered = 1;
  }

  if(Triggered){
    flash();
    my_key();
    if(!ON_OFF_STATE)
      Triggered = 0;
  }
/************************************************/
if(STATE_ENTRY_EXIT){
  for(int x=5; x>0; x--)
    { Serial.print("Disarm in ");
      Serial.print(x);
      Serial.println(" seconds. ");
      delay(1000);
    }

    Triggered = 1;
  }

  if(Triggered){
    flash();
    my_key();
    if(!ON_OFF_STATE)
      Triggered = 0;
  }

  
}

/*void flash(){
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);                      // wait for a second
}
*/
void my_key(){
  char key = keypad.getKey();
   if (key != NO_KEY){
      delay(60); 
      switch (key){
      case 'A': break; 
      case 'B': break; 
      case 'C': break; 
      case 'D': break; 
      case '#': checkPassword(); break;
      case '*': resetPassword(); break;
      default: processNumberKey(key);
      }
   }
}
 
void processNumberKey(char key) {
   Serial.print(key);
   currentPasswordLength++;
   password.append(key);
   if (currentPasswordLength == maxPasswordLength) {
      checkPassword();
   } 
}

void checkPassword() {
   if (password.evaluate()){
      Serial.println(" OK.");
      ON_OFF_STATE=ON_OFF_STATE^1; //x=!x
   } else {
      Serial.println(" Wrong passwowrd!");
   } 
   resetPassword();
}

void resetPassword() {
   password.reset(); 
   currentPasswordLength = 0; 
}
void flash(){
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(BUZZER_PIN, HIGH);
  My_Delay();    // wait for a second
  digitalWrite(13, LOW);
  digitalWrite(BUZZER_PIN, LOW);    // turn the LED off by making the voltage LOW
  My_Delay();                           // wait for a second
}


void My_Delay()  {
    time_now = millis();
    while(millis() < time_now + period)	{
      my_key();
    // wait appox. [period] ms
    }
}
