#include "BleManager.h"
#include "arduino.h"
BleManager::BleManager(){
  // Default Constructor
}
BleManager::BleManager(
    BLEService escSunService,
    BLECharCharacteristic switch0,
    BLECharacteristic sensorData,
    BLEIntCharacteristic distance,
    BLEIntCharacteristic emergency,
    BLEIntCharacteristic limit_distance,
    BLEIntCharacteristic limit_heart_rate,
    BLEIntCharacteristic limit_humidity,
    BLEIntCharacteristic limit_temperature,
    BLEIntCharacteristic limit_body_heat
  ){
  MAX = 10; cnt = 0; 
  TEMPERATURE = 0;
  BODYHEAT = 1;
  HEARTRATE = 2;
  HUMIDITY = 3;
  for(int i = 0 ; i < SENSOR_VAL_NUM ; i++)sensor_val[i] = 0;

  // BLE object init in 'CurieBLE.h' library
  //Serial.print("Ble init start..");
  BLE.begin(); 
  //Serial.println("..Finish");

  // Ble's Tag name
  //Serial.print("..init device name");
  BLE.setLocalName("SUN"); 
  //Serial.println("..Finish");

  //Serial.print("..init service and Characteristic..");
  BLE.setAdvertisedService(escSunService); // set the Service
  escSunService.addCharacteristic(switch0); // add Characteristic in the Service
  escSunService.addCharacteristic(sensorData);
  escSunService.addCharacteristic(distance);
  escSunService.addCharacteristic(emergency);
  escSunService.addCharacteristic(limit_heart_rate);
  escSunService.addCharacteristic(limit_humidity);
  escSunService.addCharacteristic(limit_distance);
  escSunService.addCharacteristic(limit_temperature);
  escSunService.addCharacteristic(limit_body_heat);
  BLE.addService(escSunService);
  //Serial.println("..Finish");

  //Serial.print("..init Characteristic value..");
  switch0.setValue(0);
  distance.setValue(0);
  emergency.setValue(0);
  limit_distance.setValue(0);
  limit_heart_rate.setValue(0);
  limit_humidity.setValue(0);
  limit_temperature.setValue(0);
  limit_body_heat.setValue(0);
  const unsigned char values[] = {
    (const unsigned char)sensor_val[0],
    (const unsigned char)sensor_val[1],
    (const unsigned char)sensor_val[2],
    (const unsigned char)sensor_val[3]
  };
  sensorData.setValue(values,SENSOR_VAL_NUM);
  //Serial.println("..Finish");

  //Serial.print("..init BLE advertise...");
  BLE.advertise();
  //Serial.println("..Finish");
  
  IsDataChanged = false;
  mode = 0;
  limit_distance_value = 0;
  limit_heart_rate_value = 0;
  limit_humidity_value = 0;
  limit_temperature_value = 0;
  limit_body_heat_value = 0;
  distance_val = 0;
  temp_distance_val = 0;
  emergency_val = 0;
  IsDataChanged = false;
  IsEmergencyChanged = false;
  Serial.println("Escape Sun Ble Manager Peripheral setup finish!");
  buzzer = Bboobboo();
}

void BleManager::setIntSensorValue(int index, int value){
  // INDEX
  // 0 : Temp
  // 1 : Body temp
  // 2 : Heart
  // 3 : Humidity
  if(index >= 0 && index < SENSOR_VAL_NUM){
    sensor_val[index] = value;
    IsDataChanged = true;
  }
}

