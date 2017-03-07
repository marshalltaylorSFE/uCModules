uCModules
========================================

Repository Contents
-------------------

* **/examples** - Example sketches for the library (.ino). Run these from the Arduino IDE. 
* **/extras** - Additional documentation for the user. These files are ignored by the IDE.
* **/src** - Source files for the library (.cpp, .h).
* **keywords.txt** - Keywords from this library that will be highlighted in the Arduino IDE. 
* **library.properties** - General library properties for the Arduino package manager. 

Examples
-------------------

* **flagMessagingNested** - Shows how to use the flagMessaging to communicate between classes.  Grounding pin 7 flags an inner class with a MessagingFlag object, which then changes state and communicates back to the outer class through another MessagingFlag object.
* **timerModule32Example** - Example XORing 3 timers on the LED pin using uS counts with 32 bit counters
* **timerModule32Template** - Same as example sketch with only a single timer, debugTimer, at 1 second
* **timerModuleExample** - Example XORing 3 timers on the LED pin using mS counts with 16 bit counters
* **timerModuleTemplate** - Same as example sketch with only a single timer, debugTimer, at 1 second

Documentation
--------------

Tested on Uno and Teensy 3.x  Use examples for demonstration.

Version History
---------------

* [v1.0](https://github.com/marshalltaylorSFE/uCModules/tree/V_1_0) - Initial release
* [v1.0.1](https://github.com/marshalltaylorSFE/uCModules/tree/V_1_0_1) - Converted #defines to constants

License Information
-------------------

This product is _**open source**_! 

Please review the LICENSE.md file for license information. 

Distributed as-is; no warranty is given.

