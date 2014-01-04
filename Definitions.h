//Definitions.h - Arduino-Based Door Access Controller Pre-Processor Definitions
//Ethan Goff, October 2013

//Pins
	//Input Pins
	#define KEYPAD_PROBE_ROWA  30
	#define KEYPAD_PROBE_ROWB  31
	#define KEYPAD_PROBE_ROWC  32
	#define KEYPAD_PROBE_ROWD  33
	#define CONTROL_PANEL_OVERRIDE_IN  A0
	#define CONTROL_PANEL_PROGRAM_IN  A1
        #define PUBLIC_ACCESS_MODE_IN A2
        
        
        
	//Output Pins
	#define DOOR_RELAY_CONTROL  26
	#define KEYPAD_RECIEVE_COLUMNA  42
	#define KEYPAD_RECIEVE_COLUMNB  43
	#define KEYPAD_RECIEVE_COLUMNC  44
	#define CONTROL_PANEL_PROBE 25
        #define GREEN_LED 50
        #define YELLOW_LED 51
        #define RED_LED 52        
        
//Settings
	//Timeouts
	#define INPUT_TIMEOUT 5000
	#define UNLOCK_TIMEOUT 5000
        #define OPEN_DOOR_LED_BLINK_DURATION 500
	#define MINUMUM_OVERRIDE_SWITCH_DURATION 1000
	#define MINUMUM_PROGRAM_SWITCH_DURATION 20
        #define MINUMUM_PUBLIC_ACCESS_SWITCH_DURATION 20
        #define LATCHING_SWITCH_PROBE_PERIOD 1000
        #define WAITING_BLINK_PERIOD 1000
        #define WRONG_ATTEMPT_INDICATION_DURATION 1500

	//Memory Addressing
	#define KEYMAP_START_ADDRESS 1
	#define KEYMAP_POPULATED_FLAG_ADDRESS 0
	#define KEYMAP_KEY_COUNT 4
	#define KEYMAP_INCIDENT_KEY_LENGTH 4

	//Default Keys
	#define DEFAULT_KEY 1212
	#define DEFAULT_EMPTY_KEY -1

//Constants
#define INVALID_INPUT 10
#define RESET_KEY_VALUE 11
#define RESET_LISTENING_TIMEOUT -1
