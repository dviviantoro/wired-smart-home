#include <common.h>

struct dataLayout
{
  char nodes[10];
  int pins;
  int addrs;
  int sch1S;
  int sch1E;
  int sch2S;
  int sch2E;
};

dataLayout mapping[6]
{
  {"lamp1", 23, 91, 11, 14, 15, 18},
  {"lamp2", 25, 92, 21, 24, 25, 28},
  {"lock1", 26, 93, 31, 34, 35, 38},
  {"lock2", 27, 94, 41, 44, 45, 48},
  {"soc1", 32, 95, 51, 54, 55, 58},
  {"soc2", 33, 96, 61, 64, 65, 68},
};

struct dataTime
{
    int hh1;
    int mm1;
    int hh2;
    int mm2;
};
dataTime timeValue;

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

void writeEEPROM(int addr, int value)
{
    EEPROM.write(addr, value);
    EEPROM.commit();
}

// void mapNode(String node, bool state) {
//     for(int i=0;i<6;i++)
//     {
//         if(node==mapping[i].nodes)
//         {
//             writeEEPROM(mapping[i].addrs, state);
//             digitalWrite(mapping[i].pins, state);
//             break;
//         }
//     }
// }

void copyArr()
{
    PROGMEM int fillArr[] = {hh1, mm1, hh2, mm2};
    memcpy(arr, fillArr, sizeof(fillArr));
}

void readMapNode(String node, int sch)
{
    sendNextion = "";
    for(int i=0;i<6;i++)
    {
        if(node==mapping[i].nodes)
        {
            if(sch==0)
            {
                flagDir = EEPROM.read(mapping[i].addrs) == 1;
            }
            else if(sch==1)
            {
                for(int k=mapping[i].sch1S;k<=mapping[i].sch1E;k++)
                {
                    int s = k-mapping[i].sch1S;
                    if(s==1 or s==3)
                    {
                        sendNextion += ":";
                    }
                    if(s==2)
                    {
                        sendNextion += "-";
                    }
                    if(EEPROM.read(k) < 10)
                    {
                        sendNextion += "0";    
                    }
                    sendNextion += EEPROM.read(k);
                }
                break;
            }
            else if(sch==2)
            {
                for(int k=mapping[i].sch2S;k<=mapping[i].sch2E;k++)
                {
                    int s = k-mapping[i].sch2S;
                    if(s==1 or s==3)
                    {
                        sendNextion += ":";
                    }
                    if(s==2)
                    {
                        sendNextion += "-";
                    }
                    if(EEPROM.read(k) < 10)
                    {
                        sendNextion += "0";    
                    }
                    sendNextion += EEPROM.read(k);
                }
                break;
            }
        }
    }
}

void mapNode(String node, int sch, bool state)
{
    for(int i=0;i<6;i++)
    {
        if(node==mapping[i].nodes)
        {
            if (sch==0)
            {
                writeEEPROM(mapping[i].addrs, state);
                digitalWrite(mapping[i].pins, state);
                break;
            }
            else if(sch==1)
            {
                for(int k=mapping[i].sch1S;k<=mapping[i].sch1E;k++)
                {
                    writeEEPROM(k, arr[k-mapping[i].sch1S]);
                }
                break;
            }
            else if(sch==2) 
            {
                for(int k=mapping[i].sch2S;k<=mapping[i].sch2E;k++)
                {
                    writeEEPROM(k, arr[k-mapping[i].sch2S]);
                }
                break;
            }
        }
    }
}

void printnextion() {
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
}


