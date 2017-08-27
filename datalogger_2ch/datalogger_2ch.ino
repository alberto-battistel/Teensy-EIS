#include <ADC.h>
#include <DMAChannel.h>
#include <array>

/* do you want to check timing?
 *  it print info about how many samples are collected per iteration 
 *  and timing...
 */
bool debug_ = false;

/* Stop after a number of samples
 *  allow to stop the stream after collecting maxSamples
 */
bool stopOnSamples = true;    // flag
uint32_t maxSamples = 500000;   // stop after
uint32_t nSamples = 0;        // counter

/* Settings for Ring buffer (DMA) and aquisition frequency */
// DMA buffer size
#define BUFFERSIZE 16384

// stream buffer size
uint16_t streamBuffer[BUFFERSIZE];

// Frequency of acquistion (max 250 kHz with 8 bit resolution)
#define FREQUENCY 220000   // Hz

/* Settings for ADC
 *  AVERAGE 32 OK at 10 kHz
 *  AVERAGE max 8 at 50 kHz
 *  RESOLUTION 16 at 10 kHz gives a floor noise of almost 0.01 mV
 *  RESOLUTION 12 at 10 kHz gives a floor noise better than 0.1 mV
 *  RESOLUTION 8 at 250 kHz gives a floor noise of 0.1 mV
 */
#define AVERAGE 1    // max 8 at 50 kHz
#define RESOLUTION 8 // bits

/* flags and counter */
// command to stop streaming
boolean streamData = true;

// track in print time for serial print
unsigned long time = 0;

// store old index of buffer
volatile size_t old_buffer_idx0 = 0;

// count iterations
int16_t Iterations = 0;

// flag to know that the loop just started
bool JustStart = true;

/* Calculate the best delay 
 *  it calculates the best delay (Delay_us) between iterations 
 *  to have the best number of samples to send (BEST_N_VAL)
 *  according to the time it takes to send a value (TIME_1VAL) and the aquistion frequency (FREQUENCY)
 *  TIME_1VAL is between 2 and 5 us
 */
 #define BEST_N_VAL 50
 #define TIME_1VAL 5e-6   // s
 int16_t Delay_us = max((int16_t)(BEST_N_VAL*(1-TIME_1VAL*FREQUENCY)/FREQUENCY*1e6), 0);    // us


/* 
 * I did not touch much from here on 
 */

// connect out_pin to adc_pin0 and adc_pin1, PWM output on out_pin will be measured
// via adc_pin0 and adc_pin1.

const uint8_t adc_pin0 = A9;  // digital pin 23, on ADC0
const uint8_t adc_pin1 = A19; // digital pin 31, on ADC1
const uint8_t out_pin = 2;

ADC adc;
DMAChannel dma0;
DMAChannel dma1;

struct CapturePair {
    uint16_t v_adc0;
    uint16_t v_adc1;
};
std::array<volatile CapturePair, BUFFERSIZE> buffer;
volatile size_t write_pos = 0;

void setup() {
    pinMode(adc_pin0, INPUT);
    pinMode(adc_pin1, INPUT);
    Serial.begin(9600);
    delay(2000);
    //Serial.println("Starting");
    
    constexpr uint32_t pdb_trigger_frequency = FREQUENCY;
    adc.setAveraging(AVERAGE);
    adc.setResolution(RESOLUTION);

    adc.setConversionSpeed(ADC_CONVERSION_SPEED::VERY_HIGH_SPEED);
    adc.setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_HIGH_SPEED);

    adc.setAveraging(AVERAGE, ADC_1); // set number of averages
    adc.setResolution(RESOLUTION, ADC_1); // set bits of resolution
    
    adc.setConversionSpeed(ADC_CONVERSION_SPEED::VERY_HIGH_SPEED, ADC_1); // change the conversion speed
    adc.setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_HIGH_SPEED, ADC_1); // change the sampling speed
    
    adc.adc0->analogRead(adc_pin0); // performs various ADC setup stuff
    adc.adc1->analogRead(adc_pin1); // performs various ADC setup stuff

    if(adc.adc0->fail_flag || adc.adc1->fail_flag) {
        Serial.printf("ADC error, ADC0: %x ADC1: %x\n", adc.adc0->fail_flag, adc.adc1->fail_flag);
    }

    adc.adc0->stopPDB();
    adc.adc1->stopPDB();

    dma0.source((uint16_t&) ADC0_RA); // ADC result register
    //dma0.source(ADC0_RA); // ADC result register
    dma0.triggerAtHardwareEvent(DMAMUX_SOURCE_ADC0);
    dma0.destinationBuffer(&buffer[0].v_adc0, buffer.size() * sizeof(buffer[0]));
    dma0.TCD->DOFF = 4;
    dma0.TCD->CITER = buffer.size();
    dma0.TCD->BITER = buffer.size();
    dma0.enable();
    adc.enableDMA(ADC_0);

    dma1.source((uint16_t&) ADC1_RA); // ADC result register
    //dma1.source(ADC1_RA); // ADC result register
    dma1.triggerAtHardwareEvent(DMAMUX_SOURCE_ADC1);
    dma1.destinationBuffer(&buffer[0].v_adc1, buffer.size() * sizeof(buffer[0]));
    dma1.TCD->DOFF = 4;
    dma1.TCD->CITER = buffer.size();
    dma1.TCD->BITER = buffer.size();
    dma1.enable();
    adc.enableDMA(ADC_1);

    adc.adc0->setHardwareTrigger();
    adc.adc1->setHardwareTrigger();
    
    // enable PDB    
    SIM_SCGC6 |= SIM_SCGC6_PDB;
    // constexpr uint32_t pdb_trigger_frequency = 10000;
    constexpr uint32_t mod = (F_BUS / pdb_trigger_frequency);
    static_assert(mod <= 0x10000, "Prescaler required.");
    PDB0_MOD = (uint16_t)(mod-1);
    PDB0_CH0C1 = PDB_CHnC1_TOS_1 | PDB_CHnC1_EN_1; // PDB triggers ADC0 SC1A
    PDB0_CH1C1 = PDB_CHnC1_TOS_1 | PDB_CHnC1_EN_1; // PDB triggers ADC1 SC1A
    PDB0_SC = ADC_PDB_CONFIG | PDB_SC_PRESCALER(0) | PDB_SC_MULT(0) | PDB_SC_LDOK;

    // Kick off ADC conversion.
    PDB0_SC = ADC_PDB_CONFIG | PDB_SC_PRESCALER(0) | PDB_SC_MULT(0) | PDB_SC_SWTRIG; // start

}


