//Doorman.h - Arduino Doorman Main Controller
//Ethan Goff, October 2013

#pragma once

#include <Keypad.h>
#include "SwitchController.h"
#include "Keychain.h"
#include "Definitions.h"
#include "DebugTools.h"
#include "Time.h"

class Doorman
{
public:
    Doorman(Keypad * inputKeypad);
	//Public Access Members
        static Keypad * InputKeypad;
	static SwitchController * SwitchBank;
	static Keychain * Keys;
	int getCode(const int& targetLength, const int& timeout, int seed);

	void KeypadCheck();
	void SwitchCheck();

private:
	void Program();
	void OpenDoor();



	bool PublicAccessMode;


};
