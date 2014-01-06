//SwitchController.h - Switches Interface Class Declaration
//Part of ArduinoDoorman, an Arduino-Based Access Control System
//Ethan Goff, January 2014



#pragma once

#include <Arduino.h>
#include "Definitions.h"



class SwitchController
{
public:
	SwitchController();
	bool OverrideSwitchIsActive(); 
	bool ProgramSwitchIsActive();
	bool PublicAccessModeSwitchIsActive();
 };
