//SwitchController.cpp - Switches Interface Class Method Definitions
//Part of ArduinoDoorman, an Arduino-Based Access Control System
//Ethan Goff, January 2014



#include "SwitchController.h"


SwitchController::SwitchController(){}



bool SwitchController::OverrideSwitchIsActive()
{
	return !digitalRead(OVERRIDE_INPUT_PIN);
}



bool SwitchController::ProgramSwitchIsActive()
{
	return !digitalRead(PROGRAM_MODE_INPUT_PIN);
}



bool SwitchController::PublicAccessModeSwitchIsActive()
{
	return !digitalRead(PUBLIC_ACCESS_MODE_INPUT_PIN);
}
