# IC900_DUClone
Display clone to replace the LCD ICs/Glass of the original ICOM IC-900 controller

12/20/23<br>
Current state of SW: Initial IC-900 DVT compete.  Declared fit for progress into hardware development.  Design is in C (baremetal).

Current state of HW: In layout.  PCB should be in-hand by mid-January, 2024.  Design is in Circuit Studio.

Electronics: Development prototype uses a Tiva TM4C123G Launchpad and Newhaven Display (Inc.) NHD-240128WG-ATMI-VZ# 240x128 graphics LCD.  Switches and backlighting for nomenclature will follow a similar architecture as that of the original IC-900 controller. The final design will feature jumper-wire connections from the RDU Clone board mated to a custom display clone PCB.  The arrangement would theoretically support using an original RDU control board in place of the clone allowing the new DU to interface with the original.

Mechanical: 3D model in advanced stages, nearing prototype readiness. Still need to work out 3D print strategies to support illuminated nomenclature.  Design is in OpenSCAD.

The "Snapshots" folder contains some images of the current state of the development.
