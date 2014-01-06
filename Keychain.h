//Keychain.h - Arduino Doorman Key Storage and Access Class Declaration
//Part of ArduinoDoorman, an Arduino-Based Access Control System
//Ethan Goff, January 2014



#pragma once

#include "AccessEEPROM.h"
#include "Definitions.h"


//The Keychain class holds and manages the array of valid keys,
//	which consists of integers. The keys are loaded from and
//	written to the EEPROM when programming takes place.
class Keychain
{
public:
	Keychain();
	void PrepKeys();
	bool KeyExists(const int& lookup);
	unsigned int ProgramKey(const int& incidentKey);

private:
	void WriteNewKeychain();
	void PopulateKeychain();
	void LoadKeychain();

	static int Keys[KEYCHAIN_KEY_COUNT];
};
