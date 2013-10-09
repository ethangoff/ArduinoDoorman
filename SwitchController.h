//SwitchController.h - Switch Controller Panel Interface Class Declarations
//Ethan Goff, October 2013

#include "C:\Program Files (x86)\Arduino\hardware\arduino\cores\arduino\arduino.h"

class SwitchController
{
public:
	SwitchController();

	


	bool OverrideActive(); 

	bool ProgramActive();
 
	bool PublicModeSwitchActive();
};
