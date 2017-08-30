#include <ADC.h>
#include <DMAChannel.h>
#include <array>
#include <assert.h>



/* DAC part
 * it comes from example_DAC
 * place Waveforms_Gen.h in the same folder
 */
#include "Waveforms_Gen.h"  // matlab generated
#define pinDAC A22

IntervalTimer timer0;

volatile int t = 0;

void timer0_callback() {
	analogWrite(pinDAC,waveformsTable[t]);
	t++;
	if (t >= maxSamplesNum) {
		t=0;
	}
}
/* end of DAC
 */

/* do you want to check timing?
 *  it print info about how many samples are collected per iteration
 *  and timing...
 */
//#define DEBUG

/* do you want to see the number of iteraion? */
#define ITERATIONS

/* Stop after a number of samples
 *  allow to stop the stream after collecting maxSamples
 */
bool stop4Samples[2] = {false, false};    // flag, signal that it streamed all the data of channels 0
uint32_t nSamples0 = 0;        // counter for data streamed for channel 0
uint32_t nSamples1 = 0;        // counter for data streamed for channel 1
uint32_t maxSamples = 30000;     // stop after

/* Settings for Ring buffer (DMA) and aquisition frequency */
// DMA buffer size
#define BUFFERSIZE 16384

// stream buffer size
uint16_t streamBuffer[2][BUFFERSIZE];

// Frequency of acquistion (max 250 kHz with 8 bit resolution)
#define FREQUENCY 3000   // Hz

/* Settings for ADC
 *  AVERAGE 32 OK at 10 kHz
 *  AVERAGE max 8 at 50 kHz
 *  RESOLUTION 16 at 10 kHz gives a floor noise of almost 0.01 mV
 *  RESOLUTION 12 at 10 kHz gives a floor noise better than 0.1 mV
 *  RESOLUTION 8 at 250 kHz gives a floor noise of 0.1 mV
 */
#define AVERAGE 1    // max 8 at 50 kHz
#define RESOLUTION 16 // bits

/* flags and counter */
// command to stop streaming
boolean streamData = true;

// track in print time for serial print
unsigned long time = 0;

// store old index of buffer
volatile size_t old_buffer_idx0 = 0;
volatile size_t old_buffer_idx1 = 0;

// count iterations
int16_t Iterations = 0;

// flag to know that the loop just started
bool JustStart = true;

/* Calculate the best delay
 *  it calculates the best delay (Delay_us) between iterations
 *  to have the best number of samples to send (BEST_N_VAL)
 *  according to the time it takes to send a value (TIME_1VAL) and the aquistion frequency (FREQUENCY)
 *  TIME_1VAL is between 1 and 3 us
 */
 #define BEST_N_VAL 4000
 #define TIME_1VAL 3e-6   // s
 int32_t Delay_us = max((int32_t)(BEST_N_VAL*(1-TIME_1VAL*FREQUENCY)/FREQUENCY*1e6), 0);    // us


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
    Serial.begin(9600);
    while (!Serial) {
    ; // wait for serial port to connect.
    }

    /* DAC
     *
     */
    analogWriteResolution(12);
    timer0.begin(timer0_callback, Delay);
    pinMode(pinDAC,OUTPUT);
    analogWrite(pinDAC,0);
    //

    pinMode(adc_pin0, INPUT);
    pinMode(adc_pin1, INPUT);

    //~ constexpr uint32_t pdb_trigger_frequency = FREQUENCY;
    uint32_t pdb_trigger_frequency = FREQUENCY;
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
//    constexpr uint32_t mod = (F_BUS / pdb_trigger_frequency);
    uint32_t mod = (F_BUS / pdb_trigger_frequency);
//    static_assert(mod <= 0x10000, "Prescaler required.");
    assert(mod <= 0x10000);
    PDB0_MOD = (uint16_t)(mod-1);
    PDB0_CH0C1 = PDB_CHnC1_TOS_1 | PDB_CHnC1_EN_1; // PDB triggers ADC0 SC1A
    PDB0_CH1C1 = PDB_CHnC1_TOS_1 | PDB_CHnC1_EN_1; // PDB triggers ADC1 SC1A
    PDB0_SC = ADC_PDB_CONFIG | PDB_SC_PRESCALER(0) | PDB_SC_MULT(0) | PDB_SC_LDOK;



    // wait for input
    #ifdef DEBUG
    Serial.println("Bebugging is on!");
    #endif

    Serial.println("Ready!");
    delay(1000);
    while (!(Serial.available() > 0))  {
        Serial.println("Waiting...");
        Serial.send_now();
        delay(1000);
    }
    while (Serial.available() > 0) Serial.read();
    Serial.println("Go!");
    // Kick off ADC conversion.
    PDB0_SC = ADC_PDB_CONFIG | PDB_SC_PRESCALER(0) | PDB_SC_MULT(0) | PDB_SC_SWTRIG; // start

    delayMicroseconds(Delay_us);
}



