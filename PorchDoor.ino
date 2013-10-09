//PorchDoor.ino - Arduino-Based Door Access Controller Main Source
//Ethan Goff, October 2013

#include "AccessEEPROM.h"
#include <EEPROM.h>
#include "Time.h"
#include "MatrixKeypad.h"
#include "Definitions.h"
#include "SwitchController.h"

//Globals
int Keys[KEYMAP_KEY_COUNT];
bool PublicAccessMode = false;


//Begin Functions


//Opens the door by activating the relay controlling power to the door lock
void OpenDoor()
{
	//Set digital output to relay contol to High for an ammount of time, unlatching the door lock
	digitalWrite(DOOR_RELAY_CONTROL, HIGH);
	delay(UNLOCK_TIMEOUT *1000);
	digitalWrite(DOOR_RELAY_CONTROL, LOW);
}

//	Returns either a 4-digit input code or a RESET_LISTENING_TIMEOUT token
int getCode(MatrixKeypad * KeypadWatcher)
{
	int inputAttempt = INVALID_INPUT;
	time_t referenceTime = now();
	int digitsEntered = 0;
	//Look for input until 4 digits have been entered or the operation times out
	while(digitsEntered < 4 && now() - referenceTime < INPUT_TIMEOUT)
	{
		//Look for the next valid key - quit on timeout
		while(KeypadWatcher->getCurrentKeypress() == INVALID_INPUT && now() - referenceTime < INPUT_TIMEOUT)
		{
			KeypadWatcher->updateCurrentKey();
		}
		//If the user clears the code they were entering, return a token indicating so
		//	to the call source
		if(KeypadWatcher->getCurrentKeypress() == RESET_KEY_VALUE)
		{
			return RESET_LISTENING_TIMEOUT;
		}
		//If the user has input a numerical character, add it to the inputAttempt
		else if(KeypadWatcher->getCurrentKeypress() != INVALID_INPUT)
		{
			digitsEntered++;
			inputAttempt*=10;
			inputAttempt+=KeypadWatcher->getCurrentKeypress();
			referenceTime = now();
		}
	}

	return inputAttempt;
}

//Checks if a request for enterance is occuring. If so, tries to recieve a 4-digit keycode
void KeypadCheck(MatrixKeypad * KeypadWatcher)
{
	int InputTimeout = 0;
	time_t referenceTime;

	//Update wether
	KeypadWatcher->UpdateListeningState();

	
	//If the keypad is in listening mode, try to get an input code from the user
	if(KeypadWatcher->getCurrentState())
	{
		int inputAttempt = 0;
		//The user has 5 seconds past each entry request to enter a correct key
		while(now() - referenceTime < INPUT_TIMEOUT && !KeyExists(inputAttempt))
		{
			inputAttempt = getCode(KeypadWatcher);
			if(inputAttempt = RESET_LISTENING_TIMEOUT)
			{
				referenceTime = now();
			}
		}
		if(KeyExists(inputAttempt))
		{
			KeypadWatcher->setCurrentState(false);
			inputAttempt = 0;
			OpenDoor();
		}
	}
}

//Checks if an entry is found in the Keys array or if the public key was entered
//	while public access is enabled
bool KeyExists(int keyAttempt)
{
	if(PublicAccessMode && keyAttempt == PUBLIC_ACCESS_MODE_KEY)
		return true;
	else
	{
		for(int i=0; i<KEYMAP_KEY_COUNT; i++)
		{
			if(Keys[i] == keyAttempt)
				return true;
		}
		return false;
	}
}

//Writes the Keys Array to the EEPROM
void WriteNewKeymap()
{
	EEPROM_writeAnything(KEYMAP_START_ADDRESS, Keys);
}

