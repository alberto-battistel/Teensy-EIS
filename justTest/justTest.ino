
#define BUFFERSIZE 64
#define DEBUG_ false

uint16_t a[BUFFERSIZE]; 
uint32_t Delay_us = 100;
uint32_t count = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial){
  }

  for(uint16_t i = 0; i < BUFFERSIZE; i++) a[i] = (uint16_t)round((0.5+sin(i*2*PI/BUFFERSIZE)/2)*(pow(2,16)-1));
  
  Serial.println("Ready!");
  while (!Serial.available())
  Serial.read();
  
    
//  Serial.println(PI);
//  Serial.println();

}

void loop() {
  if(DEBUG_) {
    for(uint32_t i = 0; i < BUFFERSIZE; i++) {
      Serial.print(a[i]);
      Serial.print(" ");
      if(i%32 == 31) Serial.println();
    }
    Serial.println();
    delayMicroseconds(1000*Delay_us);
  }  
  else  {
    for(uint32_t i = 0; i < BUFFERSIZE; i++) {
      Serial.write(lowByte(a[i]));
      Serial.write(highByte(a[i]));
      /* use 
       * data = s.readline() and 
       * np.fromstring(data,dtype = np.uint16)
       * to get the data in python
       */
    }
    Serial.println();
    delayMicroseconds(Delay_us);
  }
  count++;
  if(count > 5 ) { 
    Serial.println();
    exit(0);
//    Serial.println("Ready!");
//    while (!Serial.available())
//    Serial.read();
//    count = 0;
  }
}
