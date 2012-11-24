#include <stdio.h>
#include "Button.h"
#include "RPA.h"
#include "pitches.h"
#include <Servo.h> 

#define SERIAL_BAUDRATE 9600

// ****** Sounds for winning a round ******
// notes for the win melody (for winning lab song):
int melodyWin[] = {
  NOTE_G4, NOTE_G4, NOTE_B4, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_CS5, NOTE_D5, NOTE_G4, 
NOTE_G4, NOTE_B4, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_B4, NOTE_G4};
  
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurationsWin[] = {
  8, 4, 4, 2, 8, 8, 8, 8, 8, 2, 8, 4, 4, 2, 8, 8, 8, 8, 8, 2};
// end of winning sounds

// ****** Sounds for losing a round ******
// notes for the losing melody:
int melodyLose[] = {
NOTE_C8, NOTE_B7, NOTE_AS7, NOTE_A7, NOTE_C8, NOTE_GS7, NOTE_GS7, NOTE_G7, NOTE_FS7, NOTE_F7, NOTE_E7, NOTE_DS7, NOTE_D7, NOTE_CS7, NOTE_C7, NOTE_B6, NOTE_AS6, NOTE_A6, NOTE_GS6, NOTE_G6, NOTE_FS6, NOTE_F6, NOTE_E6, NOTE_DS6, NOTE_D6, NOTE_CS6, NOTE_C6, NOTE_B5, NOTE_AS5, NOTE_A5, NOTE_GS5, NOTE_G5, NOTE_FS5, NOTE_F5, NOTE_E5, NOTE_DS5, NOTE_D5, NOTE_CS5, NOTE_C5, NOTE_B4, NOTE_AS4, NOTE_A4, NOTE_GS4, NOTE_G4, NOTE_FS4, NOTE_F4, NOTE_E4, NOTE_DS4, NOTE_D4, NOTE_CS4, NOTE_C4, NOTE_B3, NOTE_AS3, NOTE_A3, 
NOTE_GS3, NOTE_G3, NOTE_FS3, NOTE_F3, NOTE_E3, NOTE_DS3   
};

// note durations: 16 = 32nd note, 8 = sixteenth note, 4 = quarter note, 2 = half note, 1 = whole note etc.:
int noteDurationsLose[] = {
  32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 4, 4, 1};

// end of losing sounds

// create servo object to control a servo 
Servo myservo;
int servoPos = 0;    // variable to store the servo position

// the pins that control the brightness of RED, GREEN, and BLUE LEDs respectively
#define R 9
#define G 10
#define B 11


// pins that receive input for ROCK, PAPER, and SCISSOR
#define PIN_ROCK A3
#define PIN_PAPER A4
#define PIN_SCISSORS A5

// Set up variable for light reading
int lightReadingROCK = 0;
int lightReadingPAPER = 0;
int lightReadingSCISSORS = 0;

// pin that receives input from the NEW GAME (READY) button
#define PIN_READY 3

// wait this many ms before considering that a button has been pressed again
#define BUTTON_DEBOUNCE_WAIT 50

// instantiate a RockPaperAwesome state machine
RPA rpa = RPA();

// Button wrappers; uses the Button library
//  -- see: https://github.com/virgildisgr4ce/Button
Button rockButton = Button(PIN_ROCK, BUTTON_PULLUP_INTERNAL, true, BUTTON_DEBOUNCE_WAIT);
Button paperButton = Button(PIN_PAPER, BUTTON_PULLUP_INTERNAL, true, BUTTON_DEBOUNCE_WAIT);
Button scissorsButton = Button(PIN_SCISSORS, BUTTON_PULLUP_INTERNAL, true, BUTTON_DEBOUNCE_WAIT);

Button readyButton = Button(PIN_READY, BUTTON_PULLUP_INTERNAL, true, BUTTON_DEBOUNCE_WAIT);


/*******************************************************************/
/** EVENT HANDLERS *************************************************/
/*******************************************************************/

// when you've requested a connection to the XMPP server
void on_connect() {
  //blink(G, 3, 1000);
}

// when you're connected to the XMPP chatroom
void on_connected() {
  //blink(G, 3, 1000);
}

// when you've signaled that we're ready to play a game
void on_ready() {
  //blink(G, 3, 1000);
  //setLED(G, 15);
  myservo.write(35);
}

// when the other player has signaled that they're ready to play a game
void on_remote_ready() {
  //blink(G, 3, 1000);
}

// when you have chosen a weapon
void on_you_choose(String weapon) {
  if (weapon == "Rock")
    setLED(R, 255);
  else if (weapon == "Paper")
      setLED(G, 255);
  else if (weapon == "Scissors")
      setLED(B, 255);
}

// when the other player has chosen a weapon
void on_they_choose(String weapon) {
  //blink(G, 3, 1000);
}

// when you've won
void on_you_win() {
  //blink(G, 5, 600);
  for (int thisNote = 0; thisNote < 20; thisNote++) {
    
    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDurationWin = 1000/noteDurationsWin[thisNote];
    tone(13, melodyWin[thisNote],noteDurationWin);
    
    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDurationWin * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(13);
  }
}

// when you've lost
void on_you_lose() {
  //blink(R, 5, 600);
  for (int thisNote = 0; thisNote < 60; thisNote++) {
  
    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDurationLose = 1000/noteDurationsLose[thisNote];
    tone(13, melodyLose[thisNote],noteDurationLose);
  
    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDurationLose * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(13);
  }
}

