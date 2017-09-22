#include "pulse.h"
#include "InfraredTemperature.h"
#include "Bboobboo.h"
#include "checkHeat.h"
#include "BleManager.h"
#include "TimeCheck.h"
#include "StepDetection.h"
#include <CurieBLE.h>

/*Bluetooth Low Energy*/
// Main Service UUID
BLEService escSunService("19B10000-E8F2-537E-4F6C-D104768A1214"); 
BLECharCharacteristic switch0("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEIntCharacteristic distanceData("19B10002-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEIntCharacteristic emergency("19B10003-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEIntCharacteristic limit_distance("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEIntCharacteristic limit_heart_rate("19B10012-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEIntCharacteristic limit_humidity("19B10013-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
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

void setup() {
  // put your setup code here, to run once:
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  blemanager = BleManager(escSunService,switch0,sensorData,distanceData,emergency,limit_distance,limit_heart_rate,limit_humidity);
  checkheat.init(&blemanager);
  infraredTemp = InfraredTemperature();
  buzzer = Bboobboo();
  stepdetect = StepDetection();
  stepdetect.init();
  currentTime.init();

  // For test
  checkheat.resetTestData();
}
 

// 심박 수 측정을 위한 delay, loop()에서 전체적인 delay의 틀
const int delayMsec = 100;
// 심박 수를 마지막으로 측정한 시간
long previousMillis = 0;

// 온도 체크 딜레이
int tempDelay = 0;
int tempHumidDelay = 0;
void loop()
{  

  blemanager.initInLoop(central,sensorData,distanceData,switch0,emergency,limit_distance,limit_heart_rate,limit_humidity);

  if (!stepdetect.stepEventsEnabeled) {
     stepdetect.updateStepCount();
  }
   
  long currentMillis = millis();
  // if 200ms have passed, check the heart rate measurement:
  if (currentMillis - previousMillis >= 200) {
    previousMillis = currentMillis;
  }
 

  if(tempHumidDelay > 1000){
     checkheat.allcheck(stepdetect); // --> Original MODE
     //checkheat.checkTestData();    // --> Test MODE
     tempHumidDelay = 0;
  }
    
   
 delay(delayMsec);
 
 tempHumidDelay += delayMsec;
 tempDelay += delayMsec;
  
}

