/***************************************************
  This is an simplified library for the FPM10A sensor
  Based on the brianrho FPM library and examples (https://github.com/brianrho/FPM)

  Written by LIN<42040834@qq.com> (2017)
  Distributed under the terms of the MIT license
 ****************************************************/
#include <EasyFPM.h>
#define TEMPLATES_PER_PAGE  256

EasyFPM::EasyFPM():FPM()
{

}
//Empty the template database
bool EasyFPM::EmptyDB()
{
    int p = -1;

    p = emptyDatabase();
    if (p == FINGERPRINT_OK){
      return true;
    }
    else if (p == FINGERPRINT_PACKETRECIEVEERR) {
      ErrorMessage="Communication error!";
      //Serial.print("Communication error!");
      return false;
    }
    else if (p == FINGERPRINT_DBCLEARFAIL) {
      ErrorMessage="Could not clear database!";
      //Serial.println("Could not clear database!");
      return false;
    }
}
//Count stored fingerprints
int EasyFPM::GetTemplateCount()
{
    ErrorMessage="";
    int p = getTemplateCount();
    if (p == FINGERPRINT_OK){
    return templateCount;
    }
  else if (p == FINGERPRINT_PACKETRECIEVEERR)
   {
      ErrorMessage="Communication error!";
    //Serial.println("Communication error!");
    return -1;
   }
  else
  {
      ErrorMessage="Unknown error!";
      // Serial.println("Unknown error!");
      return -1;
  }


}

//Delete fingerprint by ID
bool EasyFPM::DeletePrint(int id)
{
    if(deleteFingerprint(id)==FINGERPRINT_OK)
    {
        return true;
    }else
    {
        return false;
    }
}
//Check whether the input fingerprint matches the template in the database
int EasyFPM::SearchPrint()
{

    //return 100;
    getFingerprintID();

    if(fingerID>=0)
    {
        return fingerID;
    }else
    {
        return -1;
    }

}
//Enroll one fingerprint
//succeed return ID
// fail return -1 and ErrorMessage record the error message
int EasyFPM::Enroll()
{
    int16_t id;

  if (get_free_id(&id))
    getFingerprintEnroll(id);
  else
    ErrorMessage="No free slot in flash library!";
    //Serial.println("No free slot in flash library!");
  if(ErrorMessage=="")
  {
      return id;
  }
  else
  {
      return -1;
  }
}
int EasyFPM::deleteFingerprint(int id)
 {
      int p = -1;
      ErrorMessage="";
      p = deleteModel(id);

      if (p == FINGERPRINT_OK) {
        return p;
      } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
        ErrorMessage="Communication error";
        //Serial.println("Communication error");
        return p;
      } else if (p == FINGERPRINT_BADLOCATION) {
        ErrorMessage="Could not delete in that location";
        //Serial.println("Could not delete in that location");
        return p;
      } else if (p == FINGERPRINT_FLASHERR) {
        ErrorMessage="Error writing to flash";
        //erial.println("Error writing to flash");
        return p;
      } else {
          ErrorMessage="Unknown error";
        //Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
        return p;
      }
}

bool EasyFPM:: get_free_id(int16_t * id){
  int p = -1;
  ErrorMessage="";
  for (int page = 0; page < (capacity / TEMPLATES_PER_PAGE) + 1; page++){
    p = getFreeIndex(page, id);
    switch (p){
      case FINGERPRINT_OK:
        if (*id != FINGERPRINT_NOFREEINDEX){
         // Serial.print("Free slot at ID ");
          Serial.println(*id);
          return true;
        }
      case FINGERPRINT_PACKETRECIEVEERR:
       //Serial.println("Communication error!");
        return false;
      default:
        //Serial.println("Unknown error!");
        return false;
    }
  }
}

int EasyFPM:: getFingerprintEnroll(int id) {

  int p = -1;
  ErrorMessage="";

  //Serial.println("Waiting for valid finger to enroll");
  while (p != FINGERPRINT_OK) {
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
      break;
    case FINGERPRINT_IMAGEFAIL:
      //Serial.println("Imaging error");
      break;
    default:
      //Serial.println("Unknown error");
      break;
    }
  }
  // OK success!

  p = image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
     // Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      ErrorMessage="Image too messy";
     // Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
        ErrorMessage="Communication error";
      //Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
        ErrorMessage="Could not find fingerprint features";
      //Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
        ErrorMessage="Could not find fingerprint features";
      //Serial.println("Could not find fingerprint features");
      return p;
    default:
        ErrorMessage="Unknown error";
      //Serial.println("Unknown error");
      return p;
  }

  //Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = getImage();
  }

  p = -1;
  //Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = getImage();
    switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
     // Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      //Serial.println("Imaging error");
      break;
    default:
      //Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
        ErrorMessage="Image too messy";
      //Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
         ErrorMessage="Communication error";
      //Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
       ErrorMessage="Could not find fingerprint features";
      //Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
         ErrorMessage="Could not find fingerprint features";
      //Serial.println("Could not find fingerprint features");
      return p;
    default:
         ErrorMessage="Unknown error";
      //Serial.println("Unknown error");
      return p;
  }


  // OK converted!
  p = createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
      ErrorMessage="Communication error";
    //Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
      ErrorMessage="Fingerprints did not match";
    //Serial.println("Fingerprints did not match");
    return p;
  } else {
      ErrorMessage="Unknown error";
    //Serial.println("Unknown error");
    return p;
  }

 // Serial.print("ID "); Serial.println(id);
  p = storeModel(id);
  if (p == FINGERPRINT_OK) {
    //Serial.println("Stored!");
    return 0;
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
        ErrorMessage="Communication error";
    //Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
        ErrorMessage="Could not store in that location";
    //Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
      ErrorMessage="Error writing to flash";
   // Serial.println("Error writing to flash");
    return p;
  } else {
       ErrorMessage="Unknown error";
    //Serial.println("Unknown error");
    return p;
  }
}

int EasyFPM:: getFingerprintID() {
  int p = -1;
  ErrorMessage="";
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
  Serial.println();
  // OK converted!
  p = fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    //Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
   // Serial.println("Communication error");
    ErrorMessage="Communication error";
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    ErrorMessage="Did not find a match";
   // Serial.println("Did not find a match");
    return p;
  } else {
    //Serial.println("Unknown error");
    ErrorMessage="Unknown error";
    return p;
  }

  //found a match!
  //Serial.print("Found ID #"); Serial.print(fingerID);
  //Serial.print(" with confidence of "); Serial.println(confidence);
}

