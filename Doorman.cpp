//Doorman.cpp - Doorman Class Method Definitions
//Part of ArduinoDoorman, an Arduino-Based Access Control System
//Ethan Goff, January 2014



#include "Doorman.h"


//Static Class Members
SwitchController * Doorman::SwitchBank;
Keychain * Doorman::Keys;
Keypad * Doorman::InputKeypad;
uint8_t Doorman::StateLED;
bool Doorman::PublicAccessModeOn;
bool Doorman::ProgramModeOn;
unsigned long Doorman::StateLEDReferenceTime;



//Constructor
Doorman::Doorman(Keypad * inputKeypad)
{
	//Because the Keypad class cannot be lazy-instantiated, we pass it from the main
	//	sketchinto the static instance of the Doorman class
	InputKeypad = inputKeypad;

	//Define the other static members. The initial indicator LED, as well as both
	//	mode booleans, are arbitrary - they are checked within the first second of
	//	operation regardless (and at a defined interval throughout the system's uptime)
	Keys = &Keychain();
	SwitchBank = &SwitchController();
	StateLED = GREEN_LED_OUTPUT_PIN;
	PublicAccessModeOn = false;
	ProgramModeOn = false;
	StateLEDReferenceTime = millis();			//Sample the initial reference time
}



//		Main system update method		//
//The system operates in 3 modes -
//	Normal Mode: checks entered keys against the keychain
//	Public Access Mode: opens the door at any keypress
//	Programming Mode: adds novel keys to the keychain or removes existing keys
//
//After the initial setup, the system simply cycles through this method repetatively.
//First, the system checks the two mode control switches to see if the mode should
//be changed before continuing. Then the system updates the mode indication LED if
//needed. Then, the system probes the Keypad, acting on a meaningful receipt as the
//current mode implies.
void Doorman::Update()
{
	ProbeLatchingSwitches();	//Check the mode control switches
	UpdateIndicatorLED();	//Update the mode LED

	//Probe the Keypad for a keypress.
	//currentInput holds the 'seed' keypress (the first entered by a user)
	//	or a c-string terminator to indacte nothing is received
	char currentInput = InputKeypad->getKey();

	//If the system is in public access mode and any key is pressed OR if the
	//	override switch is active, we open the door without further checks.
	if( (PublicAccessModeOn && currentInput != '\0') || RecievingOverrideRequest() )
		OpenDoor();

	//Otherwise, if the system received a meaningful keypress...
	else if(currentInput != '\0' && currentInput != '#' && currentInput != '*')
	{
		//...it attempts to get the other three digits of the code.
		int Attempt = getCode(3, int(currentInput-'0') );
        
		if(Attempt != TIMEDOUT)//If the getCode method didn't time-out
		{
			//If the system is in programming mode, it forwards the key to the 
			//	programming method.
			if(ProgramModeOn)
			{
				LED(FORCE_LEDS_OFF);	//Clear the LEDs first
				//The Programming Method returns an event token, which selects the
				//	appropriate LED indication
				LED(Keys->ProgramKey(Attempt));		
			}

			//Otherwise, it checks if the Key exists in Keychain
			else if( Keys->KeyExists(Attempt) )
				OpenDoor();
			
			//If the key isn't found in the keychain, indicate an incorrect attempt
			else
				LED(INCORRECT_GUESS);
		}
		else
			LED(TIMEDOUT);
	}

	//If none of the above occur, we do nothing, and the checks start anew
	else
		return;
}



//Updates the system's mode-indicator LED (if necessary)
void Doorman::UpdateIndicatorLED()
{
	//If we aren't in Public Access Mode, we toggle the current indicator LED 
	//	back and forth at a defined rate
	if(millis() - StateLEDReferenceTime > WAITING_ON_INPUT_LED_HOLD_TIME && !PublicAccessModeOn)
		LED(TOGGLE_STATE_LED_ON_OFF);
}



//Check the latching switches and reset the check timer
void Doorman::ProbeLatchingSwitches()
{
	PublicAccessModeOn = RecievingPublicAccessRequest();
	//Programming mode is meant to be exclusive (and superceded by) Public Access Mode
	if(!PublicAccessModeOn)
		ProgramModeOn = RecievingProgramRequest();
}



