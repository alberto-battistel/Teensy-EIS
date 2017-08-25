# Teensy-EIS

At the moment this git is still a bit chaotic and thrown together.
The idea is to use the two ADCs of the teensy to aquire data and the DAC to create the perturbation (sinus/multisine). All somehow controlled or managed by a pc. I have a prototype analog front-end for voltage shift (teensy works 0-3.3V both ADC and DAC) to connect to the real potentiostat. 

1- datalogger_2ch_v01.ino is a prototype to use a teensy 3.6 to aquire data and stream to the computer. It is working well and it is well commented. It use DMA and PDB to synchronize and speed up aquisition. I will need to use normal interrupts later, but it is a good starting point.
2- in justTest/ there are some python files to fetch the stream/data on the computer (I use an Ubuntu 16.04 at the moment)
3- justTest/pingPong_v02 is a sketch to try with pingPong.py to communicate between pc and teensy
