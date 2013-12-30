//Keychain.h - Arduino Doorman Key Storage and Access Declarations
//Ethan Goff, October 2013

#pragma once

#include <Arduino.h>
#include "AccessEEPROM.h"
#include "Definitions.h"


using namespace std;



class Keychain
{
public:
    Keychain();
	bool KeyExists(int keyAttempt);
	void WriteNewKeymap();
	void ProgramKey(int newKey);
	void PopulateKeymap();
        void LoadKeymap();

	//Globals
	int Keys[KEYMAP_KEY_COUNT];
        int DefaultEmptyKey;
};
