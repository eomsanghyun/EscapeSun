#include "pulse.h"
#include "InfraredTemperature.h"
#include "Bboobboo.h"
#include "checkHeat.h"
#include "BleManager.h"
#include "TimeCheck.h"
#include "StepDetection.h"
#include <CurieBLE.h>
#include "DisplayManager.h"
/** Display **/
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
/*Bluetooth Low Energy*/
// Main Service UUID
BLEService escSunService("19B10000-E8F2-537E-4F6C-D104768A1214"); 
BLECharCharacteristic switch0("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEIntCharacteristic distanceData("19B10002-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEIntCharacteristic emergency("19B10003-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEIntCharacteristic limit_distance("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEIntCharacteristic limit_heart_rate("19B10012-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEIntCharacteristic limit_humidity("19B10013-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEIntCharacteristic limit_temperature("19B10014-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEIntCharacteristic limit_body_heat("19B10015-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLECharacteristic sensorData("19B10006-E8F2-537E-4F6C-D104768A1214",BLERead | BLEWrite, 4);
BLEDevice central;
PulseSensor pulse;
InfraredTemperature infraredTemp;
Bboobboo buzzer;
checkHeat checkheat ;
BleManager blemanager;
TimeCheck currentTime;
StepDetection stepdetect;
int buttonPin = 13;
DisplayManager disManager;
void setup(){
  // put your setup code here, to run once:
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  blemanager = BleManager(escSunService,switch0,sensorData,distanceData,emergency,limit_distance,limit_heart_rate,limit_humidity,limit_temperature,limit_body_heat);
  checkheat.init(&blemanager);
  infraredTemp = InfraredTemperature();
  buzzer = Bboobboo();
  stepdetect = StepDetection();
  stepdetect.init();
  currentTime.init();
  disManager.init(&display);
  // For test
  checkheat.resetTestData();

}
 

const int delayMsec = 100;
long previousMillis = 0;
int tempDelay = 0;
int tempHumidDelay = 0;
void loop()
{  
  ModeHeatScan();
  UpdateDisplay();
}

void UpdateDisplay(){
    disManager.update(checkheat.getDistanceDis(),checkheat.getTempDis(),checkheat.getBodyTempDis(),checkheat.getHumidDis(),checkheat.getEmergencyCode());
}
void ModeHeatScan(){
    blemanager.initInLoop(central,sensorData,distanceData,switch0,emergency,limit_distance,limit_heart_rate,limit_humidity,limit_temperature,limit_body_heat);
    delay(50);
    if (!stepdetect.stepEventsEnabeled){
       stepdetect.updateStepCount();
    }
    if(stepdetect.getStepUpdate()){
      int steps = stepdetect.getLastStepCount();
      blemanager.setDistance(steps);
    }
    
    long currentMillis = millis();
    // if 200ms have passed, check the heart rate measurement:
    if (currentMillis - previousMillis >= 200) {previousMillis = currentMillis;}
    if(tempHumidDelay > 1000){
       checkheat.allcheck(stepdetect);// --> Original MODE
       //checkheat.checkTestData(stepdetect);// --> Test MODE
       tempHumidDelay = 0;
    }
    delay(delayMsec);
    tempHumidDelay += delayMsec;
    tempDelay += delayMsec;
}



