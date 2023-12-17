# IC900_DUClone
Display clone to replace the LCD ICs/Glass of the original ICOM IC-900 controller

Current state of SW: Initial IC-900 DVT compete.  Full DVT and IC-900 RDU Clone DVT has yet to complete.

Current state of HW: Initial Concept:

Electronics: Uses Tiva TM4C123G Launchpad and Newhaven Display (Int.) NHD-240128WG-ATMI-VZ# 240x128 graphics LCD.  Switches and backlighting for nomenclature will follow a similar architecture as that of the original IC-900 controller. The final design will feature a flex/ribbon cable connection that matches the original controller in terms of size and spacing and relative location (allows the new DU to interface with an original control board or my controller clone).  The Tiva LP will be replaced with a custom PCB that will also contain the switches, illumination LEDs, and dial encoder.

Mechanical: Need to work out 3D print strategies to support illuminated nomenclature, then translate that into a physical button layout and finally a PCB that incorporates the LCD bridge, switches, and illumination & status LEDs
