#include "Doorman.h"



SwitchController* Doorman::SwitchBank;
Keychain * Doorman::Keys;
Keypad * Doorman::InputKeypad;
uint8_t Doorman::StateLED;
bool Doorman::PublicAccessModeOn;
bool Doorman::ProgramModeOn;

unsigned long Doorman::LatchingSwitchesReferenceTime;
unsigned long Doorman::StateLEDReferenceTime;

//	Constructor
Doorman::Doorman(Keypad * inputKeypad)
{
    //Keys->PopulateKeymap();
    InputKeypad = inputKeypad;
    StateLED = GREEN_LED;
    PublicAccessModeOn = false;
    ProgramModeOn = false;
    LatchingSwitchesReferenceTime = millis();
    StateLEDReferenceTime = millis();
    
}

void Doorman::ProbeLatchingSwitches()
{
  PublicAccessModeOn = RecievingPublicAccessRequest();
  ProgramModeOn = RecievingProgramRequest();
  LatchingSwitchesReferenceTime = millis();
}


void Doorman::Update()
{
   //Because testing the switches takes time (due to de-noising/debouncing),
   //  we only want to check them periodically.
   if(millis() - LatchingSwitchesReferenceTime > LATCHING_SWITCH_PROBE_PERIOD)
   {
     ProbeLatchingSwitches();
   }

   //We update our indicator LED if we're not in public access mode
   if(millis() - StateLEDReferenceTime > WAITING_BLINK_PERIOD && !PublicAccessModeOn)
   {
     digitalWrite(StateLED, !digitalRead(StateLED) );
     StateLEDReferenceTime = millis();
   }
    
   //Check if a key was recieved
   char currentInput = InputKeypad->getKey();  //currentInput holds the 'seed' key - that is, the first key pressed by a user

   if(PublicAccessModeOn && currentInput != '\0')
        OpenDoor();
   else if(currentInput == '\0' || currentInput == '#' || currentInput == '*')
        return;
   else
   {
         Serial.println("Got something...");
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
              delay(WRONG_ATTEMPT_INDICATION_DURATION);
              digitalWrite(RED_LED, HIGH);              
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
	unsigned long referenceTime = millis();

	//Look for input until the requested number of digits have been entered or the operation times out
	while(digitsEntered < targetLength && millis() - referenceTime < timeout)
	{	
           //We update our indicator LED
           if(millis() - StateLEDReferenceTime > WAITING_BLINK_PERIOD/2)
           {
             digitalWrite(StateLED, !digitalRead(StateLED) );
             StateLEDReferenceTime = millis();
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
			referenceTime = millis();
       		}


	}
        if(millis() - referenceTime >= timeout)
          return RESET_LISTENING_TIMEOUT;
        else
          return inputAttempt;
}

//Note to self - asynchronosity isn't really needed here...
//Opens the door by activating the relay controlling power to the door lock
void Doorman::OpenDoor()
{
        unsigned long referenceTime = millis();
	//Set digital output to relay contol to LOW for an ammount of time, unlatching the door lock
	digitalWrite(DOOR_RELAY_CONTROL, LOW);
 
        //Anytime we open the door, we'll to blink all three LEDs simultaneously
        //  we'll start by forcing all three off
        digitalWrite(GREEN_LED, HIGH );
        digitalWrite(YELLOW_LED, HIGH );
        digitalWrite(RED_LED, HIGH );
        
        while(millis() - referenceTime < (UNLOCK_TIMEOUT)) //While the latch should be open...
        {
            //...we want to toggle the indicator LEDs at a specified rate.
                //Sample the current state of the LEDs
                boolean LEDState = digitalRead(GREEN_LED);
 
                //Toggle all LEDs to the opposite state
              	digitalWrite(GREEN_LED, !LEDState );                
              	digitalWrite(YELLOW_LED, !LEDState );
              	digitalWrite(RED_LED, !LEDState );
                
                delay(WAITING_BLINK_PERIOD);
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
                        Serial.println("Programming Mode On");
                        if(!ProgramModeOn)
                          digitalWrite(StateLED, HIGH);
                        StateLED = YELLOW_LED;
			return true;
                }
	}
        else if(ProgramModeOn)
           digitalWrite(StateLED, HIGH);
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
                        Serial.println("Public Access Mode On");
                        digitalWrite(GREEN_LED, LOW);
                        return true;
                }
	}
        return false;
}
