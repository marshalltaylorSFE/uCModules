uCModules
========================================

A collection of useful code for developing applications for microcontrollers, especially for projects with panels.  The panel interfacing code is highly developed and object oriented, allowing the ability to create new panel object hardware interfaces and logic structures, and to supplement the library from within the user's project.

Repository Contents
-------------------

* **/examples** - Example sketches for the library (.ino).
* **/extras** - Diagrams + stalled development examples
* **/src** - Source files (.cpp, .h).
* **keywords.txt** - For Arduino IDE. 
* **library.properties** - For Arduino package manager. 
* **LICENSE.md**
* **README.md**

Examples
-------------------

* Panel Components:
  * **ButtonBlinker** - Demonstration of 1 button and 1 led, with state machine.
  * **Knob** - Demonstration of 1 Windowed10BitKnob with default settings.
  * **SixKnobs** - Knobs on A0 through A5. A1 and A2 control window size and samples averaged.
  * **PanelTemplate** - Does nothing, starting place for projects.
* Utilities:
  * **flagMessagingNested** - Shows how to use the flagMessaging to communicate between classes.  Grounding pin 7 flags an inner class with a MessagingFlag object, which then changes state and communicates back to the outer class through another MessagingFlag object.
  * **timerModule32Example** - Example XORing 3 timers on the LED pin using uS counts with 32 bit counters
  * **timerModule32Template** - Same as example sketch with only a single timer, debugTimer, at 1 second
  * **timerModuleExample** - Example XORing 3 timers on the LED pin using mS counts with 16 bit counters
  * **timerModuleTemplate** - Same as example sketch with only a single timer, debugTimer, at 1 second
* Full systems: (Beta only)
  * **LightMeter** - Extends PanelComponents and HardwareInterfaces, includes TeensyView, Encoder, and TSL2561.

Compatibility
--------------

Tested on Uno and Teensy 3.x

* Arduino version 1.8.1
* Teensyduino version 1.35

Examples designed for Uno.
 
Documentation
--------------

[PanelComponent information](https://github.com/marshalltaylorSFE/uCModules/blob/master/extras/ClassDiagrams.md)

Version History
---------------

* [v1.0](https://github.com/marshalltaylorSFE/uCModules/tree/V_1_0) - Initial release
* [v1.0.1](https://github.com/marshalltaylorSFE/uCModules/tree/V_1_0_1) - Converted #defines to constants
* v2.0.0 currently in beta

License Information
-------------------

This product is _**open source**_! 

Please review the LICENSE.md file for license information. 

Distributed as-is; no warranty is given.

