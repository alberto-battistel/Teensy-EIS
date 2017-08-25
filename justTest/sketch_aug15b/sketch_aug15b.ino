int firstByte = 0;   // for incoming serial data
char restByte[4];
float valueFloat = 0;
uint32_t valueInt = 0;
bool printOnce = false;
bool measuring = false;

union u_tag {
   byte b[4];
   float floatVal;
   uint32_t intVal;
} u;


void setup() {
    Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
}

void loop() {

    if (!measuring) {
    // read data
        if (Serial.available() > 0) {
            printOnce = false;
            Serial.println(Serial.available());
            // read the incoming byte:
            firstByte = Serial.read();

            for (uint16_t i = 0; i< 4;  ++i) restByte[i] = Serial.read();
            
            switch (firstByte)  {
              case 'F':
                  Serial.println("Frequency:");
                  for (uint16_t i = 0; i< 4;  ++i) u.b[i] = restByte[i];
                  Serial.println(u.floatVal);
                  break;
              case 'R':
                  Serial.println("Resolution:");
                  for (uint16_t i = 0; i< 4;  ++i) u.b[i] = restByte[i];
                  Serial.println(u.intVal);
                  break;
              default:
                  Serial.println("Not the right info!");
                  break;
              
            }
            
//            // say what you got:
//            Serial.print("I received: ");
//            Serial.println(incomingByte, HEX);
//            printOnce = false;
        }
        else if(!printOnce) {
            Serial.println("Waiting");
            Serial.println();
            printOnce = ~printOnce;
        }
    }
}
