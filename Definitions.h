//Definitions.h - Arduino-Based Door Access Controller Pre-Processor Definitions
//Ethan Goff, October 2013

//Pins
	//Input Pins
	#define KEYPAD_PROBE_ROWA  30
	#define KEYPAD_PROBE_ROWB  31
	#define KEYPAD_PROBE_ROWC  32
	#define KEYPAD_PROBE_ROWD  33
	#define CONTROL_PANEL_OVERRIDE_IN  22
	#define CONTROL_PANEL_PUBLICMODE_IN  23
	#define CONTROL_PANEL_PROGRAM_IN  24


	//Output Pins
	#define DOOR_RELAY_CONTROL  26
	#define KEYPAD_RECIEVE_COLUMNA  42
	#define KEYPAD_RECIEVE_COLUMNB  43
	#define KEYPAD_RECIEVE_COLUMNC  44
	#define CONTROL_PANEL_PROBE 25
        #define DEBUG_A 51

//Settings
	//Timeouts
	#define INPUT_TIMEOUT 5
	#define UNLOCK_TIMEOUT 5
	#define PROGRAMING_TIMEOUT 30
	#define MINUMUM_OVERRIDE_SWITCH_DURATION 1
	#define MINUMUM_PROGRAM_SWITCH_DURATION 3
	#define MINUMUM_PUBLICMODE_SWITCH_DURATION 1

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
#define RESET_LISTENING_TIMEOUT 12