//Returns either an input code or a TIMEDOUT token
//  targetLength: digit count for the code to be read
//  timeout: time in seconds to allow for a code to be entered, refreshed after each keypress
int Doorman::getCode(const unsigned int& targetLength, unsigned int seed = 0)
{
	//inputAttempt holds the security code guess that is being entered by a user
	int inputAttempt = seed;

	//digitsEntered counts the number of input digits entered by the user
	int digitsEntered = 0;

	//referenceTime holds an initially arbitrary time, which is checked against
	//	the current time to check for the elapsed difference
	unsigned long referenceTime = millis();

	//Look for input until the requested number of digits have been entered 
	//	or the operation times out
	while(digitsEntered < targetLength && millis() - referenceTime < INPUT_TIMEOUT)
	{	
		//We update our indicator LED twice as fast to indicate key presses 
		//	are being received
		if(millis() - StateLEDReferenceTime > (WAITING_ON_INPUT_LED_HOLD_TIME)/2)
		{
			LED(TOGGLE_STATE_LED_ON_OFF);
		}        

		//Sample for a keypress
		char currentInput = InputKeypad->getKey();

		//If the user enters a valid numeric key
		if( currentInput != '#' && currentInput != '*' && currentInput != '\0')
		{
			//Append the inputed key to the input code
			inputAttempt*=10;
			inputAttempt+= int(currentInput-'0');

			digitsEntered++;	//increment the digits counter
			referenceTime = millis();	//reset the timeout
		}


	}
	//If the user failed to finish entering a code, return the suitable token 
	if(millis() - referenceTime >= INPUT_TIMEOUT)
		return TIMEDOUT;
	else //Otherwise, return the entered code
		return inputAttempt;
}



//Opens the door by activating the relay controlling power to the door lock
void Doorman::OpenDoor()
{
	//Set digital output to relay contol to LOW for an ammount of time, 
	//	unlatching the door lock
	digitalWrite(DOOR_RELAY_CONTROL_OUTPUT_PIN, LOW); 

	LED(DOOR_OPENED);	//Indicate a successful open via the LEDs

	//Re-Latch the door
	digitalWrite(DOOR_RELAY_CONTROL_OUTPUT_PIN, HIGH);
}



//Checks if the system is set to Programming Mode
bool Doorman::RecievingProgramRequest()
{
	//The Program mode is designed to be triggered by a latching switch -
	//  this method is written more generally (since debounce is still neccessary
	//  with a latching switch, this method should be called occassionally to check
	//  for changes once programming has begun.

	//Check once...
	if(SwitchBank->ProgramSwitchIsActive())
	{
		delay(SWITCH_DEBOUNCE_DELAY);
		//...then re-check after some de-bouncing/de-noising time
		if(SwitchBank->ProgramSwitchIsActive())
                {
			//If the mode is switched on, turn off the LEDs and switch to
			//	the yellow indicator LED to indicate the system has entered 
			//	programming mode
			if(!ProgramModeOn)
			{
				LED(FORCE_LEDS_OFF);
				StateLED = YELLOW_LED_OUTPUT_PIN;
			}
			return true;
		}
	}
	//If the mode is switched on, turn off the LEDs and switch to
	//	the green indicator LED to indicate the system has left 
	//	programming mode
	if(ProgramModeOn)
	{
		LED(FORCE_LEDS_OFF);
		StateLED = GREEN_LED_OUTPUT_PIN;
	}
	return false;
}



//Checks if the system is set to Public Access Mode
bool Doorman::RecievingPublicAccessRequest()
{
	//Similarly to the program mode, the public access mode is designed to be
	//  triggered by a latching switch. The two are meant to be mutually exclusive;
	//  However, public access mode takes precedence, creating a sink state
	//  until released.

	//Check once...
	if(SwitchBank->PublicAccessModeSwitchIsActive())
	{
		delay(SWITCH_DEBOUNCE_DELAY);
		//...then re-check after some de-bouncing/de-noising time
		if(SwitchBank->PublicAccessModeSwitchIsActive())
		{
			//Indicate that we have entered Public Access mode
			LED(PUBLIC_ACCESS_SWITCHED_ON);
			return true;
		}
	}
	return false;
}



