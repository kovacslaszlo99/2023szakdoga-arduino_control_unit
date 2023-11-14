void messageInterpretative(String message){
  String tag = getValue(message,';',0);
  if(tag == "get"){
    messageGet(message);
  }else if(tag == "datetime"){
    String date = getValue(message,';',1);
    setRTC(date);
    setLMST_RA();
  }else if(tag == "gps"){
    String gps = getValue(message,';',1);
    Serial.print("Be állitom a GPS koordinátákat és az UTC eltolást erre: ");
    Serial.println(gps);
    latitude = getValue(gps,',',0).toDouble();
    longitude = getValue(gps,',',1).toDouble();
    utcOffset = getValue(gps,',',2).toInt();
    Serial.print("latitude: ");
    Serial.print(latitude);
    Serial.print(" longitude: ");
    Serial.print(longitude);
    Serial.print(" utcOffset: ");
    Serial.println(utcOffset);
    EEPROM.writeDouble(0, latitude);
    EEPROM.writeDouble(8, longitude);
    EEPROM.writeInt(16, utcOffset);
    EEPROM.commit();
    setLMST_RA();
  }else if(tag == "obj"){
    setObjMessage(message);
  }else if(tag == "tracking"){
    String tracking_str = getValue(message,';',1);
    tracking = tracking_str == "on";
    if(tracking){
      char send[30];
      sprintf(send, "tracking;on");
      sendMessageMotorControl(send);
      trackingLEDActive(true);
    }else{
      char send[30];
      sprintf(send, "tracking;off");
      sendMessageMotorControl(send);
      trackingLEDActive(false);
    }
  }else if(tag == "camera"){
    String camera_tag = getValue(message,';',1);
    Serial.print("---");
    Serial.println(camera_tag);
    if(camera_tag == "expo"){
      String expo_str = getValue(message,';',2);
      double expo_tmp = expo_str.toDouble();
      if(cameraExpo != expo_tmp){
        cameraItem = 0;
      }
      cameraExpo = expo_tmp;
      Serial.print("expo-");
      Serial.println(expo_str);
    }else if(camera_tag == "sleep"){
      String sleep_str = getValue(message,';',2);
      cameraSleep = sleep_str.toInt();
    }else if(camera_tag == "piece"){
      String piece_str = getValue(message,';',2);
      int piece_tmp = piece_str.toInt();
      if(cameraPiece != piece_tmp){
        cameraItem = 0;
      }
      cameraPiece = piece_tmp;
    }else if(camera_tag == "start"){
      cameraStatus = true;
      cameraShottActive(true);
      lastCameraShott = millis();
      shottSleep = true;
      Serial.println("Start photo session.");
    }else if(camera_tag == "stop"){
      cameraStatus = false;
      cameraShottActive(false);
      Serial.println("Stop photo session.");
    }else if(camera_tag == "mirrorlookup"){
      String offset_str = getValue(message,';',2);
      int offset_tmp = offset_str.toInt();
      cameraMirrorLookUpOffset = offset_tmp;
    }else if(camera_tag == "starttime"){
      cameraStartTime = getValue(message,';',2);
    }
  }else if(tag == "manual"){
    String man_tag = getValue(message,';',1);
    String man_tag2 = getValue(man_tag,',',0);
    if(man_tag2 == "ra"){
      String irany = getValue(man_tag,',',1);
      bool on = (getValue(man_tag,',',2) == "on")? true : false;
      if(irany == "up"){
        setManPin(RA_MANUAL_UP_PIN, on);
      }else if(irany == "down"){
        setManPin(RA_MANUAL_DOW_PIN, on);
      }
    }else if(man_tag2 == "dec"){
      String irany = getValue(man_tag,',',1);
      bool on = (getValue(man_tag,',',2) == "on")? true : false;
      if(irany == "up"){
        setManPin(DEC_MANUAL_UP_PIN, on);
      }else if(irany == "down"){
        setManPin(DEC_MANUAL_DOW_PIN, on);
      }
    }else if(man_tag2 == "speed"){
      int speed = getValue(man_tag,',',1).toInt();
      setSpeed(speed);
    }
  }else if(tag == "set"){
    String set_tag = getValue(message,';',1);
    if(set_tag == "currentgoto"){
      currentGoto = getValue(message,';',2).toInt();
      EEPROM.writeInt(20, currentGoto);
      EEPROM.commit();
      char sendData1[40];
      sprintf(sendData1, "set;currentgoto;%d", currentGoto);
      sendMessageMotorControl(sendData1);
    }else if(set_tag == "currenttracking"){
      currentTracking = getValue(message,';',2).toInt();
      EEPROM.writeInt(24, currentTracking);
      EEPROM.commit();
      char sendData2[40];
      sprintf(sendData2, "set;currenttracking;%d", currentTracking);
      sendMessageMotorControl(sendData2);
    }else if(set_tag == "trackingdir"){
      trackingDIR = (getValue(message,';',2) == "true")? true : false;
      EEPROM.writeBool(28, trackingDIR);
      EEPROM.commit();
      char sendData3[40];
      sprintf(sendData3, "set;trackingdir;%s", ((trackingDIR)?"true":"false"));
      sendMessageMotorControl(sendData3);
    }else if(set_tag == "radir"){
      raDIR = (getValue(message,';',2) == "true")? true : false;
      EEPROM.writeBool(29, raDIR);
      EEPROM.commit();
    }else if(set_tag == "decdir"){
      decDIR = (getValue(message,';',2) == "true")? true : false;
      EEPROM.writeBool(30, decDIR);
      EEPROM.commit();
    }else if(set_tag == "timesiftdir"){
      timeSiftDIR = (getValue(message,';',2) == "true")? true : false;
      EEPROM.writeBool(31, timeSiftDIR);
      EEPROM.commit();
    }
  }else if(tag == "version"){
    char send[40];
    sprintf(send, "Version: %s", SOFTWARE_VERSION);
    sendBTMessage(send);
  }
}

