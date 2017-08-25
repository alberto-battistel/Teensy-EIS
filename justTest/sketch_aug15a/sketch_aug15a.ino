int incomingByte = 0;   // for incoming serial data
bool printOnce = false;

void setup() {
    Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
}

void loop() {

    // send data only when you receive data:
    if (Serial.available() > 0) {
        Serial.println(Serial.available());
        // read the incoming byte:
        incomingByte = Serial.read();

        // say what you got:
        Serial.print("I received: ");
        Serial.println(incomingByte, HEX);
        printOnce = false;
    }
    else if(!printOnce) {
        Serial.println("Waiting");
        Serial.println();
        printOnce = ~printOnce;
    }
}
