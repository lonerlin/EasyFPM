/***************************************************
  This is an simplified library for the FPM10A sensor
  Based on the brianrho FPM library https://github.com/brianrho/FPM

  Written by LIN<42040834@qq.com> (2017)
  Distributed under the terms of the MIT license
 ****************************************************/
#ifndef EASYFPM_H
#define EASYFPM_H
#include<FPM.h>
#include<SoftwareSerial.h>



class EasyFPM:public FPM
{
    public:
        EasyFPM(uint8_t receivePin, uint8_t transmitPin);
        int SearchPrint();
        String ErrorMessage;

    protected:
       SoftwareSerial mySerial;
    private:
        int getFingerprintID();
};

#endif // EASYFPM_H