//Checks if a user is requesting an override
bool Doorman::RecievingOverrideRequest()
{
	//Unlike the mode switches, the override request is designed 
	//to be triggered with a momentary switch.

	//Check once...
	if(SwitchBank->OverrideSwitchIsActive())
	{
		delay(SWITCH_DEBOUNCE_DELAY);
		//...then re-check after some de-bouncing/de-noising time
		if(SwitchBank->OverrideSwitchIsActive())
			return true;
	}
	return false;
}



//Handles requests to indicate events via the LEDs
//	eventToken: Constants corresponding to specific events
void Doorman::LED(const unsigned int & eventToken)
{
	switch (eventToken)
	{
		case INCORRECT_GUESS:
                        //Clear the LEDs first
			digitalWrite(GREEN_LED_OUTPUT_PIN, HIGH );                
			digitalWrite(YELLOW_LED_OUTPUT_PIN, HIGH );
			digitalWrite(RED_LED_OUTPUT_PIN, HIGH );
			//Blink the red LED once
			digitalWrite(RED_LED_OUTPUT_PIN, LOW);
			delay(WRONG_ATTEMPT_INDICATION_DURATION);
			digitalWrite(RED_LED_OUTPUT_PIN, HIGH);
			return;


		case TOGGLE_STATE_LED_ON_OFF:
			//Switch the state LED
			digitalWrite(StateLED, !digitalRead(StateLED) );
			StateLEDReferenceTime = millis();
			return;

		case FORCE_LEDS_OFF:
			digitalWrite(GREEN_LED_OUTPUT_PIN, HIGH );                
			digitalWrite(YELLOW_LED_OUTPUT_PIN, HIGH );
			digitalWrite(RED_LED_OUTPUT_PIN, HIGH );
			return;

		case PUBLIC_ACCESS_SWITCHED_ON:
			//Ensure the green LED is forced on
			digitalWrite(GREEN_LED_OUTPUT_PIN, LOW);
			return;


		case KEY_ADDED:		//Cascade the LEDs from red to yellow to green, 3 times
			for(int i=0; i<3 ; i++)
			{
				for(int LED = RED_LED_OUTPUT_PIN; LED >= GREEN_LED_OUTPUT_PIN; LED--)
				{
					digitalWrite(LED, LOW);
					delay(PROGRAMMING_CHANGE_LED_HOLD_TIME); 
					digitalWrite(LED, HIGH);
				}
			}
			return;


		case KEY_REMOVED:	//Cascade the LEDs from green to yellow to red, 3 times
			for(int i=0; i<3 ; i++)
			{
				for(int LED = GREEN_LED_OUTPUT_PIN; LED <= RED_LED_OUTPUT_PIN; LED++)
				{
					digitalWrite(LED, LOW);
					delay(PROGRAMMING_CHANGE_LED_HOLD_TIME); 
					digitalWrite(LED, HIGH);
				}
			}
			return;


		case DOOR_OPENED:
			//The system will blink all LEDs simultaneously when the door is opened
			digitalWrite(GREEN_LED_OUTPUT_PIN, HIGH );                
			digitalWrite(YELLOW_LED_OUTPUT_PIN, HIGH );
			digitalWrite(RED_LED_OUTPUT_PIN, HIGH );
			unsigned long referenceTime = millis();		//Sample the current time

			//Blink the LEDs for some duration
			while(millis() - referenceTime < (UNLOCK_TIMEOUT))
			{
				boolean LEDState = digitalRead(GREEN_LED_OUTPUT_PIN); 
				//Toggle all LEDs to the opposite state
				digitalWrite(GREEN_LED_OUTPUT_PIN, !LEDState );                
				digitalWrite(YELLOW_LED_OUTPUT_PIN, !LEDState );
				digitalWrite(RED_LED_OUTPUT_PIN, !LEDState );
				delay(WAITING_ON_INPUT_LED_HOLD_TIME);
			}
			digitalWrite(GREEN_LED_OUTPUT_PIN, HIGH );                
			digitalWrite(YELLOW_LED_OUTPUT_PIN, HIGH );
			digitalWrite(RED_LED_OUTPUT_PIN, HIGH );
			return;
		}
}


//Sets the flag telling the system to write the default keymap
void Doorman::CheckForKeychainReset()
{
	if(RecievingOverrideRequest())
	{
		bool NowPopulated = false;
		EEPROM_writeAnything(KEYCHAIN_EXISTS_IN_EEPROM_FLAG_ADDRESS, NowPopulated);
		Keys = &Keychain();
	}
}
