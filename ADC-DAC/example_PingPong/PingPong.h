/*
 * 
 * 
 */



#ifndef PINGPONG_H
#define PINGPONG_H

#include <Arduino.h>

// Delay for the communication of the parameters
#define COMMUNICATION_DELAY 10

//// Parameters for the measurement
struct parameters4measurement {
  // default values
  uint16_t StartStop = 1;    // 0 ->start aquisition, running; 1-> finished to aquire, waiting for paramerters; 2-> stop teensy
  float ADC_frequency = 1000;  // Hz
  uint32_t ADC_nSamples = 1000; // number of samples to collect
  uint16_t ADC_average = 32;         // average in the ADC
  uint16_t ADC_resolution = 16;      // resolution of the ADC
  uint32_t DAC_delay = 10;          // us delay of the DAC
  uint16_t DAC_type = 0;            // kind of signal to output (not implemented yet)

} Parameters;

// Handle the parameters for the measurement
class obj4Parameters {
	// Every function checks for the value to apply and correct it 
	void checkStartStop(float);
	void checkADC_Resolution(float);
	void checkADC_Average(float); 
	void checkDAC_Type(float);
	
	public:
 
	// apply after checking and modify the value to Parameters
	void update_value(char, float);
	// print to Serial Parameters
	void print_values();
} handlePara;

// Check for StartStop flag
void obj4Parameters::checkStartStop(float x) { 
	if (x >= 2) {
		// Total shut down
		Parameters.StartStop = 2;
	}
	else if (x >= 1) {
		// Stop aquisition and wait for more Parameters
		Parameters.StartStop = 1;    
	}
	else {
		// Start aquisition
		Parameters.StartStop = 0; 
		print_values();
	}
}

// Check that ADC_resolution is 8, 12, or 16
void obj4Parameters::checkADC_Resolution(float x) {
	// ADC_resolution make sense between 8 and 16
	if (x >= 16) Parameters.ADC_resolution = 16;
	else if (x >= 12) Parameters.ADC_resolution = 12;
	else if (x >= 8)  Parameters.ADC_resolution = 8;
}

// Check that ADC_average is a power of 2 until 32 (2^5)
void obj4Parameters::checkADC_Average(float x) {
	// ADC_average has to be a power of 2 until 32
	if (x >= 32) Parameters.ADC_average = 32;
	else if (x >= 16) Parameters.ADC_average = 16;
	else if (x >= 8)  Parameters.ADC_average = 8;
	else if (x >= 4)  Parameters.ADC_average = 4;
	else if (x >= 2)  Parameters.ADC_average = 2;
	else Parameters.ADC_average = 1; 
}

// Check that DAC_type is OK
void obj4Parameters::checkDAC_Type(float x) {
	// DAC_type has to be 0<=x<=5
	if (x >= 5) Parameters.DAC_type = 5;
	else if (x >= 4)  Parameters.DAC_type = 4;
	else if (x >= 3)  Parameters.DAC_type = 3;
	else if (x >= 2)  Parameters.DAC_type = 2;
	else if (x >= 1)  Parameters.DAC_type = 1;
	else Parameters.DAC_type = 0; 
}

// Update the read value into Paramters
void obj4Parameters::update_value(char letter, float x) {
	// here it really change the Parameters for the measurement
	//Parameters.StartStop = 1;  // if you come here then you do not want to start the measurement
	switch (letter) {
		case 'S':   // make it ready to start and print the parameters
			checkStartStop(x);      
			break;
		case 'F':   // ADC sampling frequency
			Parameters.ADC_frequency = x;
			break;
		case 'N':   // number of samples to collect and stream
			Parameters.ADC_nSamples = (uint32_t)x;
			break;
		case 'R':   // ADC resolution
			checkADC_Resolution(x);
			break;
		case 'A':   // ADC average
			checkADC_Average(x);
			break;
		case 'D':   // DAC delay
			Parameters.DAC_delay = max((uint32_t)2, (uint32_t)x); // not less than 2 us
			break;
		case 'T':   // DAC type
			checkDAC_Type(x);
			break;
	}
}

// Give feedback on Parameters
void obj4Parameters::print_values()  {
	// print the parameters
	delay(COMMUNICATION_DELAY);
	Serial.print("S");
	Serial.print(Parameters.StartStop);
	Serial.print(" ");
	Serial.print("F");
	Serial.print(Parameters.ADC_frequency);
	Serial.print(" ");
	Serial.print("N");
	Serial.print(Parameters.ADC_nSamples);
	Serial.print(" ");
	Serial.print("R");
	Serial.print(Parameters.ADC_resolution);
	Serial.print(" ");
	Serial.print("A");
	Serial.print(Parameters.ADC_average);
	Serial.print(" ");
	Serial.print("D");
	Serial.print(Parameters.DAC_delay);
	Serial.print(" ");
	Serial.print("T");
	Serial.println(Parameters.DAC_type);
}

/*
 * Functions declaration
 */
 
// Get the parameters from the PC
void pingPongParameters() {
    while (Serial.available() && Parameters.StartStop != 0) {
        // first byte contains the parameter to change:
        int info = Serial.read();
        if (info == 13) break;  // read until the end of the line
        else if (info == ' ') continue; // there are white spaces between paramerters
        // then it looks for a number
        float valueFloat = 0;
        valueFloat = Serial.parseFloat();
        // update the parameters
        handlePara.update_value(info, valueFloat);  // here you can say to start        
        delay(COMMUNICATION_DELAY);      // wait to see if other parameters are coming
    }
    // empty buffer
    //while (Serial.available() > 0)  Serial.read();
}

// shut down teensy
void shutDown() {
    Serial.println("Shut down!");
    exit(0);
}

#endif
