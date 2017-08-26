int16_t Delay_us = 10;
int16_t l= 1;
int16_t counter = 0;

void setup() {
    Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
    while (!Serial.available())
    while (Serial.available())  Serial.read();
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int16_t i= 1; i< 10; i++)  {
    Serial.print(l*i);
    Serial.print(' ');  
  }
//  Serial.println();
  Serial.print(" ");
  Serial.println(counter++);
  l++;
  if (l>10) l= 1;
  delayMicroseconds(Delay_us);
  if(counter > 10000) exit(0);
}
