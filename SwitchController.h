//SwitchController.h - Switch Controller Panel Interface Class Declarations
//Ethan Goff, October 2013

#pragma once

#include "Arduino.h"

class SwitchController
{
public:
	SwitchController();

	bool OverrideActive(); 

	bool ProgramActive();
 
	bool PublicModeSwitchActive();
};
