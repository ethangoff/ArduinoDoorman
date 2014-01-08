Arduino Doorman, Last Updated January, 2014
Written by Ethan Goff

Uses the Keypad Library (Mark Stanley and Alexander Brevig) and the EEPROM access code described at http://playground.arduino.cc/Code/EEPROMWriteAnything



//Purpose
The Arduino Doorman is yet another arduino-based access control system which triggers opening a solenoid based dead-bolt lock. It stores integer keys of definable length and magnitude in its EEPROM, which are used to grant or deny entry, and which can be programmed (added or removed) easily. The system provides "public-access mode" functionality, used to allow free entry when desired, and allows for an override switch that can open the door using a momentary switch. It is written to be largely modular, allowing for the possibility of being used as part of a larger, or vastly different, system. 


//Theory/Considerations
The physical system the code is designed to control must consider a couple of conditions to be acceptably secure and feasible - first, only the keypad module must be accesible to the outside user, while the mode constrol and actual arduino must be inside the secured area. Second, the defined lengths of keys should not be shared and should be as long as is tolerable.


//Psuedo Parts List
Matrix Keypad, any size
2x SPST latching switches
SPST momentary switch
3x different color LEDs
TTL-level Relay
Relay-drivable solenoid lock


//Wiring
The keypad should be wired as is explained in the arduino matrix keypad tutorial. The switches should be wired to ground and to the arduino pins (they are driven by input pins in the internal pullup mode). The LEDs' anodes should be pulled-down to voltage and their cathodes connected to their control pins.


//Use
The system has 3 modes - normal access mode, public access mode, and programming mode:

In normal access mode, the system continually listens for keys to be entered. The green LED, blinks to indicate the mode is active, and blinks twice as fast as a key is being entered. All three LEDs blink in tandem to indicate a successful guess (when the door is opened) and the red LED blinks once to indicate an incorrect guess. 

In public access mode, the system allows entry when any numeric key is pressed. The green LED is always lit in this mode. The LEDs indicate an opening of the door in the same way as in normal access mode.

In programming mode, the system listens for incoming attempts, and compares them to the existing keys. It either adds novel keys or removes keys that already exist. The yellow LED blinks to indicate the system is in programming mode, blinking twice when a code is being entered. When a new key is successfully added, the LEDs cascade from red to green, while the opposite cascade (green->yellow->red) occurs.

To reset the Keychain (the list of stored keys), hold the override switch while resetting the arduino.