//Stores a new key to the EEPROM or removes a key that already exists
void ProgramKey(int newKey)
{
	//Check if the key is already stored, removing it if it has
	for(int i=0; i<KEYMAP_KEY_COUNT; i++)
	{
		if(Keys[i] == newKey)
		{
			Keys[i] = DEFAULT_EMPTY_KEY;
			WriteNewKeymap();
			return;
		}
	}
	//Find the farthest left key position that has a DEFAULT_EMPTY_KEY
	for(int i=0; i<KEYMAP_KEY_COUNT; i++)
	{
		if(Keys[i] == DEFAULT_EMPTY_KEY)
		{
			Keys[i] = newKey;
			WriteNewKeymap();
			return;
		}
	}
}

void Program(MatrixKeypad * KeypadWatcher)
{
	time_t referenceTime = now();
	while(now() - referenceTime < PROGRAMING_TIMEOUT)
	{
		int newKey = getCode(KeypadWatcher);
		if(newKey != PUBLIC_ACCESS_MODE_KEY && newKey <= 9999 && newKey >= 0 )
			ProgramKey(newKey);	
	}
}

//Checks if any of the buttons on the control module are indicating an action
void SwitchCheck(SwitchController* SwitchBank)
{
	//Open the door if the Override is active
	if(SwitchBank->OverrideActive())
	{
		delay(MINUMUM_OVERRIDE_SWITCH_DURATION);
		if(SwitchBank->OverrideActive())
			OpenDoor();
	}

	//Switch into or out of public access mode if the toggle is active
	else if(SwitchBank->PublicModeSwitchActive())
	{
		delay(MINUMUM_PUBLICMODE_SWITCH_DURATION);
		if(SwitchBank->PublicModeSwitchActive())
			PublicAccessMode = !PublicAccessMode;
	}

	//Go into Programing Mode if the Program button is pressed for
	//	some minimum duration
	else if(SwitchBank->ProgramActive())
	{
		delay(MINUMUM_PROGRAM_SWITCH_DURATION);
		if(SwitchBank->ProgramActive())
		{
			static MatrixKeypad * KeypadWatcher;
			Program(KeypadWatcher);
		}
	}
}

//Load a stored keymap into the Keys array
void LoadKeymap()
{
	EEPROM_readAnything(KEYMAP_START_ADDRESS, Keys);
}

//Populate the keymap with 1 public access mode key and 3 keys that could never
//	be correctly entered
void PopulateKeymap()
{
	for(int i = 0; i < KEYMAP_KEY_COUNT; i++)
	{
		Keys[i] = DEFAULT_EMPTY_KEY;
	}
	WriteNewKeymap();
	bool NowPopulated = true;
	EEPROM_writeAnything(KEYMAP_POPULATED_FLAG_ADDRESS, NowPopulated);
}



//Setup Pins and a keymap for the door lock unit
void setup()
{
	//Set the Keypad Probe Pins
	pinMode(KEYPAD_PROBE_ROWA, OUTPUT);
	pinMode(KEYPAD_PROBE_ROWB, OUTPUT);
	pinMode(KEYPAD_PROBE_ROWC, OUTPUT);
	pinMode(KEYPAD_PROBE_ROWD, OUTPUT);
  
	//Set the Keypad Recieve
	pinMode(KEYPAD_RECIEVE_COLUMNA, INPUT);
	pinMode(KEYPAD_RECIEVE_COLUMNB, INPUT);
	pinMode(KEYPAD_RECIEVE_COLUMNC, INPUT);

	//Check whether the key map is available, load it if it is, populate it with a 
	//	dummy array (see PopulateKeymap) if it is not
	bool KeymapIsPopulated = false;
	EEPROM_readAnything(KEYMAP_POPULATED_FLAG_ADDRESS, KeymapIsPopulated);
	if(KeymapIsPopulated)
		LoadKeymap();
	else
		PopulateKeymap();
}

// Main Loop for the arduino - checks what's going on with the keypad,
//	then 
void loop()
{
	static MatrixKeypad * KeypadWatcher;
	static SwitchController * Switches;

	KeypadCheck(KeypadWatcher);
	SwitchCheck(Switches);
}
