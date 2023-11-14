String getValue(String data, char separator, int index){
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;
  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

int dateComperTo(String d1, String d2) {
  /*
  1 d1>d2
  0 d1=d2
  -1 d1<d2
  */
  int d1_y = getValue(d1, '-', 0).toInt();
  int d1_m = getValue(d1, '-', 1).toInt();
  int d1_d = getValue(d1, '-', 2).toInt();

  int d2_y = getValue(d2, '-', 0).toInt();
  int d2_m = getValue(d2, '-', 1).toInt();
  int d2_d = getValue(d2, '-', 2).toInt();

  if(d1_y > d2_y){
    return 1;
  }
  if(d1_y < d2_y){
    return -1;
  }
  if(d1_m > d2_m){
    return 1;
  }
  if(d1_m < d2_m){
    return -1;
  }
  if(d1_d > d2_d){
    return 1;
  }
  if(d1_d < d2_d){
    return -1;
  }
  return 0;
}

int timeComperTo(String t1, String t2){
  /*
  1 t1>t2
  0 t1=t2
  -1 t1<t2
  */
  int t1_h = getValue(t1, ':', 0).toInt();
  int t1_m = getValue(t1, ':', 1).toInt();
  int t1_s = getValue(t1, ':', 2).toInt();

  int t2_h = getValue(t2, ':', 0).toInt();
  int t2_m = getValue(t2, ':', 1).toInt();
  int t2_s = getValue(t2, ':', 2).toInt();
  if(t1_h > t2_h){
    return 1;
  }
  if(t1_h < t2_h){
    return -1;
  }
  if(t1_m > t2_m){
    return 1;
  }
  if(t1_m < t2_m){
    return -1;
  }
  if(t1_s > t2_s){
    return 1;
  }
  if(t1_s < t2_s){
    return -1;
  }
  return 0;
}

void cameraShottActive(bool active){
  digitalWrite(CAMERA_SHOTTER_PIN, (active)? HIGH : LOW);
}

void trackingLEDActive(bool active){
  digitalWrite(TRACKING_STATUS_LED_PIN, (active)? HIGH : LOW);
}

void coordinateSaveVar(String ra_in, String dec_in){
  RAStrucNew.h = getValue(ra_in,':',0).toInt();
  RAStrucNew.m = getValue(ra_in,':',1).toInt();
  RAStrucNew.s = getValue(ra_in,':',2).toFloat();

  DECStrucNew.d = getValue(dec_in,':',0).toInt();
  DECStrucNew.m = getValue(dec_in,':',1).toInt();
  DECStrucNew.s = getValue(dec_in,':',2).toFloat();
}

String coordinateToString(){
  char out_str[30];
  sprintf(out_str, "%02d:%02d:%.1f, %02d°%02d'%.1f\"", RAStrucNew.h, RAStrucNew.m, RAStrucNew.s, DECStrucNew.d, DECStrucNew.m, DECStrucNew.s);
  return out_str;
}

double timeDecNull(double step){
  return ((1.1936786798576214 * pow(10, -5))) * step + 3.1519765494137424;
}

double timeDecNOTNull(double step){
  double szorzo = 1.2154382827965584;
  double hatvany = pow(10, -5);
  double eltolas = 3.2257849855337284;
  return ((szorzo * hatvany) * step) + eltolas;
}

void GOTOSteppCalculator(){
  time_t t;
  t = now();
  int y  = year(t);
  int m = month(t);
  int d = day(t);
  int h = hour(t);
  int mi = minute(t);
  int s = second(t);
  double lmst = local_mean_sidereal_time(d, m, y, h, mi, s, longitude);
  double inv_lmst = lmst - 12.0 - 6.0;
  if(inv_lmst < 0.0){
    inv_lmst = 24.0 + inv_lmst; 
  }

  double raC = 0.0;
  raC += (RAStrucOld.h - RAStrucNew.h) * 60 * 60;
  raC += (RAStrucOld.m - RAStrucNew.m) * 60;
  raC += (RAStrucOld.s - RAStrucNew.s);
  if(raC > 1.0){
    //plusz irány
    double raTav = 0.0;
    int inv_lmst_h = (int)inv_lmst;
    int inv_lmst_m = (int)((inv_lmst - inv_lmst_h) * 60);
    int inv_lmst_s = (int)((((inv_lmst - inv_lmst_h) * 60) - inv_lmst_m) * 60);
    raTav += (RAStrucOld.h - inv_lmst_h) * 60 * 60;
    raTav += (RAStrucOld.m - inv_lmst_m) * 60;
    raTav += (RAStrucOld.s - inv_lmst_s);
    if(raTav > 0 && abs(raC) > abs(raTav)){
      raC =  -1.0 * ((24.0 * 60 * 60) - raC);
    }
  }else{
    //Serial.println("negaív ág");
    double raTav = 0.0;
    int inv_lmst_h = (int)inv_lmst;
    int inv_lmst_m = (int)((inv_lmst - inv_lmst_h) * 60);
    int inv_lmst_s = (int)((((inv_lmst - inv_lmst_h) * 60) - inv_lmst_m) * 60);
    raTav += (RAStrucOld.h - inv_lmst_h) * 60 * 60;
    raTav += (RAStrucOld.m - inv_lmst_m) * 60;
    raTav += (RAStrucOld.s - inv_lmst_s);
    if(raTav < 0 && abs(raC) > abs(raTav)){
      raC =  ((24.0 * 60 * 60) + raC);
    }
  }

  double decC = 0.0;
  decC += (DECStrucOld.d - DECStrucNew.d) * 60 * 60;
  decC += (DECStrucOld.m - DECStrucNew.m) * 60;
  decC += (DECStrucOld.s - DECStrucNew.s);

  double timesift = 0.0;
  decStepp = decC / 0.311538462;
  raStepp = raC / 0.010384615;
  if(abs(raStepp) > 1.0){
    if(abs(decStepp) > 1.0){
      timesift = timeDecNOTNull(max(abs(raStepp), abs(decStepp)));
    }else{
      timesift = timeDecNull(abs(raStepp));
    }
  }
  //Serial.print("Time sift: ");
  //Serial.println(timesift);
  if(raStepp*((timeSiftDIR)?1.0:-1.0) > 0){
    raC += timesift;
  }else{
    raC -= timesift;
  }
  raStepp = raC / 0.010384615;
}

void sendMessageMotorControl(const char* message){
  Serial1.println(message);
  Serial.println(message);
}

void GOTOSend(double ra_step_loc, double dec_step_loc){
  //Serial.print("RA Step: ");
  //Serial.print(ra_step_loc);
  //Serial.print(" DEC Step: ");
  //Serial.println(dec_step_loc);
  ra_step_loc *= ((raDIR)?1.0:-1.0);
  dec_step_loc *= ((decDIR)?1.0:-1.0);
  char ra_step_str[15];
  dtostrf(ra_step_loc, 4, 0, ra_step_str);
  char dec_step_str[15];
  dtostrf(dec_step_loc, 4, 0, dec_step_str);
  char send[30];
  sprintf(send, "move;%s,%s", ra_step_str, dec_step_str);
  sendMessageMotorControl(send);
}

void setManPin(int pin, bool active){
  digitalWrite(pin, (active)? LOW : HIGH);
}

void setSpeed(int s){
  if(s == 0){
    digitalWrite(MANUAL_SPEED_1_PIN, LOW);
    digitalWrite(MANUAL_SPEED_2_PIN, LOW);
    //Serial.println("speed 0");
  }else if(s == 1){
    digitalWrite(MANUAL_SPEED_1_PIN, HIGH);
    digitalWrite(MANUAL_SPEED_2_PIN, LOW);
    //Serial.println("speed 1");
  }else if(s == 2){
    digitalWrite(MANUAL_SPEED_1_PIN, LOW);
    digitalWrite(MANUAL_SPEED_2_PIN, HIGH);
    //Serial.println("speed 2");
  }else if(s == 3){
    digitalWrite(MANUAL_SPEED_1_PIN, HIGH);
    digitalWrite(MANUAL_SPEED_2_PIN, HIGH);
    //Serial.println("speed 3");
  }
}

void newCoorCOPYoldCoor(){
  RAStrucOld.h = RAStrucNew.h;
  RAStrucOld.m = RAStrucNew.m;
  RAStrucOld.s = RAStrucNew.s;

  DECStrucOld.d = DECStrucNew.d;
  DECStrucOld.m = DECStrucNew.m;
  DECStrucOld.s = DECStrucNew.s;
}

void sendSettingsMotorControl(){
  char sendData1[40];
  sprintf(sendData1, "set;currentgoto;%d", currentGoto);
  sendMessageMotorControl(sendData1);

  char sendData2[40];
  sprintf(sendData2, "set;currenttracking;%d", currentTracking);
  sendMessageMotorControl(sendData2);

  char sendData3[40];
  sprintf(sendData3, "set;trackingdir;%s", ((trackingDIR)?"true":"false"));
  sendMessageMotorControl(sendData3);
}

void setLMST_RA(){
  time_t t;
  t = now();
  int y  = year(t);
  int m = month(t);
  int d = day(t);
  int h = hour(t);
  int mi = minute(t);
  int s = second(t);
  double lmst = local_mean_sidereal_time(d, m, y, h, mi, s, longitude);
  RAStrucOld.h = (int)lmst;
  RAStrucOld.m = (int)((lmst - RAStrucOld.h) * 60);
  RAStrucOld.s = (int)((((lmst - RAStrucOld.h) * 60) - RAStrucOld.m) * 60);

  RAStrucOld.h -= 6;
  if(RAStrucOld.h < 0.0){
    RAStrucOld.h = 24.0 + RAStrucOld.h; 
  }

  RAStrucNew.h = (int)lmst;
  RAStrucNew.m = (int)((lmst - RAStrucNew.h) * 60);
  RAStrucNew.s = (int)((((lmst - RAStrucNew.h) * 60) - RAStrucNew.m) * 60);

  RAStrucNew.h -= 6;
  if(RAStrucNew.h < 0.0){
    RAStrucNew.h = 24.0 + RAStrucNew.h; 
  }

  /*RAStrucOld.h = (int)18;
  RAStrucOld.m = (int)0;
  RAStrucOld.s = (int)0;

  RAStrucNew.h = (int)18;
  RAStrucNew.m = (int)0;
  RAStrucNew.s = (int)0;*/
}

void addOneSecRA(){
  RAStrucOld.s = RAStrucOld.s + 1;
  if(RAStrucOld.s == 60){
    RAStrucOld.s = 0;
    RAStrucOld.m = RAStrucOld.m + 1;
    if(RAStrucOld.m == 60){
      RAStrucOld.m = 0;
      RAStrucOld.h = RAStrucOld.h + 1;
      if(RAStrucOld.h == 24){
        RAStrucOld.h = 0;
      }
    }
  }
}

String atalakitEgiKoordinatatIdore(double egikoordinata) {
  // Ellenőrizzük, hogy az égi koordináta 0 és 360 fok között van-e
  if (egikoordinata < 0 || egikoordinata >= 360) {
    return "HIBA: Égi koordináta érvénytelen!";
  }

  // Átalakítás fokokból órákba, percekbe és másodpercekbe
  int ora = egikoordinata / 15;
  int perc = (egikoordinata - (ora * 15)) * 4;
  int masodperc = 0;

  // Idő formátumának létrehozása (HH:MM:SS)
  String oraStr = (ora < 10) ? "0" + String(ora) : String(ora);
  String percStr = (perc < 10) ? "0" + String(perc) : String(perc);
  String masodpercStr = "00"; // Az égi koordináta csak egészperces pontossággal van megadva, ezért a másodperceket mindig 00-ra állítjuk

  String ido = oraStr + ":" + percStr + ":" + masodpercStr;
  return ido;
}

void printRA(double ra) {
  int hours = ra / 15;
  int minutes = (ra - (hours * 15)) * 4;
  int seconds = (ra - (hours * 15) - (minutes / 4.0)) * 240;
  Serial.print(hours);
  Serial.print(":");
  if (minutes < 10) Serial.print("0");
  Serial.print(minutes);
  Serial.print(":");
  if (seconds < 10) Serial.print("0");
  Serial.print(seconds);
}

void printDEC(double dec) {
  int degrees = int(dec);
  int minutes = (dec - degrees) * 60;
  int seconds = (dec - degrees - (minutes / 60.0)) * 3600;
  Serial.print(degrees);
  Serial.print("°");
  if (minutes < 10) Serial.print("0");
  Serial.print(minutes);
  Serial.print("'");
  if (seconds < 10) Serial.print("0");
  Serial.print(seconds);
  Serial.print("\"");
}

String coordinateString(){
  
}
