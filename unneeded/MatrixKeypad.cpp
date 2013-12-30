//MatrixKeypad.cpp - 4x3 Keypad Interface Class Definitions
//Ethan Goff, October 2013

#pragma once

#include "MatrixKeypad.h"
#include "DebugTools.h"
#include "Definitions.h"
#include "C:\Program Files (x86)\Arduino\hardware\arduino\variants\mega\pins_arduino.h" 
#include "C:\Program Files (x86)\Arduino\hardware\arduino\cores\arduino\arduino.h"



MatrixKeypad::MatrixKeypad()
{
    Listening = false;
    CurrentKeypress = INVALID_INPUT;
}

//	Check for the enable key to be pressed, and set the listening
//	state of the keyboard appropriatley
void MatrixKeypad::UpdateListeningState()
{
        DebugTools Debugger;
	digitalWrite(KEYPAD_PROBE_ROWA, HIGH);
	digitalWrite(KEYPAD_PROBE_ROWB, HIGH);
	digitalWrite(KEYPAD_PROBE_ROWC, HIGH);
	digitalWrite(KEYPAD_PROBE_ROWD, LOW);

	if(digitalRead(KEYPAD_RECIEVE_COLUMNA) == HIGH && digitalRead(KEYPAD_RECIEVE_COLUMNB) == HIGH && digitalRead(KEYPAD_RECIEVE_COLUMNC) == LOW)
        {
		Listening = true;
        }
	else
	{
		Listening = false;
	}
}



//	Updates the Currently Stored Key to what is being pressed
void MatrixKeypad::updateCurrentKey()
{
        DebugTools Debugger;
	setCurrentKeypress(INVALID_INPUT);
	digitalWrite(KEYPAD_PROBE_ROWA, LOW);
	digitalWrite(KEYPAD_PROBE_ROWB, HIGH);
	digitalWrite(KEYPAD_PROBE_ROWC, HIGH);
	digitalWrite(KEYPAD_PROBE_ROWD, HIGH);

	if(digitalRead(KEYPAD_RECIEVE_COLUMNA) == LOW && digitalRead(KEYPAD_RECIEVE_COLUMNB) == HIGH && digitalRead(KEYPAD_RECIEVE_COLUMNC) == HIGH)
        {
		setCurrentKeypress(1);
                return;
        }
	else if(digitalRead(KEYPAD_RECIEVE_COLUMNA) == HIGH && digitalRead(KEYPAD_RECIEVE_COLUMNB) == LOW && digitalRead(KEYPAD_RECIEVE_COLUMNC) == HIGH)
        {
		setCurrentKeypress(2);
                return;
        }
        else if(digitalRead(KEYPAD_RECIEVE_COLUMNA) == HIGH && digitalRead(KEYPAD_RECIEVE_COLUMNB) == HIGH && digitalRead(KEYPAD_RECIEVE_COLUMNC) == LOW)
        {
		setCurrentKeypress(3);
                return;
        }

	digitalWrite(KEYPAD_PROBE_ROWA, HIGH);
	digitalWrite(KEYPAD_PROBE_ROWB, LOW);

	if(digitalRead(KEYPAD_RECIEVE_COLUMNA) == LOW && digitalRead(KEYPAD_RECIEVE_COLUMNB) == HIGH && digitalRead(KEYPAD_RECIEVE_COLUMNC) == HIGH)
        {
		setCurrentKeypress(4);
                return;
        }
	else if(digitalRead(KEYPAD_RECIEVE_COLUMNA) == HIGH && digitalRead(KEYPAD_RECIEVE_COLUMNB) == LOW && digitalRead(KEYPAD_RECIEVE_COLUMNC) == HIGH)
        {
		setCurrentKeypress(5);
                return;
        }
	else if(digitalRead(KEYPAD_RECIEVE_COLUMNA) == HIGH && digitalRead(KEYPAD_RECIEVE_COLUMNB) == HIGH && digitalRead(KEYPAD_RECIEVE_COLUMNC) == LOW)
        {
		setCurrentKeypress(6);
                return;
        }



	digitalWrite(KEYPAD_PROBE_ROWB, HIGH);
	digitalWrite(KEYPAD_PROBE_ROWC, LOW);

	if(digitalRead(KEYPAD_RECIEVE_COLUMNA) == LOW && digitalRead(KEYPAD_RECIEVE_COLUMNB) == HIGH && digitalRead(KEYPAD_RECIEVE_COLUMNC) == HIGH)
        {
		setCurrentKeypress(7);
                return;
        }
	else if(digitalRead(KEYPAD_RECIEVE_COLUMNA) == HIGH && digitalRead(KEYPAD_RECIEVE_COLUMNB) == LOW && digitalRead(KEYPAD_RECIEVE_COLUMNC) == HIGH)
        {
		setCurrentKeypress(8);
                return;
        }
	else if(digitalRead(KEYPAD_RECIEVE_COLUMNA) == HIGH && digitalRead(KEYPAD_RECIEVE_COLUMNB) == HIGH && digitalRead(KEYPAD_RECIEVE_COLUMNC) == LOW)
        {
		setCurrentKeypress(9);
                return;
        }
    
    
	digitalWrite(KEYPAD_PROBE_ROWC, HIGH);
	digitalWrite(KEYPAD_PROBE_ROWD, LOW);

	if(digitalRead(KEYPAD_RECIEVE_COLUMNA) == HIGH && digitalRead(KEYPAD_RECIEVE_COLUMNB) == LOW && digitalRead(KEYPAD_RECIEVE_COLUMNC) == HIGH)
        {
		setCurrentKeypress(9);
                return;
        }
        else
        {
		setCurrentKeypress(INVALID_INPUT);
                return;
        }  
}

//	Returns the current numerical value of the key being pressed or returns the NOT_ACTIVE 
//		token if called while the Keypad is not actively looking for keypresses and the
//		INVALID_INPUT token if the user enters anything other than a number
int MatrixKeypad::getCurrentKeypress()
{
  return CurrentKeypress;
}

//	Returns the active state of the keypad, in regards to listening for numberical input
bool MatrixKeypad::getCurrentState()
{
  return Listening;
}

//	Sets the currently stored keyvalue
void MatrixKeypad::setCurrentKeypress(int newKeypressValue)
{
        digitalWrite(KEYPAD_PROBE_ROWA, HIGH);
        digitalWrite(KEYPAD_PROBE_ROWB, HIGH);
        digitalWrite(KEYPAD_PROBE_ROWC, HIGH);
        digitalWrite(KEYPAD_PROBE_ROWD, HIGH);
	CurrentKeypress = newKeypressValue;
}

//	Sets the currently stored listening state
void MatrixKeypad::setCurrentState(bool newListeningState)
{
	Listening = newListeningState;
}
