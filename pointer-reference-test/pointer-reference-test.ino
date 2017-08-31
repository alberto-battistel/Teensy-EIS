
void passByPointer(uint16_t* var)  {
    // use for vectors ;)
    uint16_t aa = *(var+1);
    Serial.println("Pass by pointers (*)");
    Serial.println("it uses pointer to address the memory position.");
    Serial.print("You can go to the second index of a vector:\t");
    Serial.println(aa);
}

void passByReference(uint16_t& var)  {
    // use to change the value

    Serial.println("Pass by reference (&)");
    Serial.println("it refers to a variable");
    Serial.println("and it can change it permanently...");
    Serial.print("Before changing:\t");
    Serial.println(var);
    Serial.print("After changing:\t");
    uint16_t aa = var++;
    Serial.print(aa);
    Serial.println("\tInside the function");
}

void setup() {

    Serial.begin(9600);
      while (!Serial) {
      ; // wait for serial port to connect.
      }
    
    uint16_t var_1[2] = {1,22};
    passByPointer(&var_1[0]);
    Serial.print("Which is:\t");
    Serial.print(var_1[1]);
    Serial.println("\tOutside the function");
    
    Serial.println();
  
    uint16_t var_2 = 3;
    passByReference(var_2);
    Serial.print(var_2);
    Serial.println("\tOutside the function");

}

void loop() {
    exit(0);

}
