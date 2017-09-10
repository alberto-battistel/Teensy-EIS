# Teensy-EIS

At the moment this git is still slightly chaotic, but it is getting better.
The idea is to use the two ADCs of the Teensy 3.6 (pjrc.com) to aquire data and the DAC to create the perturbation (sinus/multisine) to work as an impedance analyzer. All controlled by a PC using Python, which also make the analysis part. We have an analog front-end for voltage shift (Teensy works 0-3.3V both ADC and DAC) to connect to the real potentiostat. This is tested and works.

## 1 - ADC-DAC
ADC-DAC.ino and readFromPython.py work together. They are THE Teensy-EIS programs. CalculateParameters.m helps to make the parameters to feed in Python.

## 2 - Examples
In ADC-DAC there are examples of the part of the ADC-DAC.ino. DAC is for the DAC and PingPong manages the serial communication with the PC. They are more or less the most updated or the same as in ADC-DAC.ino.

## 3 - Regarding the DAC
The signal for the DAC is generated through a Matlab/GNU Octave/Scilab script. (Generate_DAC_vector). 

## 4 - Analog front-end
In analog front-end there is all about the core of the voltage shift. All the analog interface with the instrument is based on four INA128. They are very flexible pieces. There is no schematic for the power stage and activation of the signal line, but the elements are in the two PDF.

## 5 - Control flow
In Schematics there are the diagrams of the programs. At the moment only the part for the aquisition, which is the most complex. In summary, the Teensy can aquire infinite amount of data which are streamed in blocks to the PC. Python can handle easily several milions data points. 

## 6 - pointer?
pointers-reference-test contains a couple of examples about passing by reference or by pointer to a function. Not fundamental for the rest, just to remember.

------------------------------------
# Version list:

## Version 1.00
Fully operational tidy up version.

## Version 0.99
Fully operational prototype, both for the programing side and for the analog side. 

------------------------------------
# Bug List:
So we can keep track of what needs to be fixed

-To be filled (Priority: 1)

------------------------------------
# Future Feature List:

## Version 1.1?
what shall we implement next?
