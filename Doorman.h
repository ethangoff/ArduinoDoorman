//Doorman.h - Doorman Class Declaration
//Part of ArduinoDoorman, an Arduino-Based Access Control System
//Ethan Goff, January 2014



#pragma once

#include <Keypad.h>
#include "SwitchController.h"
#include "Keychain.h"
#include "Definitions.h"
#include "AccessEEPROM.h"


//The doorman class holds most of the logic and behavior for the system, and
//	encapsulates classes that interface with the keypad and the system's switches,
//	and the Keychain class, which holds and manages the array of keys for the
//	system
class Doorman
{
public:
	Doorman(Keypad * inputKeypad);
	void Update();
	void CheckForKeychainReset();

private:
	int getCode(const unsigned int& targetLength, unsigned int seed);
	void UpdateIndicatorLED();
	void OpenDoor();
	void ProbeLatchingSwitches();
	bool RecievingOverrideRequest();
	bool RecievingProgramRequest();
	bool RecievingPublicAccessRequest();
	void LED(const unsigned int & token);

	static Keypad * InputKeypad;
	static SwitchController * SwitchBank;
	static Keychain * Keys;	
	static uint8_t StateLED;
	static bool PublicAccessModeOn;
	static bool ProgramModeOn;
	static unsigned long LatchingSwitchesReferenceTime;
	static unsigned long StateLEDReferenceTime;
};
