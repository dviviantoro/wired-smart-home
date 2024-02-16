#include <Arduino.h>
#include <voice.h>

void setup()
{
  Serial.begin(9600);
  initVoice();
}

void loop()
{
  listenVoice();
}