void loop() {	
    // stop when it reaches the right number of samples
    if (stopOnSamples && (nSamples > maxSamples)) {
      streamData = false;
      stopOnSamples = !stopOnSamples;
    }
    
    noInterrupts();
    size_t buffer_idx0 = (((uint16_t*) dma0.destinationAddress()) - &buffer[0].v_adc0) / 2;
    // size_t buffer_idx1 = (((uint16_t*) dma1.destinationAddress()) - &buffer[0].v_adc1) / 2;
    interrupts();

    // number of values to stream
    size_t nvalues = 0;
    // position in the stream buffer
    size_t position = 0;
    // update timer
    time = micros();

    /* Transfer from ringbuffer (DMA) to stream buffer
     *  ring buffer can wrap
     *  stream buffer is alway filled from the first position and does not wrap
     *  you need to know how many data values go from ring buffer to stream buffer 
     */
     
    if(streamData) { // do it only if 
      /* first check that the last position and the new position in the DMA buffer are not the same
       * it happens when you start 
       */
      if(old_buffer_idx0 != buffer_idx0) {
        /* CASE I 
         *  ring buffer did not wrap
         *  no problem data are in the right consecutive order
         */
        if(old_buffer_idx0 < buffer_idx0) {
          // consecutive order
          for(size_t i = old_buffer_idx0; i < buffer_idx0; i++) {
              streamBuffer[position++] = buffer[i].v_adc0;
            nvalues++;    
          }
        }
        /* CASE II
         *  ring buffer wrapped
         *  take data from old position until the end of the buffer and start over from the beginning
         */
          else {
            for(size_t i = old_buffer_idx0; i < BUFFERSIZE; i++) {
              streamBuffer[position++] = buffer[i].v_adc0;
            nvalues++; 
            }
            for(size_t i = 0; i < buffer_idx0; i++) {
            nvalues++; 
            }
          }
      }

      /* Stream data of the stream buffer
       *  this buffer was filled from the first position
       *  so stream only the right number of values 
       *  to improve performances use "Serial" as USB type for teensy (very important)
       *    rise CPU clock
       *    print as HEX (it saves 20% of time)
       */
      for(size_t i = 0; i < position; i++) {
        Serial.print(streamBuffer[i], DEC);
        if(i % 40 == 39) Serial.println();
        else Serial.print(" ");    
      }
      // count the samples
      nSamples = nSamples+(uint32_t)nvalues;
    }

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
    if(debug_ && streamData) { 
    Serial.println();
    Serial.print("Number of values: ");
    Serial.println(nvalues);
    
    time = micros()-time;
    Serial.print("time: ");
    Serial.print(time);
    Serial.println(" us");

    Serial.print("delay time in the loop: ");
    Serial.print(Delay_us);
    Serial.println(" us");
    
    Serial.print("Current Buffer index DMA is writing to: ");
    Serial.println(buffer_idx0);

    Serial.print("Old Buffer index DMA was: ");
    Serial.println(old_buffer_idx0);

    float us4value = (float)time/(float)nvalues;
    Serial.print("us for value: ");
    Serial.println(us4value);

    Serial.print("number of samples: ");
    Serial.println(nSamples);
    Serial.println();
    }

    // get last position in the ring buffer 
    old_buffer_idx0 = buffer_idx0;
    
    // toogle between stream and not stream 
    if (Serial.available()) streamData = false;

    // count iteractions
    Iterations++;

    // no idea, it was here already
    if(adc.adc0->fail_flag || adc.adc1->fail_flag) {
        Serial.printf("ADC error, ADC0: %x ADC1: %x\n", adc.adc0->fail_flag, adc.adc1->fail_flag);
    }
    
    /* Delay the next iteration
     *  it is used to have a good number of samples streamed (ideally 50)
     */
    delayMicroseconds(Delay_us);
}
