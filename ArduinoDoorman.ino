//ArduinoDoorman.ino - Main Arduino Sketch Source
//Part of ArduinoDoorman, an Arduino-Based Access Control System
//Ethan Goff, January 2014



//Use instructions and design notes can be found in this project's ReadMe file.

#include <EEPROM.h>
#include <Keypad.h>
#include "Doorman.h"
#include "Definitions.h"



////	Begin Setup		////

//	Initialize the keypad (which needs to be instantiated globablly and statically)

const byte ROWS = 4;
const byte COLS = 3;

//Define the Keymap
static char keymap[ROWS][COLS] = 
{
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { KEYPAD_INPUT_PIN_ROWA, KEYPAD_INPUT_PIN_ROWB, KEYPAD_INPUT_PIN_ROWC, KEYPAD_INPUT_PIN_ROWD };
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { KEYPAD_OUTPUT_PIN_COLUMNA, KEYPAD_OUTPUT_PIN_COLUMNB, KEYPAD_OUTPUT_PIN_COLUMNC }; 
Keypad InputKeypad = Keypad( makeKeymap(keymap), rowPins, colPins, ROWS, COLS );
static Doorman * Dalton = &Doorman(&InputKeypad);



void PrepLEDs()
{
	//Set up LED pins
	pinMode(GREEN_LED_OUTPUT_PIN, OUTPUT);
	pinMode(YELLOW_LED_OUTPUT_PIN, OUTPUT);
	pinMode(RED_LED_OUTPUT_PIN, OUTPUT);
	
	//Turn all LEDs off to start
	digitalWrite(GREEN_LED_OUTPUT_PIN, HIGH);
	digitalWrite(YELLOW_LED_OUTPUT_PIN, HIGH);
	digitalWrite(RED_LED_OUTPUT_PIN, HIGH);       
}



void PrepSwitches()
{
	//Set up and Pullup the switch inputs
	pinMode(OVERRIDE_INPUT_PIN, INPUT);
	digitalWrite(OVERRIDE_INPUT_PIN, HIGH);

	pinMode(PROGRAM_MODE_INPUT_PIN, INPUT);
	digitalWrite(PROGRAM_MODE_INPUT_PIN, HIGH);

	pinMode(PUBLIC_ACCESS_MODE_INPUT_PIN, INPUT);
	digitalWrite(PUBLIC_ACCESS_MODE_INPUT_PIN, HIGH);        
}



void PrepRelay()
{
	//Set up the Relay Control output
	pinMode(DOOR_RELAY_CONTROL_OUTPUT_PIN, OUTPUT);
	digitalWrite(DOOR_RELAY_CONTROL_OUTPUT_PIN, HIGH);
}



void setup()
{
  	PrepLEDs();
	PrepSwitches();
	PrepRelay();
	//The system allows for a forced-reset of the Keychain stored in the EEPROM.
	//	This is triggered by holding the Override Switch while the system is reset.
        //      The Doorman must be initialized fully before we check for a request to 
        //      reset the keychain
        Dalton->CheckForKeychainReset();
}

////		End Setup		////



//The main loop
void loop()
{
	//Ask our Doorman to update the current state of the system
	Dalton->Update();
}
