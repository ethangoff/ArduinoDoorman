//Keychain.cpp - Arduino Doorman Key Storage and Access Class Method Definitions
//Part of ArduinoDoorman, an Arduino-Based Access Control System
//Ethan Goff, January 2014



#include "Keychain.h"



//Static Members
int Keychain::Keys[KEYCHAIN_KEY_COUNT];



Keychain::Keychain()
{
	PrepKeys();
}



void Keychain::PrepKeys()
{
  	//Check whether a keychain is available
	bool KeychainExists = false;
	EEPROM_readAnything(KEYCHAIN_EXISTS_IN_EEPROM_FLAG_ADDRESS, KeychainExists);

	if(KeychainExists)
		LoadKeychain();		//Load the Keychain from the EEPROM
	else
		PopulateKeychain();		//Populate a new Keychain and store it to the EEPROM
}



//Checks if an entry is found in the Keychain
bool Keychain::KeyExists(const int& lookup)
{
	//Iterate through the Keychain, looking for a match with the lookup key
	for(int i=0; i<KEYCHAIN_KEY_COUNT; i++)
        {
		if(Keys[i] == lookup)
			return true;
        }

	return false;
}

//Writes the Keys Array to the EEPROM
void Keychain::WriteNewKeychain()
{
	EEPROM_writeAnything(KEYCHAIN_EEPROM_START_ADDRESS, Keys);
}



//Stores a new key to the EEPROM or removes a key that already exists
unsigned int Keychain::ProgramKey(const int& incidentKey)
{
	//In the model of the program, the empty key is used as a never-allowed input
	//	(essentially a place holder for future keys)

	//IFF the incident key is found in the current keymap, the key is effectively removed
	//	and the keymap is re-written to the EEPROM
	for(int i=0; i<KEYCHAIN_KEY_COUNT; i++)
	{
		if(Keys[i] == incidentKey)
		{
			Keys[i] = DEFAULT_EMPTY_KEY;
			WriteNewKeychain();
			return KEY_REMOVED;
		}
	}

	//IFF the incident key does not exist in the keymap, the farthest left key position 
	//	that has an empty space for a new legitamate key. If there are no free spaces,
	//	the input is ignored.
	for(int i=0; i<KEYCHAIN_KEY_COUNT; i++)
	{
		if(Keys[i] == DEFAULT_EMPTY_KEY)
		{
			Keys[i] = incidentKey;
			WriteNewKeychain();
			return KEY_ADDED;
		}
	}
}



//Load a stored keymap into the Keys array
void Keychain::LoadKeychain()
{
	EEPROM_readAnything(KEYCHAIN_EEPROM_START_ADDRESS, Keys);
}



//Populate the keychain with keys that could never be properly entered
void Keychain::PopulateKeychain()
{
	//Iterate through the keychain, reseting each key
	for(int i = 0; i < KEYCHAIN_KEY_COUNT; i++)
		Keys[i] = DEFAULT_EMPTY_KEY;
	
	WriteNewKeychain();	//Re-write the Keychain to the EEPROM
	
	//Set the flag marking the keychain as available from the EEPROM
	bool NowPopulated = true;
	EEPROM_writeAnything(KEYCHAIN_EXISTS_IN_EEPROM_FLAG_ADDRESS, NowPopulated);
}
