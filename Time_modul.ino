double local_mean_sidereal_time(int day, int month, int year, int hour, int minute, int second, double longitude) {
  // Calculate the Julian Day number
  int A = floor(year / 100);
  int B = 2 - A + floor(A / 4);
  double JD = floor(365.25 * (year + 4716)) + floor(30.6001 * (month + 1)) + day + B - 1524.5;
  // Calculate the Greenwich Mean Sidereal Time (GMST)
  double GMST = 6.697374558 + 0.06570982441908 * (JD - 2451545) + 1.00273790935 * (hour + (minute + second / 60.0) / 60.0);
  GMST = fmod(GMST, 24);
  // Calculate the Local Mean Sidereal Time (LMST)
  double LMST = GMST + longitude / 15;
  LMST = fmod(LMST, 24);
  if (LMST < 0) {
    LMST += 24;
  }
  return LMST;
}

char* lmst_to_hhmmss(double lmst) {
  // Calculate the number of hours, minutes, and seconds
  int hours = (int)lmst;
  int minutes = (int)((lmst - hours) * 60);
  int seconds = (int)((((lmst - hours) * 60) - minutes) * 60);
  // Format the HH:MM:SS string
  char hhmmss_re[9];
  sprintf(hhmmss_re, "%02d:%02d:%02d", hours, minutes, seconds);
  return hhmmss_re;
}

// Julian Date kiszámítása
double getJulianDate(int year, int month, int day, int hour, int minute, int second) {
  if (month <= 2) {
    year -= 1;
    month += 12;
  }
  int A = year / 100;
  int B = 2 - A + (A / 4);
  int days = int(365.25 * (year + 4716)) + int(30.6001 * (month + 1)) + day + B - 1524.5;
  double hours = hour + (minute / 60.0) + (second / 3600.0);
  return days + (hours / 24.0);
}

void setRTC(String date){
  static time_t tLast;
  time_t t;
  tmElements_t tm;
  tm.Year = y2kYearToTm(getValue(date,',',0).toInt());
  tm.Month = getValue(date,',',1).toInt();
  tm.Day = getValue(date,',',2).toInt();
  tm.Hour = getValue(date,',',3).toInt();
  tm.Minute = getValue(date,',',4).toInt();
  tm.Second = getValue(date,',',5).toInt();
  t = makeTime(tm);
  rtc.set(t);
  setTime(t);
}

String time_now(){
  time_t t;
  t = now();
  int y = year(t);
  int m = month(t);
  int d = day(t);
  int h = hour(t);
  int mi = minute(t);
  int s = second(t);
  char out[21];
  sprintf(out, "%04d-%02d-%02d %02d:%02d:%02d", y, m, d, h, mi, s);
  return out;
}