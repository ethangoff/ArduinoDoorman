//Definitions.h - Global Constants / Pre-Processor Definitions
//Part of ArduinoDoorman, an Arduino-Based Access Control System
//Ethan Goff, January 2014



//Pins
	//Keypad Pins - roms/columns oriented left->right and up->down, respectively 
	#define KEYPAD_INPUT_PIN_ROWA  30	
	#define KEYPAD_INPUT_PIN_ROWB  31
	#define KEYPAD_INPUT_PIN_ROWC  32
	#define KEYPAD_INPUT_PIN_ROWD  33
	#define KEYPAD_OUTPUT_PIN_COLUMNA  42
	#define KEYPAD_OUTPUT_PIN_COLUMNB  43
	#define KEYPAD_OUTPUT_PIN_COLUMNC  44

	//Switch Input Pins - these *do not* need to be analog pins
	#define OVERRIDE_INPUT_PIN  A0
	#define PROGRAM_MODE_INPUT_PIN  A1
	#define PUBLIC_ACCESS_MODE_INPUT_PIN A2

	//LED Output Pins
	#define GREEN_LED_OUTPUT_PIN 50
	#define YELLOW_LED_OUTPUT_PIN 51
	#define RED_LED_OUTPUT_PIN 52        

	//Output Pins
	#define DOOR_RELAY_CONTROL_OUTPUT_PIN  26

        
//Settings
	//Timeouts / Defined Durations (ms)
	#define INPUT_TIMEOUT 5000
	#define UNLOCK_TIMEOUT 5000
	#define SWITCH_DEBOUNCE_DELAY 20
	#define WRONG_ATTEMPT_INDICATION_DURATION 1500

	//LED Hold Times (ms)
	#define WAITING_ON_INPUT_LED_HOLD_TIME 1000
	#define PROGRAMMING_CHANGE_LED_HOLD_TIME 300

	//EEPROM Addressing
	#define KEYCHAIN_EEPROM_START_ADDRESS 1
	#define KEYCHAIN_EXISTS_IN_EEPROM_FLAG_ADDRESS 0
	#define KEYCHAIN_KEY_COUNT 4
	#define KEYCHAIN_INCIDENT_KEY_LENGTH 4

	//Default Empty Key - Represents an un-enterable code must be out of 
	//	the possible range
	#define DEFAULT_EMPTY_KEY -1


//Constants / Tokens - Used primarily to indicate specific events 
//	(none should be equal to another)
	#define TIMEDOUT -1		//Must be negative
	#define INCORRECT_GUESS 1
	#define DOOR_OPENED 2
	#define KEY_ADDED 3
	#define KEY_REMOVED 4
	#define TOGGLE_STATE_LED_ON_OFF 5
	#define FORCE_LEDS_OFF 6
	#define PUBLIC_ACCESS_SWITCHED_ON 7