void BleManager::initInLoop(
    BLEDevice _central,
    BLECharacteristic sensorData,
    BLEIntCharacteristic distance,
    BLECharCharacteristic mode_switch,
    BLEIntCharacteristic emergency,
    BLEIntCharacteristic limit_distance,
    BLEIntCharacteristic limit_heart_rate,
    BLEIntCharacteristic limit_humidity,
    BLEIntCharacteristic limit_temperature,
    BLEIntCharacteristic limit_body_heat
  ){

  // this is a testing module ++++++++++++++++++++++++++++++++++++++++++++++++++
  if(sensorData.written()){
    /*
    Serial.println("Read Sensor BLE value");
    const byte* data = sensorData.value(); 
    Serial.println((int)data[0]);
    Serial.println((int)data[1]);
    Serial.println((int)data[2]);
    Serial.println((int)data[3]);
    */
    Serial.println("----> Debugging --> Sensor written!");
    const unsigned char* data = sensorData.value();
    for(int i = 0 ; i < SENSOR_VAL_NUM ; i++){
      sensor_val[i] = (const unsigned char)data[i];
      IsDataChanged = true;
    }
  }// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  if(IsDataChanged){
    IsDataChanged = false;
    _central = BLE.central();
    const unsigned char values[] = {
      (const unsigned char)(sensor_val[0]),
      (const unsigned char)(sensor_val[1]),
      (const unsigned char)(sensor_val[2]),
      (const unsigned char)(sensor_val[3])
    };
    sensorData.setValue(values,SENSOR_VAL_NUM);
    Serial.println("---------------> Sensor Data Updated!");

  }


  if(IsEmergencyChanged){
    IsEmergencyChanged = false;
    Serial.print("FINAL EMERGENCY CODE :");
    Serial.println(emergency_val);
    emergency.setValue(emergency_val);
    //emergency_val = 0;
  }

  if(IsDistanceChanged){
    IsDistanceChanged = false;
    distance.setValue(distance_val);
  }

  if(mode_switch.written()){
    mode = (int)mode_switch.value();
    if(mode < 0 || mode > 15){
      // Error?
    }else{
      int temp = mode;
      int idx = 0;
      while(idx <= 3){
        function_switch[idx] = temp % 2;
        idx++;
        temp = (int)(temp/2);
      }
    }
  }

  if(limit_distance.written()){
    limit_distance_value = limit_distance.value();
    Serial.print("distance_value changed : ");
    Serial.println(limit_distance_value);
  }
  
  /*
  if(limit_heart_rate.written()){
    limit_heart_rate_value = limit_heart_rate.value();
    Serial.print("heart_rate_value changed : ");
    Serial.println(limit_heart_rate_value);
  }
  */

  if(limit_humidity.written()){
    limit_humidity_value = limit_humidity.value();
    Serial.print("humidity_value changed : ");
    Serial.println(limit_humidity_value);
  }
  if(limit_temperature.written()){
    limit_temperature_value = limit_temperature.value();
    Serial.print("temperature_value changed : ");
    Serial.println(limit_temperature_value);
  }
  if(limit_body_heat.written()){
    limit_body_heat_value = limit_body_heat.value();
    Serial.print("body_heat_value changed : ");
    Serial.println(limit_body_heat_value);  
  }


  if(checkLimitDistance()){
    buzzer.turnOn();
    Serial.println("-------ALARM DISTANCE--------");
  }
  if(checkLimitTemperature()){
    buzzer.turnOn();
    Serial.println("-------ALARM TEMPERATURE--------");
  }
  if(checkLimitHumidity()){
    buzzer.turnOn();
    Serial.println("-------ALARM HUMIDITY--------");
  }
  if(checkLimitBodyHeat()){
    buzzer.turnOn();
    Serial.println("-------ALARM BODYHEAT--------");
  }
  buzzer.checkButtonOff();


}


void BleManager::setDistance(int dis){ 
    IsDistanceChanged = true;
    distance_val = dis;
}


bool BleManager::checkLimitDistance(){
    if((limit_distance_value != 0) && (limit_distance_value < distance_val)){
      limit_distance_value = 0;
      distance_val = 0;
      return true;
    }
    else return false;
}

bool BleManager::checkLimitTemperature(){
    if((limit_temperature_value != 0 )&& (limit_temperature_value < sensor_val[TEMPERATURE])){
      limit_temperature_value = 0;
      return true;
    }
    else return false;
}

bool BleManager::checkLimitHumidity(){
  if((limit_humidity_value != 0) && (limit_humidity_value < sensor_val[HUMIDITY])){
    limit_humidity_value = 0;
    return true;
  }
  else return false;
}

bool BleManager::checkLimitBodyHeat(){
  if((limit_body_heat_value != 0 )&& (limit_body_heat_value < sensor_val[BODYHEAT])){
    limit_body_heat_value = 0;
    return true;
  }
  else return false;
}

int BleManager::getDistance(){
  return distance_val;
}

void BleManager::setEmergency(int _val){
  emergency_val = _val;
  Serial.print("> 현재 BLE 응급 식별 코드 : ");
  Serial.println(emergency_val);
  IsEmergencyChanged = true;
}
