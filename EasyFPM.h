/***************************************************
  This is an simplified library for the FPM10A sensor
  Based on the brianrho FPM library and examples (https://github.com/brianrho/FPM)

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
        EasyFPM();
        int SearchPrint();
        int GetTemplateCount();
        int Enroll();
        bool DeletePrint(int id);
        bool EmptyDB();
        String ErrorMessage;

    protected:
      Stream *softwareSerial;
    private:
        int getFingerprintID();
        bool get_free_id(int16_t * id);
        int getFingerprintEnroll(int id);
        int deleteFingerprint(int id);

};

#endif // EASYFPM_H
