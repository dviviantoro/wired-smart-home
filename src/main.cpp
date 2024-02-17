#include <common.h>

struct dataLayout
{
  char a[10];
  int b;
};

dataLayout data[6]
{
  {"lamp1", 23},
  {"lamp2", 25},
  {"lock1", 26},
  {"lock2", 27},
  {"soc1", 32},
  {"soc2", 33},
};

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void mapNode(String str, bool state) {
    for(int i=0;i<6;i++)
    {
        if(str==data[i].a)
        {
            digitalWrite(data[i].b, state);
            Serial.println(data[i].a);    
            Serial.println(data[i].b);
            Serial.println(state);
            break;
        }
    }
}

void setup()
{
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  initRTC();

  for(int i=0;i<6;i++)
  {
    pinMode(data[i].b, OUTPUT);
  }
//   initVoice();
}

void loop()
{
//   listenVoice();
    while(Serial2.available() > 0 ) {
        msg = Serial2.readString();
        header = getValue(msg, ';', 1);

        if (header == "dir")
        {
            node = getValue(msg, ';', 2);
            flagDir = getValue(msg, ';', 3) == "1";
            mapNode(node,flagDir);
        }
        else if (header == "sch")
        {
            node = getValue(msg, ';', 2);
            if (getValue(msg, ';', 3) == "1")
            {
                hh1 = getValue(msg, ';', 4).toInt();
                mm1 = getValue(msg, ';', 5).toInt();
                hh2 = getValue(msg, ';', 6).toInt();
                mm2 = getValue(msg, ';', 7).toInt();
                // save to eeprom
            }
            if (getValue(msg, ';', 8) == "1")
            {
                hh3 = getValue(msg, ';', 9).toInt();
                mm3 = getValue(msg, ';', 10).toInt();
                hh4 = getValue(msg, ';', 11).toInt();
                mm4 = getValue(msg, ';', 12).toInt();
            }
        }
    }

    readRTC();


   
}
