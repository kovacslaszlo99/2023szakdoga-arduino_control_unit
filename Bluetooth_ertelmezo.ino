#include <Arduino.h>
#include <math.h>
#include <DS3232RTC.h>
#include <Streaming.h> // https://github.com/janelia-arduino/Streaming
#include <TimeLib.h>
#include <SPI.h>
#include <SD.h>
#include <DHT.h>
#include "BluetoothSerial.h"
#include <FS.h>
#include <EEPROM.h>

hw_timer_t *Timer0_Cfg = NULL;

#define EEPROM_SIZE 36

#define DHTPIN 4
#define DHTTYPE DHT22
#define CAMERA_SHOTTER_PIN 2
#define OT_PERC 300000
#define SOFTWARE_VERSION "1.1"

#define RA_MANUAL_UP_PIN 33
#define RA_MANUAL_DOW_PIN 25
#define DEC_MANUAL_UP_PIN 26
#define DEC_MANUAL_DOW_PIN 0
#define MANUAL_SPEED_1_PIN 27
#define MANUAL_SPEED_2_PIN 14

#define TRACKING_STATUS_LED_PIN 17
#define ERROR_STATUS_LED_PIN 16
#define TRACKING_BUTTON_PIN 36
#define VOLTAGE_SENSOR_PIN 39

//const double PI = 3.14159265358979323846;
const double DR = PI / 180;
const double K1 = 15 * DR * 1.0027379;
// Föld sugara méterben
const double R = 6371009;

float adc_voltage = 0.0;
float in_voltage = 0.0;
float R1 = 30000.0;
float R2 = 7500.0;
float ref_voltage = 3.3;
int adc_value = 0;

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

struct RACoordinate{
   int h;
   int m;
   float s;
};

struct DECCoordinate{
   int d;
   int m;
   float s;
};

DHT dht(DHTPIN, DHTTYPE);
DS3232RTC rtc;
bool first_connect = false;
unsigned long lastMeres = 0;
unsigned long lastCameraShott = 0;
unsigned long lastTrackingButtonPressed = 0;
unsigned long lastErrorLedBlink = 0;
unsigned long lastRA_addOneSec = 0;
int tracking_button_val;
float temp;
float hum;
String obj = "";
String dec = "";
String ra = "";
RACoordinate RAStrucOld = {0,0,0.0};
DECCoordinate DECStrucOld = {90,0,0.0};
RACoordinate RAStrucNew = {0,0,0.0};
DECCoordinate DECStrucNew = {90,0,0.0};
double latitude = 0.0;
double longitude = 0.0;
int utcOffset = 2;
bool tracking = false;
double cameraExpo = 0.0;
int cameraSleep = 0;
int cameraPiece = 0;
bool cameraStatus = false;
int cameraItem = 0;
bool shottSleep = true;
int cameraMirrorLookUpOffset = 0;
String cameraStartTime = "";
double raStepp = 0.0;
double decStepp = 0.0;
bool motorConrolerConnection = false;
bool sdModulConnection = false;
int currentGoto = 1500;
int currentTracking = 1000;
bool trackingDIR = true;
bool raDIR = true;
bool decDIR = true;
bool timeSiftDIR = true;

double RA = 0.0;
double DEC_my = 0.0;

double Mars_RA = 0.0;
double Mars_DEC = 0.0;


float battery = 0.0;

void IRAM_ATTR Timer0_ISR()
{
    digitalWrite(ERROR_STATUS_LED_PIN, !digitalRead(ERROR_STATUS_LED_PIN));
}

