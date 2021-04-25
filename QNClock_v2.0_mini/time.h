
void setStoreTime()
{
  if ( isNeedStoreTime == true )
  {
    isNeedStoreTime = false;

    year = 18;
    seconds = 0;
    date = 01;
    month = 01;

    Serial.print(" SetTime : ");
    Serial.print(hours, DEC);
    Serial.print(":");
    Serial.print(minutes, DEC);
    Serial.print(":");
    Serial.println(seconds, DEC);

    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0x00);
    Wire.write(decToBcd(seconds));
    Wire.write(decToBcd(minutes));
    Wire.write(decToBcd(hours));
    Wire.write(decToBcd(day));
    Wire.write(decToBcd(date));
    Wire.write(decToBcd(month));
    Wire.write(decToBcd(year));
    Wire.endTransmission();
  }
}


//시간설정
// T(설정명령) + 년(00~99) + 월(01~12) + 일(01~31) + 시(00~23) + 분(00~59) + 초(00~59) + 요일(1~7, 일1 월2 화3 수4 목5 금6 토7)
// 예: T1810091300002 (2016년 5월 9일 13시 00분 00초 월요일)
void set3231Date()
{
  year    = (byte) ((Serial.read() - 48) * 10 + (Serial.read() - 48));
  month   = (byte) ((Serial.read() - 48) * 10 + (Serial.read() - 48));
  date    = (byte) ((Serial.read() - 48) * 10 + (Serial.read() - 48));
  hours   = (byte) ((Serial.read() - 48) * 10 + (Serial.read() - 48));
  minutes = (byte) ((Serial.read() - 48) * 10 + (Serial.read() - 48));
  seconds = (byte) ((Serial.read() - 48) * 10 + (Serial.read() - 48));
  day     = (byte) (Serial.read() - 48);

  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0x00);
  Wire.write(decToBcd(seconds));
  Wire.write(decToBcd(minutes));
  Wire.write(decToBcd(hours));
  Wire.write(decToBcd(day));
  Wire.write(decToBcd(date));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));
  Wire.endTransmission();

}
