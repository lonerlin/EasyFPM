#include<SoftwareSerial.h>
#include<EasyFPM.h>
SoftwareSerial mySerial(2, 3);
EasyFPM efpm;
bool deleteState=false;
void setup() {
  Serial.begin(9600);
   mySerial.begin(57600);
  if(efpm.begin(&mySerial))Serial.println("Found fingerprint sensor!");
}

void loop() {
  if(!deleteState) {
      if(efpm.EmptyDB()){
          Serial.println("Database Init");
          deleteState=true;
        }else
        {
            Serial.print("Error:");
            Serial.println(efpm.ErrorMessage);
          }
    }

}
