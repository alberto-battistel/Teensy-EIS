#Analog front-end
The analog front-end interface the Teensy to the instrument (potentiostat). It serves as conditioning of the signals. It mainly provides low impedance for the inputs, level shift (Teensy is 0-3.3V), and some degree of rejection for ground loops (still to understand a bit better). It also features an independent power stage (USB compatible) to operate with bipolar signals.

## Signal conditioning
At the moment the conditioning is build on with 4 **INAs 128** (Instrumentation amplifiers) from Texas Instruments. They are flexible building blocks featuring low noise, low distortion, easy level shift, and high impedance inputs (10 GOhm). Two INAs are used to buffer, level shift, and shrink the DAC (digital to analog converter) of the Teensy and the other two are for level shifting of the inputs of the two ADCs (analog to digital converter). 
Analog filtering can be easily attached to the INAs.
**4Input-1Output_board** contains the full design (schematics and printed circuit board layout with bill of components) to build up the signal conditioning part. It is a very flexible board. We also employ it for *additional reference electrode probes* in electrochemical cells or to *break groud loops*.

## Power
The power stage is based on a **DC-DC converter** xxx (*I do not remember the name*) from Murata. It takes 5 V as input and provides +/- 12 V (or +/- 15 V?). It may be connected to the Teensy and take power from the same USB cable, but we did not test this yet. The bipolar voltage is used to power the INAs. The positive rail is also used to hold a normally open *signal relay* (MT2 Relay from AXICOM). This relay is controlled by a digital pin of the Teensy, though a transistor (2N3904) and it is necessary to cut the DAC line from the Teensy toward the instrument so that unwanted signals cannot be injected (for example when you turn on the Teensy...).

## State of the material
At the moment the schematics and the PCB layout signal conditioning part is available in **4Input-1Output_board**, but we may come out with a more practical design later. For the power stage there is something in **Design_schematics**, but it is still in progress.
 

