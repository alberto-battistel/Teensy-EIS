#ifndef SETTINGPARAMETERS_H
#define SETTINGPARAMETERS_H

#include <Arduino.h>
#include <ADC.h>
#include <DMAChannel.h>
#include <array>

/*
 * Function declaration for streaming data and setting the ADC/DAC
 */


/* Calculate the best delay
	 *  it calculates the best delay (Delay_us) between iterations
	 *  to have the best number of samples to send (BEST_N_VAL)
	 *  according to the time it takes to send a value (TIME_1VAL) and the aquistion frequency (FREQUENCY)
	 *  TIME_1VAL is between 1 and 3 us
	 */
uint32_t calculateDelay_loop(float ADC_frequency){
	 #define BEST_N_VAL 100
	 #define TIME_1VAL 3e-6   // s
	 uint32_t Delay_ = max((int32_t)(BEST_N_VAL*(1-TIME_1VAL*Parameters.ADC_frequency)/Parameters.ADC_frequency*1e6), 0);    // us
	 return Delay_;
}

/*  Set PDB stuff
 * 
 */
 void setPDB(float ADC_frequency)  {
    // enable PDB
    // this is part is completely obscure to me...
    SIM_SCGC6 |= SIM_SCGC6_PDB;
    uint32_t mod = (F_BUS / Parameters.ADC_frequency);   // PDB frequency
    //Serial.println(F_BUS / Parameters.ADC_frequency);
    //Serial.println(mod);
    //assert(mod <= 0x10000);
    //Serial.println(PDB0_MOD);
    PDB0_MOD = (uint16_t)(mod-1);
    //PDB0_MOD = (uint32_t)(mod-1);
    //Serial.println(PDB0_MOD);
    PDB0_CH0C1 = PDB_CHnC1_TOS_1 | PDB_CHnC1_EN_1; // PDB triggers ADC0 SC1A
    PDB0_CH1C1 = PDB_CHnC1_TOS_1 | PDB_CHnC1_EN_1; // PDB triggers ADC1 SC1A
    PDB0_SC = ADC_PDB_CONFIG | PDB_SC_PRESCALER(0) | PDB_SC_MULT(0) | PDB_SC_LDOK;
}

/*  Start conversion for ADC DMA stuff
 * 
 */
void startConversion()  {
    PDB0_SC = ADC_PDB_CONFIG | PDB_SC_PRESCALER(0) | PDB_SC_MULT(0) | PDB_SC_SWTRIG; // start
}



/*  set ADC parameters
 *  average, resoltution
 */
 void setADC(int8_t ADC_average, int8_t ADC_resolution) {
      /* Settings for ADC
     *  AVERAGE 32 OK at 10 kHz
     *  AVERAGE max 8 at 50 kHz
     *  RESOLUTION 16 at 10 kHz gives a floor noise of almost 0.01 mV
     *  RESOLUTION 12 at 10 kHz gives a floor noise better than 0.1 mV
     *  RESOLUTION 8 at 250 kHz gives a floor noise of 0.1 mV
     */
    adc.setAveraging(ADC_average);
    adc.setResolution(ADC_resolution);

    adc.setConversionSpeed(ADC_CONVERSION_SPEED::VERY_HIGH_SPEED);
    adc.setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_HIGH_SPEED);

    adc.setAveraging(ADC_average, ADC_1); // set number of averages
    adc.setResolution(ADC_resolution, ADC_1); // set bits of resolution

    adc.setConversionSpeed(ADC_CONVERSION_SPEED::VERY_HIGH_SPEED, ADC_1); // change the conversion speed
    adc.setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_HIGH_SPEED, ADC_1); // change the sampling speed

    adc.adc0->analogRead(adc_pin0); // performs various ADC setup stuff
    adc.adc1->analogRead(adc_pin1); // performs various ADC setup stuff

    if(adc.adc0->fail_flag || adc.adc1->fail_flag) {
        Serial.printf("ADC error, ADC0: %x ADC1: %x\n", adc.adc0->fail_flag, adc.adc1->fail_flag);
    }

    adc.adc0->stopPDB();
    adc.adc1->stopPDB(); 
}