void setup() {
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);
  latitude = EEPROM.readDouble(0);
  longitude = EEPROM.readDouble(8);
  utcOffset = EEPROM.readInt(16);
  currentGoto = EEPROM.readInt(20);
  currentTracking = EEPROM.readInt(24);
  trackingDIR = EEPROM.readBool(28);
  raDIR = EEPROM.readBool(29);
  decDIR = EEPROM.readBool(30);
  timeSiftDIR = EEPROM.readBool(31);
  /*
  Serial.print("lat: ");
  Serial.print(latitude);
  Serial.print(" lon: ");
  Serial.print(longitude);
  Serial.print(" utc: ");
  Serial.println(utcOffset);

  Serial.print("currentGoto: ");
  Serial.print(currentGoto);
  Serial.print(" currentTracking: ");
  Serial.print(currentTracking);
  Serial.print(" trackingDIR: ");
  Serial.print(trackingDIR);
  Serial.print(" raDIR: ");
  Serial.print(raDIR);
  Serial.print(" decDIR: ");
  Serial.print(decDIR);
  Serial.print(" timeSiftDIR: ");
  Serial.println(timeSiftDIR);*/

  pinMode(ERROR_STATUS_LED_PIN, OUTPUT);
  Timer0_Cfg = timerBegin(0, 80, true);
  timerAttachInterrupt(Timer0_Cfg, &Timer0_ISR, true);
  timerAlarmWrite(Timer0_Cfg, 50000, true);
  timerAlarmEnable(Timer0_Cfg);
  
  SerialBT.begin("Telescope");
  Serial1.begin(115200);
  while(!motorConrolerConnection){
    sendMessageMotorControl("ping");
    if (Serial1.available()) {
    Serial.println("available data itt");
    String receivedData = Serial1.readStringUntil('\n');
    receivedData.trim();
      if (receivedData.length() > 0) {
        Serial.println(receivedData);
        if(receivedData == "pong"){
          motorConrolerConnection = true;
        }
      }
    }
    delay(100);
  }
  Serial.println("itt");
  timerAlarmDisable(Timer0_Cfg);
  digitalWrite(ERROR_STATUS_LED_PIN, HIGH);
  sendSettingsMotorControl();
  Wire.begin();
  rtc.begin();
  setSyncProvider(rtc.get);
  
  int i = 0;
  while(!sdModulConnection && i < 4){
    sdModulConnection = SD.begin();
    Serial.println("Hiba a SD-kártya inicializálásakor!");
    i++;
  }
  if(!sdModulConnection){
    timerAlarmWrite(Timer0_Cfg, 500000, true);
    timerAlarmEnable(Timer0_Cfg);
  }else{
    Serial.println("SD-kártya sikeresen inicializálva.");
  }
  dht.begin();
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  pinMode(TRACKING_BUTTON_PIN, INPUT_PULLUP);
  pinMode(TRACKING_STATUS_LED_PIN, OUTPUT);
  pinMode(CAMERA_SHOTTER_PIN, OUTPUT);
  cameraShottActive(false);
  pinMode(RA_MANUAL_UP_PIN, OUTPUT);
  setManPin(RA_MANUAL_UP_PIN, false);
  pinMode(RA_MANUAL_DOW_PIN, OUTPUT);
  setManPin(RA_MANUAL_DOW_PIN, false);
  pinMode(DEC_MANUAL_UP_PIN, OUTPUT);
  setManPin(DEC_MANUAL_UP_PIN, false);
  pinMode(DEC_MANUAL_DOW_PIN, OUTPUT);
  setManPin(DEC_MANUAL_DOW_PIN, false);
  pinMode(MANUAL_SPEED_1_PIN, OUTPUT);
  pinMode(MANUAL_SPEED_2_PIN, OUTPUT);
  setSpeed(0);
  tracking_button_val = digitalRead(TRACKING_BUTTON_PIN);
  setLMST_RA();


  /*time_t t;
  t = now();
  int y  = year(t);
  int m = month(t);
  int d = day(t);
  int h = hour(t);
  int mi = minute(t);
  int s = second(t);
  double lmst = local_mean_sidereal_time(d, m, y, h, mi, s, longitude);
  //calculateMoonPosition(lmst, h, mi, s, latitude);
  calculateMarsPosition(lmst, y, m, d, h+utcOffset, mi, s, latitude, longitude);
  Serial.print("Mars RA: ");
  Serial.print(Mars_RA);
  Serial.print(", Mars DEC: ");
  Serial.print(Mars_DEC);
  Serial.println();*/
}

void loop() {
  if(!first_connect){
    if(SerialBT.hasClient()){
      first_connect = true;
      sendBTMessage("get;gps");
      sendBTMessage("get;datetime");
    }
  }

  if(!tracking && millis() - lastRA_addOneSec > 1000){
    lastRA_addOneSec = millis();
    addOneSecRA();
  }

  if(millis() - lastMeres > OT_PERC){
    adat_mentes(SD);
    lastMeres = millis();
  }

  if (SerialBT.available()) {
    String receivedData = SerialBT.readStringUntil('\n');
    receivedData.trim();
    if (receivedData.length() > 0) {
      Serial.println(receivedData);
      messageInterpretative(receivedData);
    }
  }
  if(cameraStatus){
    if(cameraItem < cameraPiece){
      if(shottSleep){
        //if(lastCameraShott + ((cameraExpo + cameraMirrorLookUpOffset + 1)*1000) <= millis()){
        if(millis() - lastCameraShott > ((cameraExpo + cameraMirrorLookUpOffset + 1)*1000)){
          cameraShottActive(false);
          shottSleep = !shottSleep;
          lastCameraShott = millis();
          cameraItem++;
          Serial.println(cameraItem);
          char send[30];
          sprintf(send, "camera;start;%d", cameraItem);
          sendBTMessage(send);
        }
      }else{
        //if(lastCameraShott + (cameraSleep*1000) <= millis()){
        if(millis() - lastCameraShott > (cameraSleep*1000)){
          cameraShottActive(true);
          shottSleep = !shottSleep;
          lastCameraShott = millis();
        }
      }
    }else{
      cameraStatus=!cameraStatus;
      Serial.println("End photo seassion. Count: ");
      Serial.println(cameraItem);
      char send[30];
      sprintf(send, "camera;stop;%d", cameraItem);
      sendBTMessage(send);
      cameraItem = 0;
      shottSleep = true;
    }
  }


  if(digitalRead(TRACKING_BUTTON_PIN) != tracking_button_val && digitalRead(TRACKING_BUTTON_PIN) == HIGH){
    if (millis() - lastTrackingButtonPressed > 200) {
      tracking_button_val = digitalRead(TRACKING_BUTTON_PIN);
      lastTrackingButtonPressed = millis();
      if(!tracking){
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
      tracking = !tracking;
    }
  }else{
    tracking_button_val = digitalRead(TRACKING_BUTTON_PIN);
  }
}