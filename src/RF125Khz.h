#ifndef RF125KHZ_H
#define RF125KHZ_H

//author: Hau Truong
//link repo: https://github.com/truonghau8/RDM6300_125Khz


#include <SoftwareSerial.h>
#include "Arduino.h"

typedef struct
{
  boolean _check;
  boolean _valid;
  boolean _isAvailable;
  uint8_t _dataLen;
  uint8_t _chk;
  unsigned char raw[6];

} RFIDdata;

class RF125KHZ
{
  private:
    SoftwareSerial* _rfidIO;
    RFIDdata _data;

   // boolean checksum();
  public:
    RF125KHZ(uint8_t rxPin, uint8_t txPin, boolean check);
    RF125KHZ(uint8_t rxPin, uint8_t txPin);
    ~RF125KHZ();
    void init(uint8_t rxPin, uint8_t txPin);
    bool isAvailable();
    bool isAvailable_in(); //Serail hardware **chua test
    bool checkValid();
    void clearbuffer();
  //add
    void raw2long(unsigned char *_raw, unsigned long *result);
    void raw2Str(unsigned char *_raw, char *arr_result);
    void raw2Str(unsigned long *a, char *arr_result);
    void s2l(char *str, unsigned long *l_result);
    void l2a(unsigned long *l, unsigned char*str);
    void swap(char *a, char*b);
  //add
    RFIDdata data();
};



#endif //RF125KHZ_H
