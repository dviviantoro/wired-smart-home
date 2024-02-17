#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <rtc-ao.h>
 // #include <voice.h>
// #include <nextion-ao.h>

String msg,header,node;
bool flagDir,flagSch1,flagSch2;

int hh1,mm1,hh2,mm2,hh3,mm3,hh4,mm4;

#define RXD2 16
#define TXD2 17

String headerDirect = "dir";
String headerSchedule = "sch";

/*
state node = 6




*/