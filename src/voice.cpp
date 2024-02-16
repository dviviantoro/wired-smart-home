#include <voice.h>

DFRobot_DF2301Q_I2C DF2301Q;
const long interval = 100;
unsigned long lastMilSensor = 0;

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

    DF2301Q.playByCMDID(23); // Common word ID
}

void listenVoice()
{
    unsigned long nowMil = millis();

    if (nowMil - lastMilSensor >= interval)
    {
        uint8_t CMDID = DF2301Q.getCMDID();
        switch (CMDID)
        {
        case 103: // If the command is “Turn on the light”
            // digitalWrite(Led, HIGH);                                         // Turn on the LED
            Serial.println("received'Turn on the light',command flag'103'"); // Serial transmits "received"Turn on the light",command flag"103""
            break;

        case 104: // If the command is “Turn off the light”
            // digitalWrite(Led, LOW);                                           // Turn off the LED
            Serial.println("received'Turn off the light',command flag'104'"); // The serial transmits "received"Turn off the light",command flag"104""
            break;

        default:
            if (CMDID != 0)
            {
                Serial.print("CMDID = "); // Print command ID
                Serial.println(CMDID);
            }
        }

        lastMilSensor = nowMil;
    }
}