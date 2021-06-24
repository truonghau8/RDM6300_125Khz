#include <SoftwareSerial.h>
#include "RF125Khz.h"
#include "Arduino.h"

RF125KHZ::RF125KHZ(uint8_t rxPin, uint8_t txPin)
{
  _data._check = true;
	init(rxPin, txPin);
}
RF125KHZ::RF125KHZ(uint8_t rxPin, uint8_t txPin, boolean check)
{
  _data._check = check;
	_data._dataLen = 0;
  _data._valid = false;
  _data._chk = 0;
  _data._isAvailable = false;
}

RF125KHZ::~RF125KHZ()
{
}

bool RF125KHZ::isAvailable()
{
  _data._isAvailable = false;
  _data._chk = 0;
  if (_data._dataLen != 0)
  {
    _data._dataLen = 0;
  }
  uint8_t val = 0;
  uint8_t bytesread = 0;

  if (_rfidIO->available() > 0)
  {
    if ((val = _rfidIO->read()) == 0x02)
    {

      while (bytesread < 12)
      {
        //read 10 bytedata + 2 byte checksum
        if (_rfidIO->available() > 0)
        {
          val = _rfidIO->read();
          //check header || footer
          if ((val == 0x03) || (val == 0x02))
          {
            return 0;
            break;
          }

          //convert acii to hex // 'C' in ascii to 0xC
          if ((val >= '0') && (val <= '9'))
          {
            val = val - '0';
          }
          else if ((val >= 'A') && (val <= 'Z'))
          {
            val = val - 'A' + 10;
          }
          //shift 4 bit
          if (bytesread & 1 == 1)
          {
            _data.raw[int(bytesread / 2)] |= val;
          }
          else
          {
            _data.raw[int(bytesread / 2)] = val << 4;
          }
          //checksum valid
          if (bytesread % 2 == 1)
          {
            if ((int(bytesread / 2)) != 5)
            {
              _data._chk ^= _data.raw[int(bytesread / 2)];
              _data._dataLen++;
            }
          }
          bytesread++;
        }
      }
      bytesread = 0;
    }
  }
  return RF125KHZ::checkValid();
}

bool RF125KHZ::isAvailable_in()
{
  _data._isAvailable = false;
  _data._chk = 0;
  if (_data._dataLen != 0)
  {
    _data._dataLen = 0;
  }
  uint8_t val = 0;
  uint8_t bytesread = 0;

  if (Serial.available() > 0)
  {
    if ((val = Serial.read()) == 0x02)
    {

      while (bytesread < 12)
      {
        //read 10 bytedata + 2 byte checksum
        if (Serial.available() > 0)
        {
          val = Serial.read();
          //check header || footer
          if ((val == 0x03) || (val == 0x02))
          {
            return 0;
            break;
          }

          //convert acii to hex // 'C' in ascii to 0xC
          if ((val >= '0') && (val <= '9'))
          {
            val = val - '0';
          }
          else if ((val >= 'A') && (val <= 'Z'))
          {
            val = val - 'A' + 10;
          }
          //shift 4 bit
          if (bytesread & 1 == 1)
          {
            _data.raw[int(bytesread / 2)] |= val;
          }
          else
          {
            _data.raw[int(bytesread / 2)] = val << 4;
          }
          //checksum valid
          if (bytesread % 2 == 1)
          {
            if ((int(bytesread / 2)) != 5)
            {
              _data._chk ^= _data.raw[int(bytesread / 2)];
              _data._dataLen++;
            }
          }
          bytesread++;
        }
      }
      bytesread = 0;
    }
  }
  return RF125KHZ::checkValid();
}

bool RF125KHZ::checkValid()
{
  if ((5 == _data._dataLen) && (_data._chk == _data.raw[5]))
  {
    _data._valid = _data._isAvailable = true;
    clearbuffer();
    return true;
  }
  else
  {
    _data._valid = _data._isAvailable = false;
    return false;
  }
}

RFIDdata RF125KHZ::data()
{
  return _data;
}

void RF125KHZ::init(uint8_t rxPin, uint8_t txPin)
{
  _rfidIO = new SoftwareSerial(rxPin, txPin);
  _rfidIO->begin(9600);

  _data._dataLen = 0;
  _data._valid = false;
  _data._chk = 0;
  _data._isAvailable = false;
}

void RF125KHZ::clearbuffer() {
  if(_data._check)
  {
    while(_rfidIO->available() > 0 || Serial.available()> 0)
    {
      char c;

      if(_rfidIO->available() > 0)
      {
        c = _rfidIO->read();
        
      }
      if(Serial.available()>0)
      {
        c = Serial.read();
      }
    }
  }
  else
  {
    while( Serial.available()> 0)
    {
      char c;
      if(Serial.available()>0)
      {
        c = Serial.read();
      }
    }
  }
  
}
void RF125KHZ::raw2long(unsigned char *_raw, unsigned long *result)
{
  unsigned char tmp[4] ={0x00, 0x00, 0x00, 0x00};
  byte len = strlen((char*)_raw);
  if(len==0) return;
  for(byte i = 0; i < 3; i++)
  {
    tmp[i] = _raw[2 - i];
  }
  *result = *(unsigned long *)tmp;
  return;
}
void RF125KHZ::raw2Str(unsigned char *_raw, char *arr_result)//11 byte char
{
  unsigned long tmp;
  raw2long(_raw, &tmp);
  char ID_array[11];
  ltoa(tmp, ID_array, 10);
  byte len = (byte)strlen(ID_array);
  if(len <10)
  {
    
    byte minus = 10 - len;
    for(char i = 0; i<10; i++)
    {
      if(i< minus)
      {
        arr_result[i] ='0';
        //Serial.println(arr_result[i]);
      }
      else
      {
        arr_result[i] = ID_array[i - minus];
       // Serial.println(arr_result[i]);
      }
    }
  }
  arr_result[10] = '\0';

  /*for(char i = 0; i < 11; i ++)
  {
    Serial.println(ID_array[i]);
  }*/
 // strcpy(arr_result, ID_array);
  return;

}

void RF125KHZ::raw2Str(unsigned long *a, char *arr_result)
{
  
}
void RF125KHZ::s2l(char*str, unsigned long *l_result)
{
   char *pEnd;
    *l_result = strtol(str, &pEnd, 10);
    return;
}

void RF125KHZ::l2a(unsigned long *l, unsigned char *str)
{
  char* tmp = (char*)l;
  for(byte i = 0; i < 3; i++)
  {
    str[i] = tmp[2-i];
  }
  return;
}
void RF125KHZ::swap(char *a, char *b)
{
  char tmp = *a;
  *a=*b;
  *b=tmp;
}