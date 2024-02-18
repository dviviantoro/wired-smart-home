#include <common.h>

unsigned long previousMillis = 0;
const long interval = 1000;

struct dataLayout
{
  char nodes[10];
  int pins;
  int flags;
  int sch1S;
  int sch1E;
  int sch2S;
  int sch2E;
  int ena1;
  int ena2;
};

dataLayout mapping[6]
{
  {"lamp1", 2, 91, 11, 14, 15, 18, 19, 20}, //pin=23
  {"lamp2", 25, 92, 21, 24, 25, 28, 29, 30},
  {"lock1", 26, 93, 31, 34, 35, 38, 39, 40},
  {"lock2", 27, 94, 41, 44, 45, 48, 49, 50},
  {"soc1", 32, 95, 51, 54, 55, 58, 59, 60},
  {"soc2", 33, 96, 61, 64, 65, 68, 69, 70},
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

void copyArr()
{
    PROGMEM int fillArr[] = {hh1, mm1, hh2, mm2};
    memcpy(arr, fillArr, sizeof(fillArr));
}

void readMapNode(String node, int sch)
{
    for(int i=0;i<6;i++)
    {
        if(node==mapping[i].nodes)
        {
            if(sch==0)
            {
                flagDir = EEPROM.read(mapping[i].flags) == 1;
            }
            else if(sch==1)
            {
                ena1 = EEPROM.read(mapping[i].ena1) == 1;
                for(int k=mapping[i].sch1S;k<=mapping[i].sch1E;k++)
                {
                    int s = k-mapping[i].sch1S;
                    String r = String(EEPROM.read(k));
                    if (r.toInt() < 10)
                    {
                        r = "0"+r;
                    }

                    switch (s)
                    {
                        case 0:
                            shh1 = r;
                            break;
                        case 1:
                            smm1 = r;
                            break;
                        case 2:
                            shh2 = r;
                            break;
                        case 3:
                            smm2 = r;
                            break;
                    }
                }
                break;
            }
            else if(sch==2)
            {
                ena2 = EEPROM.read(mapping[i].ena2) == 1;
                for(int k=mapping[i].sch2S;k<=mapping[i].sch2E;k++)
                {
                    int s = k-mapping[i].sch2S;
                    String r = String(EEPROM.read(k));
                    if (r.toInt() < 10)
                    {
                        r = "0"+r;
                    }

                    switch (s)
                    {
                        case 0:
                            shh1 = r;
                            break;
                        case 1:
                            smm1 = r;
                            break;
                        case 2:
                            shh2 = r;
                            break;
                        case 3:
                            smm2 = r;
                            break;
                    }
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
                writeEEPROM(mapping[i].flags, state);
                digitalWrite(mapping[i].pins, state);
                break;
            }
            else if(sch==1)
            {
                if(flagSch1==true)
                {
                    writeEEPROM(mapping[i].ena1, 1);
                    for(int k=mapping[i].sch1S;k<=mapping[i].sch1E;k++)
                    {
                        writeEEPROM(k, arr[k-mapping[i].sch1S]);
                    }
                }
                else
                {
                    writeEEPROM(mapping[i].ena1, 0);
                }
                break;
            }
            else if(sch==2) 
            {
                if(flagSch2==true)
                {
                    writeEEPROM(mapping[i].ena2, 1);
                    for(int k=mapping[i].sch2S;k<=mapping[i].sch2E;k++)
                    {
                        writeEEPROM(k, arr[k-mapping[i].sch2S]);
                    }
                }
                else
                {
                    writeEEPROM(mapping[i].ena2, 0);
                }
                break;
            }
        }
        if(sch==3)
        {
            digitalWrite(mapping[i].pins, mapping[i].flags);
        }
    }
}

void printnextion() {
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
}

void checkSchedule(String node)
{
    printnextion();

    readMapNode(node,1);
    ena1 == true ? Serial2.print("pSwP1.pic=11") : Serial2.print("pSwP1.pic=10"); 
    printnextion();
    ena1 == true ? Serial2.print("flag1.txt=\"1\"") : Serial2.print("flag1.txt=\"0\"");
    printnextion();
    Serial2.print("hStart1.val="+String(shh1.toInt()));
    printnextion();
    Serial2.print("mStart1.val="+String(smm1.toInt()));
    printnextion();
    Serial2.print("hEnd1.val="+String(shh2.toInt()));
    printnextion();
    Serial2.print("mEnd1.val="+String(smm2.toInt()));
    printnextion();
    Serial2.print("tH1.txt=\""+shh1+"\"");
    printnextion();
    Serial2.print("tM1.txt=\""+smm1+"\"");
    printnextion();
    Serial2.print("tH2.txt=\""+shh2+"\"");
    printnextion();
    Serial2.print("tM2.txt=\""+smm2+"\"");
    printnextion();


    readMapNode(node,2);
    ena2 == true ? Serial2.print("pSwP2.pic=11") : Serial2.print("pSwP2.pic=10");
    printnextion();
    ena2 == true ? Serial2.print("flag2.txt=\"1\"") : Serial2.print("flag2.txt=\"0\"");
    printnextion();
    Serial2.print("hStart2.val="+String(shh1.toInt()));
    printnextion();
    Serial2.print("mStart2.val="+String(smm1.toInt()));
    printnextion();
    Serial2.print("hEnd2.val="+String(shh2.toInt()));
    printnextion();
    Serial2.print("mEnd2.val="+String(smm2.toInt()));
    printnextion();
    Serial2.print("tH3.txt=\""+shh1+"\"");
    printnextion();
    Serial2.print("tM3.txt=\""+smm1+"\"");
    printnextion();
    Serial2.print("tH4.txt=\""+shh2+"\"");
    printnextion();
    Serial2.print("tM4.txt=\""+smm2+"\"");
    printnextion();
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
    ena1 == true ? Serial2.print("tSch1Lamp1.txt=\""+shh1+":"+smm1+" - "+shh2+":"+smm2+ "\"") : Serial2.print("tSch1Lamp1.txt=\"no schedule\"");
    printnextion();
    
    readMapNode("lamp1",2);
    ena2 == true ? Serial2.print("tSch2Lamp1.txt=\""+shh1+":"+smm1+" - "+shh2+":"+smm2+ "\"") : Serial2.print("tSch2Lamp1.txt=\"no schedule\"");
    printnextion();

    readMapNode("lamp2",1);
    ena1 == true ? Serial2.print("tSch1Lamp2.txt=\""+shh1+":"+smm1+" - "+shh2+":"+smm2+ "\"") : Serial2.print("tSch1Lamp2.txt=\"no schedule\"");
    printnextion();

    readMapNode("lamp2",2);
    ena2 == true ? Serial2.print("tSch2Lamp2.txt=\""+shh1+":"+smm1+" - "+shh2+":"+smm2+ "\"") : Serial2.print("tSch2Lamp2.txt=\"no schedule\"");
    printnextion();

    readMapNode("lock1",1);
    ena1 == true ? Serial2.print("tSch1Lock1.txt=\""+shh1+":"+smm1+" - "+shh2+":"+smm2+ "\"") : Serial2.print("tSch1Lock1.txt=\"no schedule\"");
    printnextion();
    
    readMapNode("lock1",2);
    ena2 == true ? Serial2.print("tSch2Lock1.txt=\""+shh1+":"+smm1+" - "+shh2+":"+smm2+ "\"") : Serial2.print("tSch2Lock1.txt=\"no schedule\"");
    printnextion();

    readMapNode("lock2",1);
    ena1 == true ? Serial2.print("tSch1Lock2.txt=\""+shh1+":"+smm1+" - "+shh2+":"+smm2+ "\"") : Serial2.print("tSch1Lock2.txt=\"no schedule\"");
    printnextion();

    readMapNode("lock2",2);
    ena2 == true ? Serial2.print("tSch2Lock2.txt=\""+shh1+":"+smm1+" - "+shh2+":"+smm2+ "\"") : Serial2.print("tSch2Lock2.txt=\"no schedule\"");
    printnextion();

    readMapNode("soc1",1);
    ena1 == true ? Serial2.print("tSch1Soc1.txt=\""+shh1+":"+smm1+" - "+shh2+":"+smm2+ "\"") : Serial2.print("tSch1Soc1.txt=\"no schedule\"");
    printnextion();
    
    readMapNode("soc1",2);
    ena2 == true ? Serial2.print("tSch2Soc1.txt=\""+shh1+":"+smm1+" - "+shh2+":"+smm2+ "\"") : Serial2.print("tSch2Soc1.txt=\"no schedule\"");
    printnextion();

    readMapNode("soc2",1);
    ena1 == true ? Serial2.print("tSch1Soc2.txt=\""+shh1+":"+smm1+" - "+shh2+":"+smm2+ "\"") : Serial2.print("tSch1Soc2.txt=\"no schedule\"");
    printnextion();

    readMapNode("soc2",2);
    ena2 == true ? Serial2.print("tSch2Soc2.txt=\""+shh1+":"+smm1+" - "+shh2+":"+smm2+ "\"") : Serial2.print("tSch2Soc2.txt=\"no schedule\"");
    printnextion();
}

void doItOn()
{
    mapNode(node,0,true);
    Serial.print(node);
    Serial.println(" doit ON");
}

void doItOff()
{
    mapNode(node,0,false);
    Serial.print(node);
    Serial.println(" doit OFF");
}

void setSchedule()
{
    for(int i=0; i<6;i++)
    {
        if(EEPROM.read(mapping[i].ena1)==1)
        {
            node = mapping[i].nodes;
            readMapNode(mapping[i].nodes,1);
            Alarm.alarmRepeat(shh1.toInt(),smm1.toInt(),0,doItOn);
            Alarm.alarmRepeat(shh2.toInt(),smm2.toInt(),0,doItOff);
            Serial.print(node);
            Serial.print(" sch1 ");
            Serial.println(shh1+":"+smm1+"-"+shh2+":"+smm2);
        }
        if(EEPROM.read(mapping[i].ena2)==1)
        {
            node = mapping[i].nodes;
            readMapNode(mapping[i].nodes,2);
            Alarm.alarmRepeat(shh1.toInt(),smm1.toInt(),0,doItOn);
            Alarm.alarmRepeat(shh2.toInt(),smm2.toInt(),0,doItOff);
            Serial.print(node);
            Serial.print(" sch2 ");
            Serial.println(shh1+":"+smm1+"-"+shh2+":"+smm2);
        }
        // Serial.println("sini-sini");
        // Serial.print(EEPROM.read(mapping[i].ena1));
        // Serial.println(EEPROM.read(mapping[i].ena2));
    }
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
    initVoice();
    // for(int j=21;j<=28;j++)
    // {
    //     Serial.println(EEPROM.read(j));
    // }
    // checkAll();
    setSchedule();
    mapNode("",3,false);
}

void loop()
{
//   listenVoice();
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        checkAll();
        routineTime();

        listenVoice();
        if(CMDID != 0)
        {
            sch = 0;
            switch (CMDID)
            {
                case 103:
                    mapNode("lamp1", sch, true);
                    break;
                case 104:
                    mapNode("lamp1", sch, false);
                    break;
            } 
        }

        previousMillis = currentMillis;
    }

    if(Serial2.available() > 0 ) {
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
                flagSch1 = true;
                sch = 1;
                hh1 = getValue(msg, ';', 4).toInt();
                mm1 = getValue(msg, ';', 5).toInt();
                hh2 = getValue(msg, ';', 6).toInt();
                mm2 = getValue(msg, ';', 7).toInt();
                
                copyArr();
                mapNode(node, sch, flagSch1);
            }
            else 
            {
                flagSch1 = false;
                sch = 1;
                mapNode(node, sch, flagSch1);
            }

            if (getValue(msg, ';', 8) == "1")
            {
                flagSch2 = true;
                sch = 2;
                hh1 = getValue(msg, ';', 9).toInt();
                mm1 = getValue(msg, ';', 10).toInt();
                hh2 = getValue(msg, ';', 11).toInt();
                mm2 = getValue(msg, ';', 12).toInt();

                copyArr();
                mapNode(node, sch, flagSch2);
            }
            else 
            {
                flagSch2 = false;
                sch = 2;
                mapNode(node, sch, flagSch1);
            }
            ESP.restart(); 
        }
        else if (header == "page")
        {
            page = getValue(msg, ';', 2);
            if (page == "control")
            {
                checkAll();
            }
            else if (page == "schedule")
            {
                node = getValue(msg, ';', 3);
                // checkAll();
                checkSchedule(node);
            }
        }
    }  
}
