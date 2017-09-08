
String string2parameters = " ";
uint16_t communicationDelay = 100;

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
    
//    // get parameters from the pc
//    string2parameters = Serial.readStringUntil('\n');
//    Serial.println(string2parameters.length());
//    Serial.println(string2parameters); 

}

void loop() {
  // get parameters from the pc
    pingPongParameters();
    
    delay(communicationDelay);
}

// get parameters from the pc
void pingPongParameters() {
    // make sure you are not ready to start
    //Parameters.StartStop = 1;
        while (Serial.available()) {
            // first byte contains the parameter to change:
            int info = Serial.read();
            if info == '/r' 
            // then it looks for a number
            float valueFloat = 0;
            valueFloat = Serial.parseFloat();
            Serial.println(info);
            Serial.println(valueFloat);
        }  
}
