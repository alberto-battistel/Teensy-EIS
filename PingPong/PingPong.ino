bool measuring = false; // are you measuring? it is used to update measurement parameters
int communicationDelay = 10;      // delay for serial communication


// parameters for the measurement
struct parameters4measurement {
  // default values
  uint16_t StartStop = 1;    // 0 ->start aquisition, running; 1-> finished to aquire, waiting for paramerters; 2-> stop teensy
  float ADC_frequency = 1000;  // Hz
  uint32_t ADC_nSamples = 1000; // number of samples to collect
  uint16_t ADC_average = 32;         // average in the ADC
  uint16_t ADC_resolution = 16;      // resolution of the ADC
  uint32_t DAC_delay = 10;          // us delay of the DAC
  uint16_t DAC_type = 0;            // kind of signal to output (not implemented yet)
  
} Parameters;

// handle the parameters for the measurement
class obj4ADC {
  void checkStartStop(float);
  void checkADC_Resolution(float);
  void checkADC_Average(float); 
  void checkDAC_Type(float);
  public:
  void update_value(char, float);
  void print_values();
} handlePara;

// 
void obj4ADC::checkStartStop(float x) { 
  if (x >= 2) {
    // total shut down
    Parameters.StartStop = 2;
  }
  else if (x >= 1) {
    // stop aquisition and wait for more Parameters
    Parameters.StartStop = 1;    
  }
  else {
    // start aquisition
    Parameters.StartStop = 0; 
    print_values();
  }
}
// check that the resolution is 8, 12, or 16
void obj4ADC::checkADC_Resolution(float x) {
  // ADC_resolution make sense between 8 and 16
  if (x >= 16) Parameters.ADC_resolution = 16;
  else if (x >= 12) Parameters.ADC_resolution = 12;
  else if (x >= 8)  Parameters.ADC_resolution = 8;
}

// check that the average is a power of 2 until 32 (2^5)
void obj4ADC::checkADC_Average(float x) {
  // ADC_average has to be a power of 2 until 32
  if (x >= 32) Parameters.ADC_average = 32;
  else if (x >= 16) Parameters.ADC_average = 16;
  else if (x >= 8)  Parameters.ADC_average = 8;
  else if (x >= 4)  Parameters.ADC_average = 4;
  else if (x >= 2)  Parameters.ADC_average = 2;
  else Parameters.ADC_average = 1; 
}

// check that the DAC type is OK
void obj4ADC::checkDAC_Type(float x) {
  // DAC_type has to be 0<=x<=5
  if (x >= 5) Parameters.DAC_type = 5;
  else if (x >= 4)  Parameters.DAC_type = 4;
  else if (x >= 3)  Parameters.DAC_type = 3;
  else if (x >= 2)  Parameters.DAC_type = 2;
  else if (x >= 1)  Parameters.DAC_type = 1;
  else Parameters.DAC_type = 0; 
}

// update the read values into Paramters
void obj4ADC::update_value(char letter, float x) {
  // here it really change the Parameters for the measurement
    //Parameters.StartStop = 1;  // if you come here then you do not want to start the measurement
  switch (letter) {
    case 'S':   // make it ready to start and print the parameters
      checkStartStop(x);      
      break;
    case 'F':   // ADC sampling frequency
      Parameters.ADC_frequency = x;
      break;
    case 'N':   // number of samples
      Parameters.ADC_nSamples = (uint32_t)x;
      break;
    case 'R':   // ADC resolution
      checkADC_Resolution(x);
      break;
    case 'A':   // ADC average
      checkADC_Average(x);
      break;
    case 'D':   // DAC delay
      Parameters.DAC_delay = max((uint32_t)2, (uint32_t)x); // not less than 2 us
    case 'T':   // DAC type
      checkDAC_Type(x);
      break;
  }
}

// give feedback on Parameters
void obj4ADC::print_values()  {
  // print the parameters
  delay(communicationDelay);
  Serial.print("S");
  Serial.print(Parameters.StartStop);
  Serial.print(" ");
  Serial.print("F");
  Serial.print(Parameters.ADC_frequency);
  Serial.print(" ");
  Serial.print("N");
  Serial.print(Parameters.ADC_nSamples);
  Serial.print(" ");
  Serial.print("R");
  Serial.print(Parameters.ADC_resolution);
  Serial.print(" ");
  Serial.print("A");
  Serial.print(Parameters.ADC_average);
  Serial.print(" ");
  Serial.print("D");
  Serial.print(Parameters.DAC_delay);
  Serial.print(" ");
  Serial.print("T");
  Serial.println(Parameters.DAC_type);
}

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

/*
 * Functions declaration
 */
 
// get parameters from the pc
void pingPongParameters() {
    // make sure you are not ready to start
    //Parameters.StartStop = 1;
        while (Serial.available()) {
            // first byte contains the parameter to change:
            int info = Serial.read();
            // then it looks for a number
            float valueFloat = 0;
            valueFloat = Serial.parseFloat();
            // read until the end of the line but do not store the info
            Serial.readStringUntil('\n');
            // update the parameters
            handlePara.update_value(info, valueFloat);  // here you can say to start
            delay(communicationDelay);      // wait to see if other parameters are coming
        }  
}

// shut down teensy
void shutDown() {
    Serial.println("Shut down!");
    exit(0);
}

