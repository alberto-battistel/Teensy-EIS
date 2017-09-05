
#include <Arduino.h>
#include "PingPong.h"  


// Delay for the communication of the parameters
//int communicationDelay = 10;      // delay for serial communication

//struct parameters4measurement Parameters;

/*
 * Setup
 */

void setup() {
    Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps

    // wait for the PC
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
    
    // get parameters from the pc
    while (Parameters.StartStop == 1) pingPongParameters(); 

}

/*
 * Main loop
 */
 
void loop() {

    // get parameters from the pc
    while (Parameters.StartStop == 1) pingPongParameters(); 
    if (Parameters.StartStop == 2) shutDown();
    Parameters.StartStop = 1;
}
