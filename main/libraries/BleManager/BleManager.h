#ifndef __BLEMANAGER_H
#define __BLEMANAGER_H
#include "Arduino.h"
#include "Bboobboo.h"
#include <CurieBLE.h>
#define SENSOR_VAL_NUM 4
#define EMERGENCY_MODE 119
class BleManager{
private:
	int MAX,cnt;
	int sensor_val[SENSOR_VAL_NUM]; // Datas have to set range in 0 ~ 256 
	int function_switch[4];
 	bool IsDataChanged;
 	int mode;
 	int limit_distance_value,limit_heart_rate_value,limit_humidity_value;
 	int limit_temperature_value,limit_body_heat_value;
 	int distance_val, temp_distance_val;
 	bool IsDistanceChanged,IsEmergencyChanged;
 	int emergency_val;
 	Bboobboo buzzer;
public:
	int TEMPERATURE,BODYHEAT,HEARTRATE,HUMIDITY;
	 	
	BleManager();
	BleManager(
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
	); // This have to initialize in Setup()!
	void initInLoop(
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
  	); 
	void setIntSensorValue(int index, int value); // set the value in the integer characteristic
	int getIntSensorValue(int index){if(0 <= index && index <= 3)return sensor_val[index];else return 0;}
	inline int getMode(){return mode;}
	inline int getLimitDistance(){return limit_distance_value;}
	inline int getLimitHeartRate(){return limit_heart_rate_value;}
	inline int getLimitHumidity(){return limit_humidity_value;}
	bool checkLimitDistance();
	bool checkLimitTemperature();
	bool checkLimitHumidity();
	bool checkLimitBodyHeat();
	void setDistance(int dis);
	int getDistance();
	void setEmergency(int _val);
	inline bool IsDistanceFunctionOn(){if(function_switch[0]==1)return true;else return false;}
	inline bool IsHeartRateFunctionOn(){if(function_switch[1]==1)return true;else return false;}
	inline bool IsHeatScanFunctionOn(){if(function_switch[2]==1)return true;else return false;}
	inline bool IsHumidityFunctionOn(){if(function_switch[3]==1)return true;else return false;}
};
#endif