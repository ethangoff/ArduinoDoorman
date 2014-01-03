//DebugTools.h - Arduino Debug Tools
//Ethan Goff, October 2013


#include "Definitions.h"
#include "Arduino.h"

#pragma once

class DebugTools
{
public:
	void debugBlink(int count, int duration=1000)
        {
          for(int i=0;i<count;i++)
          {
           digitalWrite(DEBUG_A, HIGH);
           delay(duration);
           digitalWrite(DEBUG_A, LOW);           
           delay(duration);            
          }
         
        }
 
	void sendToSerial(char* message, int wait=1000)
        {
           Serial.println(message);
           delay(wait);         
        } 
};
