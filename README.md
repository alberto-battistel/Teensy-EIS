# Teensy-EIS

At the moment this git is still a bit chaotic and thrown together, sorry.
The idea is to use the two ADCs of the Teensy 3.6 (pjrc.com) to aquire data and the DAC to create the perturbation (sinus/multisine). All somehow controlled or managed by a pc. I have a very prototype analog front-end for voltage shift (teensy works 0-3.3V both ADC and DAC) to connect to the real potentiostat. 

1- datalogger_2ch (ADC-DAC contains more update communication) is a prototype to use a teensy 3.6 to aquire data and stream to the computer. It is working well and it is well commented. It use DMA and PDB to synchronize and speed up aquisition. I will probably need to use normal interrupts later, but it is a good starting point. 

2- ADC-DAC/example_DAC contains an example on how to use the DAC from a series of data produced by matlab/octave

3- In ADC-DAC there is a sketch and a python script to run a simple experiment. DAC and ADC work together and stream the data to python which collect and plot. Start the sketch first and then the script. This works very fine. To check for second ADC. 

3- in justTest/ there are some python files to fetch the stream/data on the computer (I use an Ubuntu 16.04 at the moment)

4- justTest/readFromPython is a prototype communication between teensy and python 

5- the rest is a collection of random things

**30Aug2017 - Test commit by Chris
