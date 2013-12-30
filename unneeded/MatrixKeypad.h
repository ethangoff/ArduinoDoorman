//MatrixKeypad.h - 4x3 Keypad Interface Class Declaration
//Ethan Goff, October 2013

#pragma once

#include "C:\Program Files (x86)\Arduino\hardware\arduino\cores\arduino\arduino.h"

class MatrixKeypad
{
public:
    MatrixKeypad();
	//Public Access Members
	bool getCurrentState(); 
	int getCurrentKeypress();

	//Public Access Modifiers
	void setCurrentKeypress(int newKeypressValue);
	void setCurrentState(bool newListeningState);

	void UpdateListeningState();

	void updateCurrentKey();

	bool Listening;
  
	int CurrentKeypress;  
};
