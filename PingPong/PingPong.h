/*
 * 
 * 
 */



#ifndef PINGPONG_H
#define PINGPONG_H

#include <Arduino.h>

//// Delay for the communication of the parameters
//int communicationDelay = 10;      // delay for serial communication

#define COMMUNICATION_DELAY 10

// Parameters for the measurement
struct parameters4measurement {
	// default values
	uint16_t StartStop = 1;    // 0 ->start aquisition, running; 1-> finished to aquire, waiting for paramerters; 2-> stop teensy
	float ADC_frequency = 1000;  // Hz
	uint32_t ADC_nSamples = 1000; // number of samples to collect
	uint16_t ADC_average = 32;         // average in the ADC
	uint16_t ADC_resolution = 16;      // resolution of the ADC
	uint32_t DAC_delay = 10;          // us delay of the DAC
	uint16_t DAC_type = 0;            // kind of signal to output (not implemented yet)

} /* Parameters */;

// Handle the parameters for the measurement
class obj4ADC {
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


/*
 * Functions declaration
 */
 
// Get the parameters from the PC
void pingPongParameters();

// shut down teensy
void shutDown();

#endif
