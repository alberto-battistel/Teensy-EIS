
#ifndef DEFULT_PARAMETERS_H
#define  DEFULT_PARAMETERS_H

#include <Arduino.h>

// Parameters for the measurement
struct parameters4measurement {
  // default values
  uint16_t StartStop = 1;    // 0 ->start aquisition, running; 1-> finished to aquire, waiting for paramerters; 2-> stop teensy
  float ADC_frequency = 1000;  // Hz
  uint32_t ADC_nSamples = 400; // number of samples to collect
  uint16_t ADC_average = 2;         // average in the ADC
  uint16_t ADC_resolution = 16;      // resolution of the ADC
  uint32_t DAC_delay = 100;          // us delay of the DAC
  uint16_t DAC_type = 0;            // kind of signal to output (not implemented yet)
  bool ADC_enoughSamples[2] ={false, false};         // does it have enough samples (one per ADC)
} Parameters;

#endif
