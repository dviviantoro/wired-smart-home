#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <time-ao.h>
#include <EEPROM.h>
#include <voice-ao.h>

#define EEPROM_SIZE 100
#define RXD2 16
#define TXD2 17

String msg, header, node, page, sendNextion;
String shh1, smm1, shh2, smm2, shh3, smm3, shh4, smm4;

bool flagDir, flagSch1, flagSch2, ena1, ena2, flagLamp1;
int hh1, mm1, hh2, mm2, hh3, mm3, hh4, mm4, sch;
int arr[4] = {0, 0, 0, 0};
bool flagWait = false;

unsigned long previousMillis = 0;
const long interval = 1000;

unsigned long lastMilCheck = 0;
const long intervalCheck = 2000;

struct dataLayout
{
  char nodes[10];
  int outputPins;
  int flags;
  int sch1S;
  int sch1E;
  int sch2S;
  int sch2E;
  int ena1;
  int ena2;
  int inputPins;
};
dataLayout mapping[6]{
    {"lamp1", 32, 91, 11, 14, 15, 18, 19, 20, 19}, // pin=23
    {"lamp2", 25, 92, 21, 24, 25, 28, 29, 30, 18},
    {"lock1", 26, 93, 31, 34, 35, 38, 39, 40, 36},
    {"lock2", 27, 94, 41, 44, 45, 48, 49, 50, 39},
    {"soc1", 2, 95, 51, 54, 55, 58, 59, 60, 14},
    {"soc2", 33, 96, 61, 64, 65, 68, 69, 70, 15},
};

struct dataTime
{
  int hh1;
  int mm1;
  int hh2;
  int mm2;
};
dataTime timeValue;

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define DEVICE_NAME "ESP32 Smart Home"
// https://www.uuidgenerator.net/
#define SERVICE_UUID "c166797b-4a24-439b-972a-167692d0f2fa"
#define COMMAND_CHARACTERISTIC_UUID "fa0673e1-4a0f-45ce-b141-fc7d6fd5ede5"
#define STATUS_CHARACTERISTIC_UUID "0cd4105f-0154-40ef-9a8e-3d4d5f565972"
#define PASSKEY 123456

BLEServer *pServer = NULL;
BLECharacteristic *commandCharacteristics = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

BLECharacteristic statusCharacteristics(STATUS_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor statusDescriptor(BLEUUID((uint16_t)0x2902));

boolean flagSystem = false;

/*
state:
lamp1 = 91
lamp2 = 92
lock1 = 93
lock2 = 94
soc1  = 95
soc2  = 96

s1-hh1lamp1 = 11
s1-mm1lamp1 = 12
e1-hh2lamp1 = 13
e1-mm2lamp1 = 14
s2-hh3lamp1 = 15
s2-mm3lamp1 = 16
e2-hh4lamp1 = 17
e2-mm4lamp1 = 18
ena1 = 19
ena2 = 20

s1-hh1lamp2 = 21
s1-mm1lamp2 = 22
e1-hh2lamp2 = 23
e1-mm2lamp2 = 24
s2-hh3lamp2 = 25
s2-mm3lamp2 = 26
e2-hh4lamp2 = 27
e2-mm4lamp2 = 28
ena1 = 29
ena2 = 30

s1-hh1lock1 = 31
s1-mm1lock1 = 32
e1-hh2lock1 = 33
e1-mm2lock1 = 34
s2-hh3lock1 = 35
s2-mm3lock1 = 36
e2-hh4lock1 = 37
e2-mm4lock1 = 38
ena1 = 39
ena2 = 40

s1-hh1lock2 = 41
s1-mm1lock2 = 42
e1-hh2lock2 = 43
e1-mm2lock2 = 44
s2-hh3lock2 = 45
s2-mm3lock2 = 46
e2-hh4lock2 = 47
e2-mm4lock2 = 48
ena1 = 49
ena2 = 50

s1-hh1soc1 = 51
s1-mm1soc1 = 52
e1-hh2soc1 = 53
e1-mm2soc1 = 54
s2-hh3soc1 = 55
s2-mm3soc1 = 56
e2-hh4soc1 = 57
e2-mm4soc1 = 58
ena1 = 59
ena2 = 60

s1-hh1soc2 = 61
s1-mm1soc2 = 62
e1-hh2soc2 = 63
e1-mm2soc2 = 64
s2-hh3soc2 = 65
s2-mm3soc2 = 66
e2-hh4soc2 = 67
e2-mm4soc2 = 68
ena1 = 69
ena2 = 70


*/