/* DAC part
 * it comes from example_DAC
 * place Waveforms_Gen.h in the same folder
 */

#ifndef DAC_H
#define DAC_H 
 
#include "Waveforms_Gen.h"  // matlab generated
#define pinDAC A22
#define pinREF A21

// to make the reference voltage for analog voltage shift
void DACvoltageReference()  {
    //0x800 = 2048
    analogWrite(pinREF, 0x800);
}

// for signal generator
IntervalTimer timerDAC;

// to index the DAC vector
volatile uint32_t counterDAC = 0;
//uint16_t DelayDAC = 100; // in us

/*  DAC callback
 *  this is called at DelayDAC time by the timerDAC
 */
void DAC_callback();

    //~ /* DAC
     //~ *
     //~ */
    //~ analogWriteResolution(12);
    //~ pinMode(pinDAC,OUTPUT);
    //~ timerDAC.begin(DAC_callback, DelayDAC);		// function to call to change DAC frequency
    //~ //pinMode(pinDAC,OUTPUT);
    //~ //analogWrite(pinDAC,0);
    //~ //
    
    
    /*  DAC callback
 *  this is called at DelayDAC time by the timerDAC
 */
void DAC_callback() {
    analogWrite(pinDAC,waveformsTable[counterDAC]);
    counterDAC++;
    if (counterDAC >= maxSamplesNum) {
        counterDAC = 0;
    }
}

#endif
