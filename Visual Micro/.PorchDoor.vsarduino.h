#ifndef _VSARDUINO_H_
#define _VSARDUINO_H_
//Board = Arduino Mega 2560 or Mega ADK
#define __AVR_ATmega2560__
#define 
#define _VMDEBUG 1
#define ARDUINO 105
#define ARDUINO_MAIN
#define __AVR__
#define F_CPU 16000000L
#define __cplusplus
#define __inline__
#define __asm__(x)
#define __extension__
#define __ATTR_PURE__
#define __ATTR_CONST__
#define __inline__
#define __asm__ 
#define __volatile__

#define __builtin_va_list
#define __builtin_va_start
#define __builtin_va_end
#define __DOXYGEN__
#define __attribute__(x)
#define NOINLINE __attribute__((noinline))
#define prog_void
#define PGM_VOID_P int
            
typedef unsigned char byte;
extern "C" void __cxa_pure_virtual() {;}

void OpenDoor();
int getCode(MatrixKeypad * KeypadWatcher);
void KeypadCheck(MatrixKeypad * KeypadWatcher);
bool KeyExists(int keyAttempt);
void WriteNewKeymap();
void ProgramKey(int newKey);
void Program(MatrixKeypad * KeypadWatcher);
void SwitchCheck(SwitchController* SwitchBank);
void LoadKeymap();
void PopulateKeymap();
//
//

#include "C:\Program Files (x86)\Arduino\hardware\arduino\variants\mega\pins_arduino.h" 
#include "C:\Program Files (x86)\Arduino\hardware\arduino\cores\arduino\arduino.h"
#include "C:\Users\Ethan\Google Drive\Code\Arduino\PorchDoor\PorchDoor.ino"
#include "C:\Users\Ethan\Google Drive\Code\Arduino\PorchDoor\AccessEEPROM.cpp"
#include "C:\Users\Ethan\Google Drive\Code\Arduino\PorchDoor\AccessEEPROM.h"
#include "C:\Users\Ethan\Google Drive\Code\Arduino\PorchDoor\DateStrings.cpp"
#include "C:\Users\Ethan\Google Drive\Code\Arduino\PorchDoor\Definitions.h"
#include "C:\Users\Ethan\Google Drive\Code\Arduino\PorchDoor\MatrixKeypad.cpp"
#include "C:\Users\Ethan\Google Drive\Code\Arduino\PorchDoor\MatrixKeypad.h"
#include "C:\Users\Ethan\Google Drive\Code\Arduino\PorchDoor\SwitchController.cpp"
#include "C:\Users\Ethan\Google Drive\Code\Arduino\PorchDoor\SwitchController.h"
#include "C:\Users\Ethan\Google Drive\Code\Arduino\PorchDoor\Time.cpp"
#include "C:\Users\Ethan\Google Drive\Code\Arduino\PorchDoor\Time.h"
#endif
