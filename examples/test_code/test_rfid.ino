#include "RF125Khz.h"

RF125KHZ myrf(2, 3);

typedef struct
{
      unsigned long last_value;
      unsigned long curr_value;
      bool run = false;
} timerTick_;
timerTick_ rfid;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}

bool flag = false;
unsigned char test[3]; //= {0xC7, 0xB6, 0x7E};//thevuaroi
bool check(unsigned char *a,unsigned char*b);
void cpy_array(unsigned char *a, unsigned char*b);// b to a


unsigned long count = 0;
void loop() {
  // put your main code here, to run repeatedly:

  if(rfid.run == true)
  {
    if((millis() - rfid.last_value) > 1000)
    {
      count = 0;
      flag = false;
      rfid.run = false;
      digitalWrite(13, 0);
    }
  }

  if(myrf.isAvailable() && !flag)
  {
    Serial.println("aaa");
    cpy_array(test,myrf.data().raw+2);
    flag = true;
  }
  
  if(myrf.isAvailable() && flag)
  {
    if(check(test, myrf.data().raw+2))
    {
      if(!digitalRead(13))
      {
        digitalWrite(13, 1);
      }
      count++;
      Serial.println(count);
      rfid.last_value = millis();
      rfid.run = true;
      
    }
  }

}

void cpy_array(unsigned char *a, unsigned char *b)
{
  for(byte i = 0; i < 3; i++)
  {
    a[i] = b[i];
  }
}

bool check(unsigned char*a,unsigned char*b)
{
  for(byte i =0; i < 2;i++)
  {
    if(*(a+i) != *(b+i))
    {
      return 0;
    }
  }
  return 1;
}
/*
    Serial.println("ok");
      for(byte i = 0; i < 5; i++)
      {
        unsigned char c = myrf.data().raw[i];
        Serial.print(c, HEX);
        Serial.print(" ");
      }
      Serial.println();
     */
