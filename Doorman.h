//Doorman.h - Arduino Doorman Main Controller
//Ethan Goff, October 2013

#pragma once

#include <Keypad.h>
#include "SwitchController.h"
#include "Keychain.h"
#include "Definitions.h"
#include "Time.h"

class Doorman
{
public:
    Doorman(Keypad * inputKeypad);
	//Public Access Members
        static Keypad * InputKeypad;
	static SwitchController * SwitchBank;
	static Keychain * Keys;
        static uint8_t StateLED;
        
	int getCode(const int& targetLength, const int& timeout, int seed);
        boolean PublicAccessModeIsOn();
	
        void ProbeLatchingSwitches();
        void Update();

	bool RecievingOverrideRequest();
        bool RecievingProgramRequest();
        bool RecievingPublicAccessRequest();
        
private:
	void OpenDoor();
        static bool PublicAccessModeOn;
        static bool ProgramModeOn;
        static time_t LatchingSwitchesReferenceTime;
        static time_t StateLEDReferenceTime;


	bool PublicAccessMode;


};
