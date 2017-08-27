//https://forum.pjrc.com/threads/26036-Fastest-DAC-speed-possible-for-Teensy-3-1-using-Arduino

// include lookup table of sine values from waveforms.h
#include "Waveforms_Gen.h"  // matlab generated
#define pinDAC A22

IntervalTimer timer0;
void setup() {
  analogWriteResolution(12);
  timer0.begin(timer0_callback, Delay); 
  pinMode(pinDAC,OUTPUT);
  analogWrite(pinDAC,0);
}

volatile int t = 0;

void timer0_callback() {  
  analogWrite(pinDAC,waveformsTable[t]); 
  t++;
  if (t >= maxSamplesNum) {
   t=0; 
  }
}

void loop() {
  
}
