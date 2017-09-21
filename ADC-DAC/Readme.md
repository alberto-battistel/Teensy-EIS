# Update 21 Sep 2017

## Multisine works
I was expecting it. It is annoying to know which frequencies are outputted given the settings of the DAC (delay to convert into frequency).

## Filter capacitor
It gives super strange phenomena. Where it was it was not actually filtering. It was completely distorting the highest frequencies (they were not even sinus anymore). It was just reducing the noise. 
I moved it on the input stage of the second INA of the DAC (INA for shift). Now it filters at around 200-500 Hz.

## Pulldown resitor
we need to add a pulldown resistor for the relay.

## The second program for Python (readFromPython_singleBlock)
It was design to invoke the console after every block of data. That is you give a set a parameters to feed Teensy and when acquistion finishes the console is invoked one more time. It does not really work...
Some things I still do not understand in Python.
