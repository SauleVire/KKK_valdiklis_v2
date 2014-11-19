
void printDigits(int digits, char skirtukas)

{
  // utility function for digital clock display: prints preceding colon and leading 0
//  Serial.print("simbolis");
    lcd.print(skirtukas);
  if(digits < 10) lcd.print('0');
  lcd.print(digits);

}
void showAlarmTime(AlarmID_t id)
{
  time_t alarmTime = Alarm.read(id);
  if(alarmTime!= 0)
  {
    if( alarmTime <= SECS_PER_DAY)  
       Serial.print(" Aliarmas, kurio ID ");
    else  
        Serial.print(" once only alarm with ID  ");
    Serial.print(id, DEC);    
    Serial.print(" kartojamas tokiu laiku ");
    digitalClockDisplay(alarmTime);
  }
}
void digitalClockDisplay(time_t t)
{
  // digital clock display of the time
  Serial.print(hour(t));
  printAlarmDigits(minute(t));
  printAlarmDigits(second(t));
  Serial.println();
}
void printAlarmDigits(int digits)
{
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

