//PorchDoor.ino - Arduino-Based Door Access Controller Main Source
//Ethan Goff, October 2013

#include <EEPROM.h>
#include <Keypad.h>
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


        
//Setup Pins and prepare the Key Map
void setup()
{
        Serial.begin(9600);
        
	pinMode(GREEN_LED, OUTPUT);
	pinMode(YELLOW_LED, OUTPUT);
	pinMode(RED_LED, OUTPUT);
	pinMode(DOOR_RELAY_CONTROL, OUTPUT);

        digitalWrite(GREEN_LED, HIGH);
        digitalWrite(YELLOW_LED, HIGH);
        digitalWrite(RED_LED, HIGH);
        
        pinMode(CONTROL_PANEL_OVERRIDE_IN, INPUT);
        digitalWrite(CONTROL_PANEL_OVERRIDE_IN, HIGH);
        
        pinMode(CONTROL_PANEL_PROGRAM_IN, INPUT);
        digitalWrite(CONTROL_PANEL_PROGRAM_IN, HIGH);

        pinMode(PUBLIC_ACCESS_MODE_IN, INPUT);
        digitalWrite(PUBLIC_ACCESS_MODE_IN, HIGH);        
        
        
        
        pinMode(CONTROL_PANEL_PROBE, OUTPUT);
        digitalWrite(CONTROL_PANEL_PROBE, HIGH);        
        
        digitalWrite(DOOR_RELAY_CONTROL, HIGH);

        Dalton->Keys->PrepKeys();
}

void loop()
{
  //Ask our Doorman to update the current state of the system
  Dalton->Update();
}
