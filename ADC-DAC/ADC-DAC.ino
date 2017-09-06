#include <ADC.h>
#include <DMAChannel.h>
#include <array>
//#include <assert.h>
#include "DAC.h"
#include "PingPong.h"   // contains function to get Parameters from Python

// ADC pins 
const uint8_t adc_pin0 = A9;  // digital pin 23, on ADC0
const uint8_t adc_pin1 = A19; // digital pin 31, on ADC1



/* Count number of samples
 *  allow to stop the stream after collecting maxSamples
 */
bool stop4Samples[2] = {false, false};    // flag, signal that it streamed all the data of channels 0
uint32_t counterSamplesADC0 = 0;        // counter for data streamed for channel 0
uint32_t counterSamplesADC1 = 0;        // counter for data streamed for channel 1

/* Settings for Ring buffer (DMA)  */
// DMA buffer size
#define BUFFERSIZE 4096

// stream buffer size
uint16_t streamBuffer[2][BUFFERSIZE];

/* flags and counter */
// command to stop streaming
boolean streamData = true;

#ifdef DEBUG
// track in print time for serial print
unsigned long time_us = 0;
// count iterations
int16_t Iterations = 0;
#endif

// store old index of buffer
volatile size_t old_buffer_idx0 = 0;
volatile size_t old_buffer_idx1 = 0;

// delay between loops to collect a suitable number of data to stream
uint32_t Delay_loop;

/*
 * I did not touch much from here on
 */

ADC adc;
DMAChannel dma0;
DMAChannel dma1;

struct CapturePair {
    uint16_t v_adc0;
    uint16_t v_adc1;
};
std::array<volatile CapturePair, BUFFERSIZE> buffer;
volatile size_t write_pos = 0;

/*  
 *  Function for modifing and running ADC and DAC
 */
#include "SettingParameters.h"


/*
 * Setup
 */

void setup() {
    Serial.begin(9600);
    while (!Serial) {
    ; // wait for serial port to connect.
    }

    // DAC
    analogWriteResolution(12);
    pinMode(pinDAC,OUTPUT);
    timerDAC.begin(DAC_callback, Parameters.DAC_delay);

    // ADC
    pinMode(adc_pin0, INPUT);
    pinMode(adc_pin1, INPUT);

    // set ADC average and resolution
    setADC(Parameters.ADC_average, Parameters.ADC_resolution);

    // DMA
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
	
	  // calculate how much delay in the loop
	  Delay_loop =  calculateDelay_loop(Parameters.ADC_frequency);
	
    // enable PDB
    setPDB(Parameters.ADC_frequency);

    // wait for input
    #ifdef DEBUG
    Serial.println("Bebugging is on!");
    #endif

    Serial.println("Ready!");
    delay(500);
    while (!(Serial.available() > 0))  {
        Serial.println("Waiting...");
        Serial.send_now();
        delay(500);
    }
    while (Serial.available() > 0) Serial.read();
    Serial.println("Go!");
    
    // Kick off ADC conversion.
    startConversion();
    
    delayMicroseconds(Delay_loop);
}


/*
 * loop
 */
void loop() {
    // stop when it reaches the right number of samples
    #ifdef DEBUG
    if (stop4Samples[0]) Serial.println("Done 0");
    if (stop4Samples[1]) Serial.println("Done 1");
    Serial.print("Iteration: ")
    Serial.println(Iterations++);
    #endif

    // 
    // !!!!!!!!!!!!!!!!!!!!!!!
    
    if (Parameters.StartStop == 1) exit(0);

    // !!!!!!!!!!!!!!!!!!!!!!!
    //
    
    /* Read position on DMA buffer
     */
    noInterrupts();
    size_t buffer_idx0 = (((uint16_t*) dma0.destinationAddress()) - &buffer[0].v_adc0) / 2;
    size_t buffer_idx1 = (((uint16_t*) dma1.destinationAddress()) - &buffer[0].v_adc1) / 2;
    interrupts();

    // number of values streamed
    uint16_t nvalues0;
    uint16_t nvalues1;
    
    #ifdef DEBUG
    // update timer
    time_us = micros();
    #endif

    // Transfer from ringbuffer (DMA) to stream buffer and stream
    if(streamData) { // do it only if
    nvalues0 = StreamData(counterSamplesADC0, 0, &buffer[0].v_adc0, old_buffer_idx0, buffer_idx0);
    counterSamplesADC0 = counterSamplesADC0 + (uint32_t)nvalues0;

    nvalues1 = StreamData(counterSamplesADC1, 1, &buffer[0].v_adc1, old_buffer_idx1, buffer_idx1);
    counterSamplesADC1 = counterSamplesADC1 + (uint32_t)nvalues1;
    }

  	#ifdef DEBUG
  	printAllInfo();
  	#endif
	
    // get last position in the ring buffer
    old_buffer_idx0 = buffer_idx0;
    old_buffer_idx1 = buffer_idx1;

    // no idea, it was here already
    if(adc.adc0->fail_flag || adc.adc1->fail_flag) {
    Serial.printf("ADC error, ADC0: %x ADC1: %x\n", adc.adc0->fail_flag, adc.adc1->fail_flag);
    }

    /* Delay the next iteration
     *  it is used to have a good number of samples streamed (ideally 50)
     */
    delayMicroseconds(Delay_loop);
}

// all functions are defined in .h files
// I know it is not the good way, but I could not make work .cpp files...

