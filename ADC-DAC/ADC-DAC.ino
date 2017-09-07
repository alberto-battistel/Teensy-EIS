#include <ADC.h>
#include <DMAChannel.h>
#include <array>
#include "DefaultParameters.h"
#include "DAC.h"


/* do you want to check timing?
 *  it print info about how many samples are collected per iteration
 *  and timing...
 */
//#define DEBUG

// ADC pins 
const uint8_t adc_pin0 = A9;  // digital pin 23, on ADC0
const uint8_t adc_pin1 = A19; // digital pin 31, on ADC1
// LED pin
const int ledPin = 13;

// get parameters from the PC
#include "PingPong.h"   // contains function to get Parameters from Python

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

// store index of DMA buffer
volatile size_t buffer_idx0;
volatile size_t buffer_idx1;

// store old index of DMA buffer
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
    pinMode(ledPin, OUTPUT);
    Serial.begin(9600);
    while (!Serial) {
    ; // wait for serial port to connect.
    }

    // DAC
    analogWriteResolution(12);
    pinMode(pinDAC,OUTPUT);

    // ADC
    pinMode(adc_pin0, INPUT);
    pinMode(adc_pin1, INPUT);
    activateADC();
    
    // wait for input
    #ifdef DEBUG
    Serial.println("Bebugging is on!");
    #endif

    Serial.println("Ready!");
    delay(500);
    while (!(Serial.available() > 0))  {
        digitalWrite(ledPin, HIGH);   // set the LED off
        Serial.println("Waiting...");
        Serial.send_now();
        delay(250);
        digitalWrite(ledPin, LOW);   // set the LED off
        delay(250);
    }
}


/*
 * loop
 */
void loop() {
    switch (Parameters.StartStop) {
        case 2: {
            exit(0);
            break;
        }
        case 1: {
            // get parameters from the pc
            digitalWrite(ledPin, LOW);   // set the LED off
            while (Parameters.StartStop == 1) pingPongParameters();

            digitalWrite(ledPin, HIGH);   // set the LED on
            
            // start everything
            // DAC
            timerDAC.begin(DAC_callback, Parameters.DAC_delay);
            
            // set ADC average and resolution
            setADC(Parameters.ADC_average, Parameters.ADC_resolution);
            
            // calculate how much delay in the loop
            Delay_loop =  calculateDelay_loop(Parameters.ADC_frequency);
            
            // enable PDB
            setPDB(Parameters.ADC_frequency);
            
            // Kick off ADC conversion.
            startConversion();
            
            // restart counting of samples
            counterSamplesADC0 = 0;
            counterSamplesADC1 = 0;
            
            // restart index of DMA buffer
            old_buffer_idx0 = 0;
            old_buffer_idx1 = 0;

            // flag 
            Parameters.ADC_enoughSamples[0] = false;
            Parameters.ADC_enoughSamples[1] = false;
           
            Serial.println("Go!");
            
            delayMicroseconds(Delay_loop);
        }      
        default:  {
            // Read position on DMA buffer
            noInterrupts();
            buffer_idx0 = (((uint16_t*) dma0.destinationAddress()) - &buffer[0].v_adc0) / 2;
            buffer_idx1 = (((uint16_t*) dma1.destinationAddress()) - &buffer[0].v_adc1) / 2;
            interrupts();

            #ifdef DEBUG
            // count iterations and tell when an ADC finishes
            if (Parameters.ADC_enoughSamples[0]) Serial.println("ADC0 Done!");
            if (Parameters.ADC_enoughSamples[1]) Serial.println("ADC1 Done!");
            Serial.print("Iteration: ");
            Serial.println(Iterations++);      
            #endif
        }
    }

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
        Serial.println("ADC error in the loop");
        adc.adc0->printError();
        adc.adc1->printError();
    }

    /* Delay the next iteration
     *  it is used to have a good number of samples streamed (ideally 50)
     */
    delayMicroseconds(Delay_loop);
}

// all functions are defined in .h files
// I know it is not the good way, but I could not make work .cpp files...