/* Stream data
 *  Transfer from ringbuffer (DMA) to stream buffer and stream
 *  return the number of data streamed
 *  ring buffer can wrap
 *  stream buffer is alway filled from the first position and does not wrap
 */
 uint16_t StreamData(uint32_t NumberOfSamples, uint8_t bufferIndex, volatile uint16_t* DMAbufferAddress, volatile size_t old_buffer_, volatile size_t buffer_) {
    // number of values to stream
    uint32_t nvalues = 0;
    // position in the stream buffer
    uint16_t posStreamBuffer = 0;

    /* first check that the last position and the new position in the DMA buffer are not the same
    * it happens when you start
    */
    if(old_buffer_ != buffer_) {
    /* CASE I
     *  ring buffer did not wrap
     *  no problem data are in the right consecutive order
     */
        if(old_buffer_ < buffer_) {
        // consecutive order
            for(size_t i = old_buffer_; i < buffer_; i++) {
                streamBuffer[bufferIndex][posStreamBuffer++] = *(DMAbufferAddress + i);
                nvalues++;
            }
    }
    /* CASE II
     *  ring buffer wrapped
     *  take data from old position until the end of the buffer and start over from the beginning
     */
        else {
            for(size_t i = old_buffer_; i < BUFFERSIZE; i++) {
                streamBuffer[bufferIndex][posStreamBuffer++] = *(DMAbufferAddress + i);
                nvalues++;
            }
            for(size_t i = 0; i < buffer_; i++) {
                streamBuffer[bufferIndex][posStreamBuffer++] = *(DMAbufferAddress + i);
                nvalues++;
            }
        }
    }

    /* Stream data of the stream buffer
    *  this buffer was filled from the first position
    *  so stream only the right number of values
    *  to improve performances use "Serial" as USB type for teensy (very important)
    *    rise CPU clock
    *    write bytes (1.3 us per data)
    */
    uint16_t last2Stream = posStreamBuffer;
    if(NumberOfSamples+nvalues >= Parameters.ADC_nSamples)  {
  // it is reaching the right number of samples to stream
    uint32_t samplesTooMuch = NumberOfSamples + nvalues - Parameters.ADC_nSamples;
    last2Stream = posStreamBuffer - (uint16_t)samplesTooMuch;
    stop4Samples[bufferIndex] = true;
    streamData = true;
    }
  
  if (stop4Samples[0] && stop4Samples[1]) {
    // ready to receive new parameters
    Parameters.StartStop = 1;
    }
  
    // Advice how many data are going to be streamed
    //Serial.println(Iterations);   // count iterations
    Serial.println(last2Stream);  // use s.readline() to receive

    // Stream bytes
    for(uint16_t i = 0; i < last2Stream; i++) {
  // stream
    Serial.write(lowByte(streamBuffer[bufferIndex][i]));
    Serial.write(highByte(streamBuffer[bufferIndex][i]));
    /* use
    * data = s.read(x) where x is the number of bytes (16 bits = 2 bytes)
    * np.fromstring(data,dtype = np.uint16)
    * to get the data in python
    */
    }
    return last2Stream;   // number of data streamed
}

#ifdef DEBUG
void printAllInfo()  {
  /* Print details for timing and streaming
   *  set "debug_= true;"
   *  it gives the
   *    number of values streamed:        nvalues
   *    the time it took to stream:       time
   *    delay added in the loop:          Delay_us
   *    the positions in the ring buffer: bufferold_buffer_idx0_idx0
   *    the average time per data point:  us4value
   *    number of samples collected:      nSamples
   *  it is faster to send only 10-50 data values than more than 100 by a factor of 2-3
   */
      
    Serial.println();
    Serial.print("Number of values: ");
    Serial.print(counterSamplesADC0);
    Serial.print(", ");
    Serial.println(counterSamplesADC1);

    time_us = micros()-time_us;
    Serial.print("time: ");
    Serial.print(time_us);
    Serial.println(" us");

    Serial.print("delay time in the loop: ");
    Serial.print(Delay_loop);
    Serial.println(" us");

    Serial.print("Current Buffer index DMA is writing to: ");
    Serial.print(buffer_idx0);
    Serial.print(", ");
    Serial.println(buffer_idx0);

    Serial.print("Old Buffer index DMA was: ");
    Serial.print(old_buffer_idx0);
    Serial.print(", ");
    Serial.println(old_buffer_idx1);

    float us4value = (float)time_us/(float)counterSamplesADC0;
    Serial.print("us for value: ");
    Serial.println(us4value);

    Serial.print("number of samples: ");
    Serial.print(counterSamplesADC0);
    Serial.print(", ");
    Serial.println(counterSamplesADC1);
    Serial.println();
    
}
#endif


#endif