void checkAll()
{
    printnextion();

    readMapNode("lamp1",0);
    flagDir == true ? Serial2.print("pSwLamp1.pic=11") : Serial2.print("pSwLamp1.pic=10"); 
    printnextion();

    readMapNode("lamp2",0);
    flagDir == true ? Serial2.print("pSwLamp2.pic=11") : Serial2.print("pSwLamp2.pic=10"); 
    printnextion();

    readMapNode("lock1",0);
    flagDir == true ? Serial2.print("pSwLock1.pic=11") : Serial2.print("pSwLock1.pic=10"); 
    printnextion();

    readMapNode("lock2",0);
    flagDir == true ? Serial2.print("pSwLock2.pic=11") : Serial2.print("pSwLock2.pic=10"); 
    printnextion();

    readMapNode("soc1",0);
    flagDir == true ? Serial2.print("pSwSoc1.pic=11") : Serial2.print("pSwSoc1.pic=10"); 
    printnextion();

    readMapNode("soc2",0);
    flagDir == true ? Serial2.print("pSwSoc2.pic=11") : Serial2.print("pSwSoc2.pic=10"); 
    printnextion();

    readMapNode("lamp1",1);
    Serial2.print("tSch1Lamp1.txt=\"");
    Serial2.print(sendNextion + "\"");
    printnextion();
    
    readMapNode("lamp1",2);
    Serial2.print("tSch2Lamp1.txt=\"");
    Serial2.print(sendNextion + "\"");
    printnextion();

    readMapNode("lamp2",1);
    Serial2.print("tSch1Lamp2.txt=\"");
    Serial2.print(sendNextion + "\"");
    printnextion();

    readMapNode("lamp2",2);
    Serial2.print("tSch2Lamp2.txt=\"");
    Serial2.print(sendNextion + "\"");
    printnextion();

    readMapNode("lock1",1);
    Serial2.print("tSch1Lock1.txt=\"");
    Serial2.print(sendNextion + "\"");
    printnextion();
    
    readMapNode("lock1",2);
    Serial2.print("tSch2Lock1.txt=\"");
    Serial2.print(sendNextion + "\"");
    printnextion();

    readMapNode("lock2",1);
    Serial2.print("tSch1Lock2.txt=\"");
    Serial2.print(sendNextion + "\"");
    printnextion();

    readMapNode("lock2",2);
    Serial2.print("tSch2Lock2.txt=\"");
    Serial2.print(sendNextion + "\"");
    printnextion();

    readMapNode("soc1",1);
    Serial2.print("tSch1Soc1.txt=\"");
    Serial2.print(sendNextion + "\"");
    printnextion();
    
    readMapNode("soc1",2);
    Serial2.print("tSch2Soc1.txt=\"");
    Serial2.print(sendNextion + "\"");
    printnextion();

    readMapNode("soc2",1);
    Serial2.print("tSch1Soc2.txt=\"");
    Serial2.print(sendNextion + "\"");
    printnextion();

    readMapNode("soc2",2);
    Serial2.print("tSch2Soc2.txt=\"");
    Serial2.print(sendNextion + "\"");
    printnextion();

}

void setup()
{
    Serial.begin(9600);
    Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
    Serial2.setTimeout(300); 
    EEPROM.begin(EEPROM_SIZE);
    initRTC();

    for(int i=0;i<6;i++)
    {
        pinMode(mapping[i].pins, OUTPUT);
    }
//   initVoice();
    // for(int j=21;j<=28;j++)
    // {
    //     Serial.println(EEPROM.read(j));
    // }
    checkAll();
}

void loop()
{
//   listenVoice();
    while(Serial2.available() > 0 ) {
        msg = Serial2.readString();
        Serial.println(msg);
        header = getValue(msg, ';', 1);

        if (header == "dir")
        {
            sch = 0;
            node = getValue(msg, ';', 2);
            flagDir = getValue(msg, ';', 3) == "1";
            mapNode(node, sch, flagDir);
        }
        else if (header == "sch")
        {
            node = getValue(msg, ';', 2);
            if (getValue(msg, ';', 3) == "1")
            {
                sch = 1;
                hh1 = getValue(msg, ';', 4).toInt();
                mm1 = getValue(msg, ';', 5).toInt();
                hh2 = getValue(msg, ';', 6).toInt();
                mm2 = getValue(msg, ';', 7).toInt();
                
                copyArr();
                mapNode(node, sch, flagSch1);
            }
            if (getValue(msg, ';', 8) == "1")
            {
                sch = 2;
                hh1 = getValue(msg, ';', 9).toInt();
                mm1 = getValue(msg, ';', 10).toInt();
                hh2 = getValue(msg, ';', 11).toInt();
                mm2 = getValue(msg, ';', 12).toInt();

                copyArr();
                mapNode(node, sch, flagSch2);
            }
        }
        else if (header == "page")
        {
            page = getValue(msg, ';', 2);
            if (page == "control")
            {
                checkAll();
            }
        }
    }

    // readRTC();
    // routineTime();

   
}