void sendBTMessage(const char* message){
  SerialBT.println(message);
  Serial.println(message);
}

void messageGet(String message){
  String tag = getValue(message,';',1);
  if(tag == "data"){
    String date_type = getValue(message,';',2);
    if(date_type == "all"){
      Serial.println("Összes adat küldése");
      listDir(SD, "/data", 0);
    }else{
      Serial.print(date_type);
      String date_s = getValue(date_type,' ',0);
      String time_s = getValue(date_type,' ',1);
      Serial.println("-tól rögzitet adatok küldése");
      listDirDateTime(SD, "/data", 0, date_s, time_s);
    }
  }else if(tag == "battery"){
    adc_value = analogRead(VOLTAGE_SENSOR_PIN);
    adc_voltage  = (adc_value * ref_voltage) / 4095.0;
    in_voltage = adc_voltage / (R2 / (R1 + R2));
    in_voltage += 0.6;
    battery = in_voltage;
    char battery_str[4];
    dtostrf(battery, 4, 1, battery_str);
    char send[15];
    sprintf(send, "battery;%s", battery_str);
    sendBTMessage(send);
  }else if(tag == "obj"){
    if(obj != ""){
      int obj_len = obj.length() + 1;
      char send[30] = "obj;";
      char obj_str[obj_len];
      obj.toCharArray(obj_str, obj_len);
      strcat(send, obj_str);
      sendBTMessage(send);
    }
  }else if(tag == "coordinata"){
    char send[50] = "coordinate;";
    String coor = coordinateToString();
    int coor_len = coor.length() + 1;
    char coor_str[coor_len];
    coor.toCharArray(coor_str, coor_len);
    strcat(send, coor_str);
    sendBTMessage(send);
  }else if(tag == "temp"){
    char temp_str[4];
    dtostrf(temp, 4, 1, temp_str);
    char send[15];
    sprintf(send, "temp;%s", temp_str);
    sendBTMessage(send);
  }else if(tag == "hum"){
    char hum_str[4];
    dtostrf(hum, 4, 1, hum_str);
    char send[15];
    sprintf(send, "hum;%s", hum_str);
    sendBTMessage(send);
  }else if(tag == "datetime"){
    String time_now_str = time_now();
    int datetime_len = time_now_str.length() + 1;
    char send[30] = "datetime;";
    char datetime_str[datetime_len];
    time_now_str.toCharArray(datetime_str, datetime_len);
    strcat(send, datetime_str);
    sendBTMessage(send);
  }else if(tag == "tracking"){
    if(tracking){
      char send[30] = "tracking;on";
      sendBTMessage(send);
    }else{
      char send[30] = "tracking;off";
      sendBTMessage(send);
    }
  }else if(tag == "camera,expo"){
    if(cameraExpo != 0){
      char temp_str[4];
      dtostrf(cameraExpo, 4, 1, temp_str);
      char send[30];
      sprintf(send, "camera;expo;%s", temp_str);
      sendBTMessage(send);
    }
  }else if(tag == "camera,sleep"){
    if(cameraSleep != 0){
      char send[30];
      sprintf(send, "camera;sleep;%d", cameraSleep);
      sendBTMessage(send);
    }
  }else if(tag == "camera,piece"){
    if(cameraPiece != 0){
      char send[30];
      sprintf(send, "camera;piece;%d", cameraPiece);
      sendBTMessage(send);
    }
  }else if(tag == "camera,status"){
    if(cameraStatus){
      char send[30];
      sprintf(send, "camera;start;%d", cameraItem);
      sendBTMessage(send);
    }else{
      char send[30];
      sprintf(send, "camera;stop;%d", cameraItem);
      sendBTMessage(send);
    }
  }else if(tag == "camera,mirrorlookup"){
    char send[30];
    sprintf(send, "camera;mirrorlookup;%d", cameraMirrorLookUpOffset);
    sendBTMessage(send);
  }else if(tag == "camera,starttime"){
    if(cameraStartTime.length() != 0){
      int cameraStartTime_len = cameraStartTime.length() + 1;
      char send[40] = "camera;starttime;";
      char cameraStartTime_str[cameraStartTime_len];
      cameraStartTime.toCharArray(cameraStartTime_str, cameraStartTime_len);
      strcat(send, cameraStartTime_str);
      sendBTMessage(send);
    }
  }else if(tag == "currentgoto"){
    char send[40];
    sprintf(send, "currentgoto;%d", currentGoto);
    sendBTMessage(send);
  }else if(tag == "currenttracking"){
    char send[40];
    sprintf(send, "currenttracking;%d", currentTracking);
    sendBTMessage(send);
  }else if(tag == "trackingdir"){
    char send[40];
    sprintf(send, "trackingdir;%s", (trackingDIR)?"true":"false");
    sendBTMessage(send);
  }else if(tag == "radir"){
    char send[40];
    sprintf(send, "radir;%s", (raDIR)?"true":"false");
    sendBTMessage(send);
  }else if(tag == "decdir"){
    char send[40];
    sprintf(send, "decdir;%s", (decDIR)?"true":"false");
    sendBTMessage(send);
  }else if(tag == "timesiftdir"){
    char send[40];
    sprintf(send, "timesiftdir;%s", (timeSiftDIR)?"true":"false");
    sendBTMessage(send);
  }
}

void setObjMessage(String message){
  String tag = getValue(message,';',1);
  String tag2 = getValue(tag,',',0);
  if(tag2 == "coor"){
    String ra_loc = getValue(tag,',',1);
    String dec_loc = getValue(tag,',',2);
    Serial.print("Be állitom a koordinátákat erre: ");
    Serial.print(ra_loc);
    Serial.print(" ");
    Serial.println(dec_loc);
    coordinateSaveVar(ra_loc, dec_loc);
    GOTOSteppCalculator();
    GOTOSend(raStepp, decStepp);
    newCoorCOPYoldCoor();
  }else{
    String catalog = getValue(tag,',',0);
    String objNUM = getValue(tag,',',1);
    obj = tag;
    SD_kereses(SD, catalog, objNUM);
    GOTOSteppCalculator();
    GOTOSend(raStepp, decStepp);
    newCoorCOPYoldCoor();
  }
}