// when you've tied
void on_tie() {
  blink(B, 5, 600);
}

// when you've been disconnected
void on_disconnected() {
  // blink(B, 5, 600);
}

/*******************************************************************/
/** STATE HANDLERS *************************************************/
/*******************************************************************/

// when you're offline
void while_OFFLINE() {
  setLED(R, 15);
}

// while connecting to XMPP
void while_CONNECTING() {
  //setLED(R, 15);
}

// while online, waiting for a game to start
void while_ONLINE() {
  //setLED(G, 5);
  myservo.write(13);
}

// while waiting for you to say you're ready
void while_WAITING_FOR_YOUR_READY() {
  //setLED(R, 15);
}

// while waiting for the other player to say they're ready
void while_WAITING_FOR_THEIR_READY() {
  //setLED(R, 15);
  myservo.write(35);
}

// while waiting for both players to choose a weapon
void while_READY_TO_PLAY() {
  //setLED(R, 15);
  myservo.write(87);
}

// while waiting for you to choose a weapon
void while_WAITING_FOR_YOUR_CHOICE() {
  //setLED(R, 15);
  myservo.write(63);
}

// while waiting for the other player to choose a weapon
void while_WAITING_FOR_THEIR_CHOICE() {
  //setLED(R, 15);
  myservo.write(115);
}

// while waiting to find out who won or lost
void while_WAITING_FOR_RESULT() {
  //setLED(R, 15);
}

/*******************************************************************/
/** SETUP AND MAIN LOOP ********************************************/
/*******************************************************************/

void setup() {
  //checking light sensor
  pinMode(PIN_ROCK, INPUT);
  pinMode(PIN_PAPER, INPUT);
  pinMode(PIN_SCISSORS, INPUT);
  
  // set up the Arduino hardware
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  // digitalWrite(PIN_ROCK, LOW);
  // digitalWrite(PIN_PAPER, LOW);
  // digitalWrite(PIN_SCISSORS, LOW);
  // pinMode(PIN_ROCK, INPUT);
  // pinMode(PIN_PAPER, INPUT);
  // pinMode(PIN_SCISSORS, INPUT);
  setRGB(0,0,0);
  Serial.begin(SERIAL_BAUDRATE);

  // wire up your event handlers here (all optional)
  rpa.connect = *on_connect;
  rpa.connected = *on_connected;
  rpa.ready = *on_ready;
  rpa.remote_ready = *on_remote_ready;
  rpa.you_choose = *on_you_choose;
  rpa.they_choose = *on_they_choose;
  rpa.you_win = *on_you_win;
  rpa.you_lose = *on_you_lose;
  rpa.tie = *on_tie;
  rpa.disconnected = *on_disconnected;

  // wire up your state entry handlers here (all optional)
  rpa.OFFLINE = *while_OFFLINE;
  rpa.CONNECTING = *while_CONNECTING;
  rpa.ONLINE = *while_ONLINE;
  rpa.WAITING_FOR_YOUR_READY = *while_WAITING_FOR_YOUR_READY;
  rpa.WAITING_FOR_THEIR_READY = *while_WAITING_FOR_THEIR_READY;
  rpa.READY_TO_PLAY = *while_READY_TO_PLAY;
  rpa.WAITING_FOR_YOUR_CHOICE = *while_WAITING_FOR_YOUR_CHOICE;
  rpa.WAITING_FOR_THEIR_CHOICE = *while_WAITING_FOR_THEIR_CHOICE;
  rpa.WAITING_FOR_RESULT = *while_WAITING_FOR_RESULT;

  // tell Node that the Arduino is ready and wants it to connect to XMPP
  rpa.say_hello();
  myservo.attach(2);  // attaches the servo on pin 2 to the servo object 
  myservo.write(155);
}

void loop() {
  lightReading = analogRead(PIN_ROCK);
  
  if (readyButton.uniquePress())
    rpa.bow();
  
  if (lightReadingROCK < 100){
    rpa.choose_ROCK();
  }
  
  if (lightReadingPAPER < 100){
    rpa.choose_PAPER();
  }

  if (lightReadingSCISSORS < 100){
    rpa.choose_SCISSORS();
  }

  rpa.check_input_from_serial();
}

/*******************************************************************/
/** HELPER FUNCTIONS ***********************************************/
/*******************************************************************/

// does any necessary transformation from a LED value to the value that should be written to the pin (using analogWrite)
byte convertRGBval(byte val) {
  // due to the way my LEDs are wired, my pin values are inverted, so for me 0 == maximum, 255 == minimum
  // get rid of the "255-" for yours to revert to normal usage
  return val;
  //return val;
}

// immediaely set the given led to the given brightness
void setLED(byte pin, byte val) {
  analogWrite(pin, convertRGBval(val));
}

// immediatley set the RED, GREEN, and BLUE leds to the given brightness
void setRGB(byte r, byte g, byte b) {
  setLED(R, r); 
  setLED(G, g); 
  setLED(B, b);
}

void blink(byte pin, byte times, int delayTime) {
  for (int i = 0; i < times; i++) {
    setLED(pin, 255);
    delay(delayTime);
    setLED(pin, 0);
    delay(delayTime / 2);
  }
}





