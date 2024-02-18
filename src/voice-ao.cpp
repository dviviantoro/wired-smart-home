#include <voice-ao.h>

DFRobot_DF2301Q_I2C DF2301Q;
const long interval = 100;
unsigned long lastMilSensor = 0;
uint8_t CMDID = 0;

void initVoice()
{
    while (!(DF2301Q.begin()))
    {
        Serial.println("Communication with device failed, please check connection");
        delay(3000);
    }
    Serial.println("Begin ok!");

    DF2301Q.setVolume(4);
    DF2301Q.setMuteMode(0);
    DF2301Q.setWakeTime(15);

    uint8_t wakeTime = 0;
    wakeTime = DF2301Q.getWakeTime();
    Serial.print("wakeTime = ");
    Serial.println(wakeTime);
    DF2301Q.playByCMDID(1);   // Wake-up command
    DF2301Q.playByCMDID(23); // Common word ID
}

void listenVoice()
{
    unsigned long nowMil = millis();

    if (nowMil - lastMilSensor >= interval)
    {
        CMDID = 0;
        CMDID = DF2301Q.getCMDID();
        lastMilSensor = nowMil;
    }
}