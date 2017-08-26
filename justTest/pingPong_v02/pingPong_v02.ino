bool measuring = false; // are you measuring? it is used to update measurement parameters
int stdDelay = 10;      // delay for serial communication


// parameters for the measurement
struct para4ADC {
  // default values
  bool starting = false;    // ready to start? when it goes true it print the parameters
  float sFrequency = 1000;  // Hz
  uint32_t nSamples = 1000; // number of samples
  int average = 32;         // average in the adc
  int resolution = 16;      // resolution of the adc
} Parameters;

// handle the parameters for the measurement
class obj4ADC {
  void checkResolution(float);
  void checkAverage(float); 
  public:
  void update_value(char, float);
  void print_values();
} handlePara;

void obj4ADC::checkResolution(float x) {
  // resolution make sense between 8 and 16
  if (x >= 16) Parameters.resolution = 16;
  else if (x >= 12) Parameters.resolution = 12;
  else if (x >= 8)  Parameters.resolution = 8;
}

void obj4ADC::checkAverage(float x) {
  // average has to be a power of 2 until 32
  if (x >= 32) Parameters.average = 32;
  else if (x >= 16) Parameters.average = 16;
  else if (x >= 8)  Parameters.average = 8;
  else if (x >= 4)  Parameters.average = 4;
  else if (x >= 2)  Parameters.average = 2;
  else Parameters.average = 1; 
}

void obj4ADC::update_value(char letter, float x) {
  // here it really change the Parameters for the measurement
    Parameters.starting = false;  // if you come here then you do not want to start the measurement
  switch (letter) {
    case 'S':   // make it ready to start and print the parameters
      if (x != 0) Parameters.starting = true;
      else Parameters.starting = false;
      handlePara.print_values();
      break;
    case 'F':   // sampling frequency
      Parameters.sFrequency = x;
      break;
    case 'N':   // number of samples
      Parameters.nSamples = (uint32_t)x;
      break;
    case 'R':   // adc resolution
      checkResolution(x);
      break;
    case 'A':   // adc average
      checkAverage(x);
      break;
  }
}

void obj4ADC::print_values()  {
  // print the parameters
  delay(stdDelay);
  Serial.print("S");
  Serial.print(Parameters.starting);
  Serial.print(" ");
  Serial.print("F");
  Serial.print(Parameters.sFrequency);
  Serial.print(" ");
  Serial.print("N");
  Serial.print(Parameters.nSamples);
  Serial.print(" ");
  Serial.print("R");
  Serial.print(Parameters.resolution);
  Serial.print(" ");
  Serial.print("A");
  Serial.println(Parameters.average);
}

/*
 * Setup
 */

void setup() {
    Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
    // wait for pc to give a signal
//    while (!Serial.available());
//    Serial.readStringUntil('\n'); 
//    Serial.println("Ready!");
//    delay(stdDelay);  
    // get parameters from the pc
    while (Parameters.starting) pingPongParameters(); 
}

/*
 * Main loop
 */
 
void loop() {

    if (!measuring) pingPongParameters(); // get parameters from the pc
    
}

/*
 * Functions declaration
 */
 
// get parameters from the pc
void pingPongParameters() {
    // make sure you are not ready to start
    Parameters.starting = false;
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
            delay(stdDelay);      // wait to see if other parameters are coming
        }  
}
