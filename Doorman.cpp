#include "Doorman.h"



SwitchController* Doorman::SwitchBank;
//Keychain * Doorman::Keys;
Keypad * Doorman::InputKeypad;


//	Constructor
Doorman::Doorman(Keypad * inputKeypad)
{
    InputKeypad = inputKeypad;
}

//	Returns either an input code or a RESET_LISTENING_TIMEOUT token
//  targetLength: digit count for the code to be read
//  timeout: time in seconds to allow for a code to be entered, refreshed after each keypress
int Doorman::getCode(const int& targetLength, const int& timeout, int seed = 0)
{
	//inputAttempt holds the security code guess that is being entered by a user
	int inputAttempt = seed;

	//digitsEntered counts the number of input digits entered by the user
	int digitsEntered = 0;

	//referenceTime holds an initially arbitrary time, which is checked against
	//	the current time to check for the elapsed difference
	time_t referenceTime = now();

	//Look for input until the requested number of digits have been entered or the operation times out
	while(digitsEntered < targetLength && now() - referenceTime < timeout)
	{	
		//Sample for a keypress
		char currentInput = InputKeypad->getKey();

		//If the user enters a non-numeric key, or nothing, do nothing and rescan.
                //Otherwise, add the inputed key to the greater input code, increment the
                //digits counter and reset the timeout
		if( currentInput != '#' && currentInput != '*' && currentInput != '\0')
                {
			inputAttempt*=10;
			inputAttempt+= int(currentInput-'0');
			digitsEntered++;
			referenceTime = now();
       		}


	}
        if(now() - referenceTime >= timeout)
          return RESET_LISTENING_TIMEOUT;
        else
          return inputAttempt;
}




//Checks if a key is being pressed - if so, tries to recieve the rest of a 4-digit keycode to test against a stored list of passcodes
void Doorman::KeypadCheck()
{
    char currentInput = InputKeypad->getKey();  //currentInput holds the 'seed' key - that is, the first key pressed by a user

    //  Check if a key was recieved
    //If no key was recieved, 
    if(currentInput == '\0' || currentInput == '#' || currentInput == '*')
      return;  
    //Otherwise, attempt to get the rest of the code, appending it to the first key (if successful) 
    else
    {
      int Attempt = getCode(3, INPUT_TIMEOUT, int(currentInput-'0') );
      if(Attempt != RESET_LISTENING_TIMEOUT)
      {
        Serial.print("got the input... ");
        Serial.print(Attempt);
        Serial.println("");
        
        if( ((currentInput*1000)+Attempt) == 1111)
        {
          Serial.println("Correct! Opening the door...");
	  OpenDoor();
          delay(50);
        }	
      }
      else
          Serial.println("timed out...");
    }   
}




//Enter programming mode
void Doorman::Program()
{
	time_t referenceTime = now();
	while(now() - referenceTime < PROGRAMING_TIMEOUT)
	{
		int newKey = getCode(4, 5);
		//if(newKey <= 9999 && newKey >= 0 )
			//Keys->ProgramKey(newKey);	
	}
}

//Opens the door by activating the relay controlling power to the door lock
void Doorman::OpenDoor()
{
	//Set digital output to relay contol to High for an ammount of time, unlatching the door lock
	digitalWrite(DOOR_RELAY_CONTROL, LOW);
	delay(UNLOCK_TIMEOUT *1000);
        digitalWrite(DOOR_RELAY_CONTROL, HIGH);
}

//Checks if any of the buttons on the control module are indicating an action
void Doorman::SwitchCheck()
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
			Program();
		}
	}
}
