void adat_mentes(fs::FS &fs){
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  time_t t;
  t = now();
  int y = year(t);
  int m = month(t);
  int d = day(t);
  int h = hour(t);
  int mi = minute(t);
  int s = second(t);
  char date[11];
  sprintf(date, "%02d-%02d-%02d", y, m, d);
  Serial.println(date);
  String file_name =  "/data/" + String(date) + ".txt";
  Serial.println(file_name);
  File file = fs.open(file_name, FILE_APPEND);
  //file = SD.open(file_name, FILE_WRITE);
  if (file != NULL) {
    Serial.print("Writing to file...");
    char time[9];
    sprintf(time, "%02d:%02d:%02d", h, mi, s);
    String txt = String(time) + "," + String(temp) + "," + String(hum) + "," + String(utcOffset);
    /*if(file.size() > 1){
      file.seek(file.size());
    }*/
    Serial.println(txt);
    file.println(txt);
  } else {
    Serial.println("error opening file!");
  }
  file.close();
  Serial.println("done.");
}


void SD_kereses(fs::FS &fs, String c, String o){
  File file = fs.open("/db/" + c + "/" + o + ".txt");
  //file = SD.open();
  if (file) {
    String ra_loc = "";
    String dec_loc = "";
    //Serial.println("TXT fájl megnyitva.");
    int i = 0;
    while (file.available()) {
      String line = file.readStringUntil('\n');
      if(i == 1){
        ra_loc = line.substring(0,line.length());
      }
      if(i == 2){
        dec_loc = line.substring(0,line.length());
      }
      //Serial.println(line);
      i++;
    }
    file.close();
    //Serial.println("TXT fájl bezárva.");
    coordinateSaveVar(ra_loc, dec_loc);
  } else {
    Serial.println("Hiba a TXT fájl megnyitásakor!");
  }
}

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    File root = fs.open(dirname);
    if(!root){
        Serial.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return;
    }
    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            //Serial.print("  DIR : ");
            //Serial.println(file.name());
            if(levels){
                listDir(fs, file.path(), levels -1);
            }
        } else {
            char file_path[40];
            sprintf(file_path, "%s/%s", dirname, file.name());
            readFile(SD, file_path);
        }
        file = root.openNextFile();
    }
}

void readFile(fs::FS &fs, const char * path){
    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }
    String date = getValue(path,'/',2);
    date.replace(".txt", "");
    while(file.available()){
        String line = file.readStringUntil('\n');
        String send_data = "data;" + date;
        send_data += " " + line;
        char send_str[send_data.length() + 1];
        send_data.toCharArray(send_str, send_data.length() + 1);
        sendBTMessage(send_str);
    }
    file.close();
}

void listDirDateTime(fs::FS &fs, const char *dirname, uint8_t levels, String date, String time) {
  File root = fs.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      //Serial.print("  DIR : ");
      //Serial.println(file.name());
      if (levels) {
        listDir(fs, file.path(), levels - 1);
      }
    } else {
      String date_file = getValue(file.name(), '.', 0);
      if(dateComperTo(date_file, date) >= 0){
        char file_path[40];
        sprintf(file_path, "%s/%s", dirname, file.name());
        if(dateComperTo(date_file, date) > 0){
          readFile(SD, file_path);
        }else{
          readFileTime(SD, file_path, time);
        }
      }
    }
    file = root.openNextFile();
  }
}

void readFileTime(fs::FS &fs, const char *path, String time) {
  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  String date = getValue(path, '/', 2);
  date.replace(".txt", "");
  while (file.available()) {
    String line = file.readStringUntil('\n');
    String time_line = getValue(line, ',', 0);
    if(timeComperTo(time_line, time) > 0){
      String send_data = "data;" + date;
      send_data += " " + line;
      char send_str[send_data.length() + 1];
      send_data.toCharArray(send_str, send_data.length() + 1);
      sendBTMessage(send_str);
    }
  }
  file.close();
}