# IC900_DUClone
Display clone to replace the LCD ICs/Glass of the original ICOM IC-900 controller

01/05/24<br>
Current state of SW: Initial IC-900 DVT compete.  Declared fit for progress into hardware development.  Design is in C (baremetal).

Current state of HW: In FAB.  PCBs should be in-hand by mid-January, 2024.  Design is in Circuit Studio.  In total, there are 6 PCBs for the RDU clone and MFMIC option:<br>
- IC900-RDU (circa 2020): The original controller clone CCA, Tiva MCU
- IC900-DUC (new): The LCD controller and switch matrix CCA, Tiva MCU.  Also holds the NVRAM mod for the RDU and a power switch circuit
- IC900-DIA (new)L: A small pcb that connects the dial encoder and dial back-light LEDs to the DUC CCA (no MCU)
- IC90x_RCU-B2 (circa 2020): The MFMIC CCA for the A-unit up/down de-mux function (ATTINY-816)
- IC900_ACONN (new): Provides connectors for RDU and host PC.  The MFMIC CCA mates to this PCB also.  A single 4-pr cable (CAT5) hard-connects to the IC-900 A-Unit
- HM-133B (circa 2020): This is the wired-remote version of the original HM-133 design.  It only supports the ICOM radio format and provides a serial connection to the MFMIC/RDU to carry the button press messages back to the RDU controller

The "new" PCBs are contained here as complete data packages (source and FAB data).  The older PCBs are committed as FAB data only.

Electronics: Development prototype uses a Tiva TM4C123G Launchpad and Newhaven Display (Inc.) NHD-240128WG-ATMI-VZ# 240x128 graphics LCD.  Switches and backlighting for nomenclature follow a similar architecture as that of the original IC-900 controller. The final design features jumper-wire connections from the RDU Clone board mated to the DUC PCB.  The arrangement would theoretically support using an original IC900 RDU control board in place of the clone allowing the new LCD to interface with the original controller.

Mechanical: 3D model in advanced stages, nearing prototype readiness. Still need to work out 3D print strategies to support illuminated nomenclature.  Design is in OpenSCAD.  In-process STL files are now in repo.

The "Snapshots" folder contains some images of the current state of the development.

BOM cost: Current cost data is in the $300 per radio ball-park for components and PCBs (not including any costs associated with 3D printing).

02/05/24<br>
First article is in DVT!!! The main mechanical deviation is that of button nomenclature.  I still don't have a difinitive solution in mind.  Cheesy, label printed lettering is the order of the day.  All else is progressing well, aiming for a first-to-field prototype by the end of the month.
