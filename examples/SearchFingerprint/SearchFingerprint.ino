
#include<EasyFPM.h>
EasyFPM efpm;
SoftwareSerial mySerial(2, 3);
void setup() {

  Serial.begin(9600);
  Serial.println("fingertest");
  mySerial.begin(57600);
  efpm.begin(&mySerial);
}

void loop() {

 int s=efpm.SearchPrint();
 Serial.print("returnValue:");
 Serial.println(s);
 if(s>=0)
 {
   Serial.print("Found ID #"); Serial.println(s);
 }
 else
 {
    Serial.print("Error");
    Serial.println(efpm.ErrorMessage);
 }

 delay(1000);
}
