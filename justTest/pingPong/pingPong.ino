int firstByte = 0;   // for incoming serial data
char restByte[16];
float valueFloat = 0;
bool printOnce = false;
bool measuring = false;
char endOfLine = 10;

struct para4ADC {
  bool starting = false;    // wait for update parameters
  float sFrequency = 1000;  // Hz
  uint32_t nSamples = 1000; // number of samples
  int average = 32;         // average in the adc
  int resolution = 16;      // resolution of the adc
} Parameters;

class obj4ADC {
  void checkResolution(float);
  void checkAverage(float); 
  public:
  void update_value(char, float);
  void print_values();
//  para4ADC Parameters;
} handlePara;

void obj4ADC::checkResolution(float x) {
  if (x >= 16) Parameters.resolution = 16;
  else if (x >= 12) Parameters.resolution = 12;
  else Parameters.resolution = 8;
}

void obj4ADC::checkAverage(float x) {
  if (x >= 32) Parameters.average = 32;
  else if (x >= 16) Parameters.average = 16;
  else if (x >= 8)  Parameters.average = 8;
  else if (x >= 4)  Parameters.average = 4;
  else if (x >= 2)  Parameters.average = 2;
  else              Parameters.average = 1; 
}
void obj4ADC::update_value(char letter, float x) {
  switch (letter) {
    case 'S':
      Parameters.starting = true;
      break;
    case 'F':
      Parameters.sFrequency = x;
      break;
    case 'N':
      Parameters.nSamples = (uint32_t)x;
      break;
    case 'R':
      checkResolution(x);
      break;
    case 'A':
      checkAverage(x);
      break;
  }
}

void obj4ADC::print_values()  {
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

void setup() {
    Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
    // wait for pc to give a signal
    while (!Serial.available());
    Serial.readStringUntil('\n');    
}

void loop() {

    if (!measuring) {
    // read data
        if (Serial.available()) {
            printOnce = false;
            // first byte contains the parameter to change:
            int info = Serial.read();
            // next 4 bytes are the value (example F1234.5)
            valueFloat = Serial.parseFloat();
            // read until the end of the line but do not store the info
            Serial.readStringUntil('\n');
            switch (info)  {
                case 'S':
                    Serial.println("Starting");
                case 'F':
                    Serial.println("Frequency");
                    Parameters.sFrequency = valueFloat;
                    Serial.println(Parameters.sFrequency);
                    break;
                case 'R':
                    Serial.println("Resolution");
                    Parameters.resolution = (int) valueFloat;
                    Serial.println(Parameters.resolution);
                    break;
                case 'A':
                    Serial.println("Average");
                    Parameters.average = (int) valueFloat;
                    Serial.println(Parameters.average);
                    break;
                case 'N':
                    Serial.println("Number of samples");
                    Parameters.nSamples = (int) valueFloat;
                    Serial.println(Parameters.nSamples);
                    break;
                default:
                    break;
            }
        }
        
        else if(!printOnce) {
            //Serial.println("Give me: S, F, R, A, N");
            handlePara.print_values();
            printOnce = ~printOnce;
        }
    }
}
