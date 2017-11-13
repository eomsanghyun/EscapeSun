#ifndef checkHeat_h
#define checkHeat_h
#include "Bboobboo.h"
#include "tempFunction.h"
#include "InfraredTemperature.h"
#include "pulse.h"
#include "RunningMedian.h"
#include "BleManager.h"
#include "TimeCheck.h"
#include "StepDetection.h"
#define TEMPERATURE_STACK_SIZE 3
#define BODY_HEAT_STACK_SIZE 4
#define HEART_RATE_STACK_SIZE 7
#define HUMIDITY_STACK_SIZE 3
#define MEDIAN_SIZE		4
#define SENSOR_CHECK_TIME 1
class checkHeat
{
private:
    BleManager *manager;
    long currentMillis;
    long previousMillis;
    int boo;
    int bodyTempDegree;
    int tempDegree;
    int heartDegree;
    int humidityDegree;
    float bodyTemp;
    bool EmergencyAlarmAlert;
    float humidity;
    int temp;
    int heart;
	int count;
    Bboobboo buzzer;
    InfraredTemperature infraredTemp;
    temp_func mTemp;
    PulseSensor pulse;
	RunningMedian median[MEDIAN_SIZE] = {RunningMedian(MEDIAN_MAX_SIZE), RunningMedian(MEDIAN_MAX_SIZE), RunningMedian(MEDIAN_MAX_SIZE), RunningMedian(MEDIAN_MAX_SIZE)};
	// Check Heat Time
    TimeCheck checkHeat_Time;

    // Temperature Score Lable
    int Temperature_Score_Stack[TEMPERATURE_STACK_SIZE] = {0,0,0};
    TimeCheck Temperature_Time;
    int Temperature_Score;

    // BodyHeat Score Lable
    int BodyHeat_Score_Stack[BODY_HEAT_STACK_SIZE] = {0,0,0,0};
    TimeCheck BodyHeat_Time;
    int BodyHeat_Score;

    // HeartBeat Score Lable
    int HeartRate_Score_Stack[HEART_RATE_STACK_SIZE] = {0,0,0,0,0,0,0};
    TimeCheck HeartRate_Time;
    int HeartRate_Score;

    // Humidity Score Lable
    int Humidity_Score_Stack[HUMIDITY_STACK_SIZE] = {0,0,0};
    TimeCheck Humidity_Time;
    int Humidity_Score;

    // Test Data
    int test_temperature;
    int test_heart_rate;
    int test_body_heat;
    int test_humidity;

    // Movement Score Lable
    TimeCheck Movement_Time;
    int Movement_Score;

    // Emergency CODE
    int EMG_CODE;
    int EMG_level_01;
    int EMG_level_02;

    // Datas
    int distance_display;
    int body_temp_display;
    int temp_display;
    int humid_display;
    int heart_display;


public:
    void init(BleManager *Manager);
    void reset();
    checkHeat();
    void sendCall(int zeroMotion);
    void checkBodyTemp(float);
    void checkTemp(int);
    void checkHeart(int);
    void checkHumidity(int);
    int checkMovement(StepDetection stepdetect);
    void allcheck(StepDetection stepdetect);
    void deBoo();
	void checkMedian();
    void isLongPress();
    // Temperature Score Functions
    void SetCurrentTemperatureScore();
    void TemperatureScoreStackPop(int idx);
    // Test Function
    void checkTestData(StepDetection stepdetect);
    void resetTestData();
    void getTestDataFromBLE();
    bool heatCramps(); //열경련 and 열실신 
    bool heatExhaustion();
    bool heatStroke();
    void heatAllcheck(StepDetection stepdetect);
    bool checkBuzzer();
    inline int getEmergencyCode(){return (EMG_CODE % 100);}
    inline int getDistanceDis(){
        if(manager != NULL)
            return manager->getDistance();
        else
            return 0;
    }
    inline int getTempDis(){return temp_display;}
    inline int getBodyTempDis(){return body_temp_display;}
    inline int getHumidDis(){return humid_display;}
    int getEMGCODE_set01(int);
    int getEMGCODE_set02(int);
};


#endif