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
        void ForceReset();
        void PrepKeys();
        bool KeyExists(int& lookup);
	void WriteNewKeychain();
	void ProgramKey(int& newKey);
	void PopulateKeychain();
        void LoadKeychain();

	//Globals
	static int Keys[KEYMAP_KEY_COUNT];
        int DefaultEmptyKey;
};