void loop() {
    // stop when it reaches the right number of samples
    #ifdef DEBUG
		if (stop4Samples[0]) Serial.println("Done 0");
		if (stop4Samples[1]) Serial.println("Done 1");
    #endif
    if (stop4Samples[0] && stop4Samples[1]) {
		exit(0);
    }

    #ifdef ITERATIONS
        Serial.println(Iterations);
    #endif
  	/* Read position on DMA buffer
  	 */
    noInterrupts();
    size_t buffer_idx0 = (((uint16_t*) dma0.destinationAddress()) - &buffer[0].v_adc0) / 2;
    size_t buffer_idx1 = (((uint16_t*) dma1.destinationAddress()) - &buffer[0].v_adc1) / 2;
    interrupts();

    // number of values streamed
    uint16_t nvalues0;
    uint16_t nvalues1;
    //// update timer
    time = micros();

    /* Transfer from ringbuffer (DMA) to stream buffer
     *  ring buffer can wrap
     *  stream buffer is alway filled from the first position and does not wrap
     *  you need to know how many data values go from ring buffer to stream buffer
     */

    if(streamData) { // do it only if
		nvalues0 = StreamData(nSamples0, 0, old_buffer_idx0, buffer_idx0);
		nSamples0 = nSamples0 + (uint32_t)nvalues0;

		nvalues1 = StreamData(nSamples1, 1, old_buffer_idx1, buffer_idx1);
		nSamples1 = nSamples1 + (uint32_t)nvalues1;
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
    #ifdef DEBUG
		Serial.println();
		Serial.print("Number of values: ");
		Serial.print(nvalues0);
		Serial.print(", ");
		Serial.println(nvalues1);

		time = micros()-time;
		Serial.print("time: ");
		Serial.print(time);
		Serial.println(" us");

		Serial.print("delay time in the loop: ");
		Serial.print(Delay_us);
		Serial.println(" us");

		Serial.print("Current Buffer index DMA is writing to: ");
		Serial.print(buffer_idx0);
		Serial.print(", ");
		Serial.println(buffer_idx0);

		Serial.print("Old Buffer index DMA was: ");
		Serial.print(old_buffer_idx0);
		Serial.print(", ");
		Serial.println(old_buffer_idx1);

		float us4value = (float)time/(float)nvalues0;
		Serial.print("us for value: ");
		Serial.println(us4value);

		Serial.print("number of samples: ");
		Serial.print(nSamples0);
		Serial.print(", ");
		Serial.println(nSamples1);
		Serial.println();
    #endif

    // get last position in the ring buffer
    old_buffer_idx0 = buffer_idx0;
    old_buffer_idx1 = buffer_idx1;

    //// toogle between stream and not stream
    //if (Serial.available()) streamData = false;

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




/*
 * Function declaration
 */

/* Stream data
 *  take from DMA buffer, transfer to stream buffer, and stream
 *  return the number of data streamed
 *  I know I could use pointers here, but I am lazy
 *  ERROR, I need to use pointers to pass buffer[i].v_adc0 and buffer[i].v_adc1...
 *  not corrected yet
 */
uint16_t StreamData(uint32_t NumberOfSamples, uint8_t bufferIndex, volatile size_t old_buffer_, volatile size_t buffer_) {
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
				streamBuffer[bufferIndex][posStreamBuffer++] = buffer[i].v_adc0;
				nvalues++;
			}
		}
    /* CASE II
     *  ring buffer wrapped
     *  take data from old position until the end of the buffer and start over from the beginning
     */
	else {
        for(size_t i = old_buffer_; i < BUFFERSIZE; i++) {
			streamBuffer[bufferIndex][posStreamBuffer++] = buffer[i].v_adc0;
			nvalues++;
        }
        for(size_t i = 0; i < buffer_; i++) {
			streamBuffer[bufferIndex][posStreamBuffer++] = buffer[i].v_adc0;
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
    if(NumberOfSamples+nvalues >= maxSamples)  {
	// it is reaching the right number of samples to stream
		uint32_t samplesTooMuch = NumberOfSamples + nvalues - maxSamples;
		last2Stream = posStreamBuffer - (uint16_t)samplesTooMuch;
		stop4Samples[bufferIndex] = true;
		streamData = true;
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
