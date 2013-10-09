//SwitchController.h - Switch Controller Panel Interface Class Declarations
//Ethan Goff, October 2013

#include "C:\Program Files (x86)\Arduino\hardware\arduino\cores\arduino\arduino.h"
#include "SwitchController.h"
#include "Definitions.h"

bool SwitchController::OverrideActive()
{
	digitalWrite(CONTROL_PANEL_PROBE, HIGH);
	bool OverrideIsActive = digitalRead(CONTROL_PANEL_OVERRIDE_IN);
	digitalWrite(CONTROL_PANEL_PROBE, LOW);
	return OverrideIsActive;
}

bool SwitchController::ProgramActive()
{
	digitalWrite(CONTROL_PANEL_PROBE, HIGH);
	bool ProgramIsActive = digitalRead(CONTROL_PANEL_PROGRAM_IN);
	digitalWrite(CONTROL_PANEL_PROBE, LOW);
	return ProgramIsActive;
}

bool SwitchController::PublicModeSwitchActive()
{
	digitalWrite(CONTROL_PANEL_PROBE, HIGH);
	bool PublicIsActive = digitalRead(CONTROL_PANEL_PUBLICMODE_IN);
	digitalWrite(CONTROL_PANEL_PROBE, LOW);
	return PublicIsActive;
}
