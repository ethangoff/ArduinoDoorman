//Keychain.cpp - Arduino Doorman Key Storage and Access Definitions
//Ethan Goff, October 2013
#include "Keychain.h"

int Keychain::Keys[KEYMAP_KEY_COUNT];

Keychain::Keychain()
{}

void Keychain::PrepKeys()
{
  	 //Check whether the key map is available, load it if it is, populate it with a 
	//	dummy array (see PopulateKeymap) if it is not
	bool KeymapIsPopulated = false;
	EEPROM_readAnything(KEYMAP_POPULATED_FLAG_ADDRESS, KeymapIsPopulated);
	if(KeymapIsPopulated)
	{
                Serial.println("A keychain exists");
               	LoadKeychain();
        }
	else
        {
            Serial.println("No keychain Exists, writing Default...");
	    PopulateKeymap();
        }
}

//Checks if an entry is found in the Keys array or if the public key was entered
//	while public access is enabled
bool Keychain::KeyExists(int keyAttempt)
{
		for(int i=0; i<KEYMAP_KEY_COUNT; i++)
		{
                        Serial.println(Keys[i]);
			if(Keys[i] == keyAttempt)
				return true;
		}
		return false;
}

//Writes the Keys Array to the EEPROM
void Keychain::WriteNewKeychain()
{
	EEPROM_writeAnything(KEYMAP_START_ADDRESS, Keys);
}

//Stores a new key to the EEPROM or removes a key that already exists
void Keychain::ProgramKey(int incidentKey)
{
	//In the model of the program, the empty key is used as a never-allowed input
	//	(essentially a place holder for future keys)
	//IFF the incident key is found in the current keymap, the key is effectively removed
	//	and the keymap is re-written to the EEPROM
	for(int i=0; i<KEYMAP_KEY_COUNT; i++)
	{
		if(Keys[i] == incidentKey)
		{
			Keys[i] = DEFAULT_EMPTY_KEY;
			WriteNewKeychain();
			return;
		}
	}

	//IFF the incident key does not exist in the keymap, the farthest left key position that
	//	has an empty space for a new legitamate key
	for(int i=0; i<KEYMAP_KEY_COUNT; i++)
	{
		if(Keys[i] == DEFAULT_EMPTY_KEY)
		{
			Keys[i] = incidentKey;
			WriteNewKeychain();
			return;
		}
	}
}

//Load a stored keymap into the Keys array
void Keychain::LoadKeychain()
{
	EEPROM_readAnything(KEYMAP_START_ADDRESS, Keys);
}


void Keychain::ForceReset()
{
        bool NowPopulated = false;
	EEPROM_writeAnything(KEYMAP_POPULATED_FLAG_ADDRESS, NowPopulated); 
}


//Populate the keymap with 1 default key and 3 keys that could never
//	be correctly entered
void Keychain::PopulateKeymap()
{
        Serial.println("writing Keymap");
	for(int i = 0; i < KEYMAP_KEY_COUNT; i++)
	{
          delay(500);
          Keys[i] = DEFAULT_EMPTY_KEY;
	}
        Keys[0] = DEFAULT_KEY;
                
	WriteNewKeychain();
	bool NowPopulated = true;
	EEPROM_writeAnything(KEYMAP_POPULATED_FLAG_ADDRESS, NowPopulated);
}
