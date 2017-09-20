
#ifndef checkHeat_h
#define checkHeat_h
#include <stack>
using namespace std;
#include "Bboobboo.h"
#include "tempFunction.h"
#include "InfraredTemperature.h"
#include "pulse.h"
#include "RunningMedian.h"
#include "BleManager.h"
#define TEMPERATURE_STACK_SIZE 3
#define MEDIAN_SIZE		4

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
    float bodyTemp;

    float humidity;
    int temp;
    int heart;
	int count;
    Bboobboo buzzer;
    InfraredTemperature infraredTemp;
    temp_func mTemp;
    PulseSensor pulse;
	RunningMedian median[MEDIAN_SIZE] = {RunningMedian(MEDIAN_MAX_SIZE), RunningMedian(MEDIAN_MAX_SIZE), RunningMedian(MEDIAN_MAX_SIZE), RunningMedian(MEDIAN_MAX_SIZE)};
	
    // Temperature Score Lable
    stack<int>* Temperature_Score_Stack;
    int Temperature_Score;
public:
    void init(BleManager *Manager);
    checkHeat();
    void sendCall(int zeroMotion);
    void checkBodyTemp(float);
    void checkTemp(int);
    void checkHeart(int);
    void allcheck();
    void deBoo();
	void checkMedian();
    void isLongPress();
};


#endif