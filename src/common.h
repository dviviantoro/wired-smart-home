#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <time-ao.h>
#include <EEPROM.h>
#include <voice-ao.h>

#define EEPROM_SIZE 100
// #include <rtc-ao.h>
// #include <nextion-ao.h>

String msg,header,node,page,sendNextion;
bool flagDir,flagSch1,flagSch2,ena1,ena2;
String shh1,smm1,shh2,smm2,shh3,smm3,shh4,smm4;
int hh1,mm1,hh2,mm2,hh3,mm3,hh4,mm4,sch;
int arr[4] = {0,0,0,0};


#define RXD2 16
#define TXD2 17

String headerDirect = "dir";
String headerSchedule = "sch";

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