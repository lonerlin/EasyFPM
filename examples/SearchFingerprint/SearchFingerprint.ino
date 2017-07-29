
#include<EasyFPM.h>
EasyFPM fpm;
SoftwareSerial mySerial(2, 3);
void setup() {
  
  Serial.begin(9600);
  Serial.println("fingertest");
  mySerial.begin(57600);
  fpm.begin(&mySerial);
}

void loop() {
 
 int s=fpm.SearchPrint();
 Serial.print("returnValue:");
 Serial.println(s);
 if(s>=0)
 {
   Serial.print("Found ID #"); Serial.println(s);
 }
 else
 {
    Serial.print("Error");
    Serial.println(fpm.ErrorMessage); 
 }

 delay(1000);
}
