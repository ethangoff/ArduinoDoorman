//PorchDoor.ino - Arduino-Based Door Access Controller Main Source
//Ethan Goff, October 2013

#include <EEPROM.h>
#include <Keypad.h>
#include "DebugTools.h"
#include "Doorman.h"
#include "Definitions.h"
#include "SwitchController.h"



//Initialize the keypad
const byte ROWS = 4; // Four rows
const byte COLS = 3; // Three columns

// Define the Keymap
static char keymap[ROWS][COLS] = 
{
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { KEYPAD_PROBE_ROWA, KEYPAD_PROBE_ROWB, KEYPAD_PROBE_ROWC, KEYPAD_PROBE_ROWD };
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { KEYPAD_RECIEVE_COLUMNA, KEYPAD_RECIEVE_COLUMNB, KEYPAD_RECIEVE_COLUMNC }; 
Keypad InputKeypad = Keypad( makeKeymap(keymap), rowPins, colPins, ROWS, COLS );
static Doorman * Dalton = &Doorman(&InputKeypad);


        
//Setup Pins
void setup()
{
        Serial.begin(9600);
	pinMode(DEBUG_A, OUTPUT);
	pinMode(DOOR_RELAY_CONTROL, OUTPUT);

        digitalWrite(DEBUG_A, LOW);
        
        digitalWrite(DOOR_RELAY_CONTROL, HIGH);

        Dalton->Keys->PrepKeys();
}

// Main Loop for the arduino
void loop()
{
  Dalton->KeypadCheck();
}
