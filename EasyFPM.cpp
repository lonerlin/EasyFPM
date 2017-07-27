#include <EasyFPM.h>


EasyFPM::EasyFPM(uint8_t receivePin, uint8_t transmitPin)
{
    mySerial(2, 3);
    mySerial.begin(57600);
    if(!begin(&mySerial))ErrorMessage="Did not find fingerprint sensor ";
}

int EasyFPM::SearchPrint()
{
    if(getFingerprintID()==FINGERPRINT_OK)
    {
        return fingerID;
    }else
    {
        return -1;
    }
}

int EasyFPM:: getFingerprintID() {
  int p = -1;
  //Serial.println("Waiting for a finger...");
  while (p != FINGERPRINT_OK){
    p = getImage();
    switch (p) {
      case FINGERPRINT_OK:
        //Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        //Serial.println(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        //Serial.println("Communication error");
        ErrorMessage="Get Fingerprint ID Communication error";
        break;
      case FINGERPRINT_IMAGEFAIL:
        //Serial.println("Imaging error");
         ErrorMessage="Get Fingerprint ID Imaging error";
        break;
      default:
        //Serial.println("Unknown error");
         ErrorMessage="Get Fingerprint ID Unknown error";
        break;
    }
  }

  // OK success!

  p = image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      //Serial.println("Image too messy");
      ErrorMessage="Image too messy";
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
       ErrorMessage="Communication error";
      return p;
    case FINGERPRINT_FEATUREFAIL:
      //Serial.println("Could not find fingerprint features");
      ErrorMessage="Could not find fingerprint features";
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      //Serial.println("Could not find fingerprint features");
       ErrorMessage="Could not find fingerprint features";
      return p;
    default:
      //Serial.println("Unknown error");
       ErrorMessage="Unknown error";
      return p;
  }

  //Serial.println("Remove finger...");
  while (p != FINGERPRINT_NOFINGER){
    p = getImage();
  }
 // Serial.println();
  // OK converted!
  p = fingerFastSearch();
  if (p == FINGERPRINT_OK) {
   // Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial.println("Communication error");
    ErrorMessage="Communication error";
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    ErrorMessage="Did not find a match";
    //Serial.println("Did not find a match");
    return p;
  } else {
    //Serial.println("Unknown error");
    ErrorMessage="Unknown error";
    return p;
  }

  // found a match!
  //Serial.print("Found ID #"); Serial.print(finger.fingerID);
  //Serial.print(" with confidence of "); Serial.println(finger.confidence);
}

