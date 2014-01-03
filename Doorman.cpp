#include "Doorman.h"



SwitchController* Doorman::SwitchBank;
Keychain * Doorman::Keys;
Keypad * Doorman::InputKeypad;
uint8_t Doorman::StateLED;
bool Doorman::PublicAccessModeOn;
bool Doorman::ProgramModeOn;
time_t Doorman::LatchingSwitchesReferenceTime;
time_t Doorman::StateLEDReferenceTime;

//	Constructor
Doorman::Doorman(Keypad * inputKeypad)
{
    //Keys->PopulateKeymap();
    InputKeypad = inputKeypad;
    StateLED = GREEN_LED;
    PublicAccessModeOn = false;
    LatchingSwitchesReferenceTime = now();
    StateLEDReferenceTime = now();
    
}

void Doorman::ProbeLatchingSwitches()
{
  PublicAccessModeOn = RecievingPublicAccessRequest();
  ProgramModeOn = RecievingProgramRequest();
  LatchingSwitchesReferenceTime = now();
}


void Doorman::Update()
{
   //Because testing the switches takes time (due to de-noising/debouncing),
   //  we only want to check them periodically.
   if(now() - LatchingSwitchesReferenceTime > LATCHING_SWITCH_PROBE_PERIOD)
   {
     ProbeLatchingSwitches();
   }

   //We update our indicator LED
   if(now() - StateLEDReferenceTime > WAITING_BLINK_PERIOD)
   {
     digitalWrite(StateLED, !digitalRead(StateLED) );
     StateLEDReferenceTime = now();
   }
    
   //If
   if(PublicAccessModeOn && InputKeypad->getKey() != '\0')
        OpenDoor();
   else 
   {
      //Check if a key was recieved
      char currentInput = InputKeypad->getKey();  //currentInput holds the 'seed' key - that is, the first key pressed by a user
      
      //If no numerical key is pressed, break out of the keypad check
      if(currentInput == '\0' || currentInput == '#' || currentInput == '*')
         return;
         
      //Otherwise, attempt to get the rest of the code, appending it to the first key (if successful) 
      else
      {
         int Attempt = getCode(3, INPUT_TIMEOUT, int(currentInput-'0') );
         if(Attempt != RESET_LISTENING_TIMEOUT)
         {
           //We either forward the key to the programming method or check it against the Keychain
           if(ProgramModeOn)
               Keys->ProgramKey(Attempt); 
           else if( Keys->KeyExists(Attempt) )
	      OpenDoor();
           else //If the system is not in programming mode, but the key entered isn't present, indicate so
           {
              digitalWrite(RED_LED, LOW);
              delay(500);
              digitalWrite(RED_LED, HIGH);              
           }
         }
       }       
   }
   
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
           //We update our indicator LED
           if(now() - StateLEDReferenceTime > WAITING_BLINK_PERIOD/2)
           {
             digitalWrite(StateLED, !digitalRead(StateLED) );
             StateLEDReferenceTime = now();
           }        

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

//Note to self - asynchronosity isn't really needed here...
//Opens the door by activating the relay controlling power to the door lock
void Doorman::OpenDoor()
{
  	time_t referenceTime = now();
        time_t elapsedTime = 0;
        time_t toggleReferenceTime = now();
        time_t timeSinceLastToggle = 0;


	//Set digital output to relay contol to LOW for an ammount of time, unlatching the door lock
	digitalWrite(DOOR_RELAY_CONTROL, LOW);
 
        //Anytime we open the door, we'll to blink all three LEDs simultaneously
        //  we'll start by forcing all three off
        digitalWrite(GREEN_LED, HIGH );
        digitalWrite(YELLOW_LED, HIGH );
        digitalWrite(RED_LED, HIGH );
        
        while(elapsedTime < (5)) //While the latch should be open...
        {
            Serial.print("Door has been open for");
            Serial.print(elapsedTime);
            Serial.println("");
            Serial.print("it is now");
            Serial.print(now());
            Serial.println("");
            //...we want to toggle the indicator LEDs at a specified rate.
            if( timeSinceLastToggle  > 1)
            {
                //Sample the current state of the LEDs
                boolean LEDState = digitalRead(GREEN_LED);
 
                //Toggle all LEDs to the opposite state
              	digitalWrite(GREEN_LED, !LEDState );                
              	digitalWrite(YELLOW_LED, !LEDState );
              	digitalWrite(RED_LED, !LEDState );
                
                //Reset the reference time for the LEDs
                toggleReferenceTime = now();
            }
            
            //Re-calculate the times elapsed
            elapsedTime = now() - referenceTime;
            timeSinceLastToggle = now() - toggleReferenceTime;
        }

        //  Finally, close the door and reset all LEDs to off
        digitalWrite(DOOR_RELAY_CONTROL, HIGH);
        digitalWrite(GREEN_LED, HIGH );
        digitalWrite(YELLOW_LED, HIGH );
        digitalWrite(RED_LED, HIGH );

}

//The override request is designed to be triggered with a momentary
//  switch.
bool Doorman::RecievingOverrideRequest()
{
  	//Open the door if the Override is active
	if(SwitchBank->OverrideActive())
	{
		delay(MINUMUM_OVERRIDE_SWITCH_DURATION);
		if(SwitchBank->OverrideActive())
			return true;
	}
        return false;
}

//The Program mode is designed to be triggered by a latching switch -
//  this method is written more generally (since debounce is still neccessary
//  with a latching switch, this method should be called occassionally to check
//  for changes once programming has begun.
bool Doorman::RecievingProgramRequest()
{
	//Go into Programing Mode if the Program button is pressed for
	//	some minimum duration
	if(SwitchBank->ProgramActive())
	{
		delay(MINUMUM_PROGRAM_SWITCH_DURATION);
		if(SwitchBank->ProgramActive())
                {
                        StateLED = YELLOW_LED;
			return true;
                }
	}
        StateLED = GREEN_LED;
        return false;
}


//Similarly to the program mode, the public access mode is designed to be
//  triggered by a latching switch. The two are meant to be mutually exclusive
//  however, public access mode takes precedence, creating a sink state
//  until released
bool Doorman::RecievingPublicAccessRequest()
{
	//Go into Programing Mode if the Program button is pressed for
	//	some minimum duration
	if(SwitchBank->PublicAccessModeIsOn())
	{
		delay(MINUMUM_PUBLIC_ACCESS_SWITCH_DURATION);
		if(SwitchBank->PublicAccessModeIsOn())
                {
                        digitalWrite(GREEN_LED, LOW);
                        return true;
                }
	}
        return false;
}
