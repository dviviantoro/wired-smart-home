#include <time-ao.h>

void MorningAlarm() {
  Serial.println("Tturn light off");
//   digitalWrite(led, LOW);
}
void EveningAlarm() {
  Serial.println("Turn light on");
//   digitalWrite(led, HIGH);
}

void lamp1Start()
{
    Serial.println("lamp1 start active");
}

void lamp1End()
{
    Serial.println("lamp1 end active");
}

void lamp2Start()
{
    Serial.println("lamp2 start active");
}

void lamp2End()
{
    Serial.println("lamp2 end active");
}

void lock1Start()
{
    Serial.println("lock1 start active");
}

void lock1End()
{
    Serial.println("lock1 end active");
}

void lock2Start()
{
    Serial.println("lock2 start active");
}

void lock2End()
{
    Serial.println("lock2 end active");
}

void soc1Start()
{
    Serial.println("soc1 start active");
}

void soc1End()
{
    Serial.println("soc1 end active");
}

void soc2Start()
{
    Serial.println("soc2 start active");
}

void soc2End()
{
    Serial.println("soc2 end active");
}

void printDigits(int digits) {
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void digitalClockDisplay() {
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.println();
}

void initRTC() {
  setSyncProvider(RTC.get);   
  if (timeStatus() != timeSet) 
     Serial.println("Unable to sync with the RTC");
  else
     Serial.println("RTC has set the system time");     

  Alarm.alarmRepeat(8,24,0,MorningAlarm);
  Alarm.alarmRepeat(8,26,0,EveningAlarm);
}

void routineTime() {
  digitalClockDisplay();
  // wait one second between each clock display in serial monitor
  Alarm.delay(1000); 
}