Panel System
========================================

Overview
-------------------

The panel system high level control for knobs, buttons, and LEDs.  It adds edge detection, debounce, blinking, and filtering
functions, and also provides a container for the components that is used to simplify the updating of the components.  The
system is extendable to allow new objects to be created without modifying the source.  It is easy to create hardware interfaces
for other platforms that use the same component logic.

Source Files
-------------------

* HardwareInterfaces.cpp/.h -- functions that control the hardware directly.
* Panel.cpp/.h -- Container for components.  Extend in project to add panel objects.
* PanelComponents.cpp/.h -- Logic and data manipulation features for panel objects.
* PanelDataObjects.cpp/.h -- Uniformly interfaced data objects with dynamic size.

Diagrams
-------------------

## HardwareInterfaces

<img src="https://raw.githubusercontent.com/marshalltaylorSFE/uCModules/master/extras/ClassDiagramsHardwareDescriptions.jpg">

*HardwareInterfaces diagram*

## Panel

<img src="https://raw.githubusercontent.com/marshalltaylorSFE/uCModules/master/extras/ClassDiagramsPanels.jpg">

*Panel diagram*

## PanelComponents

<img src="https://raw.githubusercontent.com/marshalltaylorSFE/uCModules/master/extras/ClassDiagramsPanelComponents.jpg">

*PanelComponents diagram*

## PanelDataObjects

<img src="https://raw.githubusercontent.com/marshalltaylorSFE/uCModules/master/extras/ClassDiagramsDataObjects.jpg">

*PanelDataObjects diagram*


