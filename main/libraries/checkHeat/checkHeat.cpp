#include "checkHeat.h"

checkHeat::checkHeat()
{
     	   bodyTemp= 0;
   	   Temp = 0;
   	   Heart = 0;
	   BodyTempdegree = 0;
           Tempdegree = 0;
           Heartdegree = 0;
}


void checkHeat::SendCall()
{
	if( (Tempdegree+ BodyTempdegree+ Heartdegree) >=5){
		//������� ����ؼ� ������ ��ȭ�� �Ѵ�.
	}
	else{
		// ��� �˻縦 �Ѵ�.
	}
}


void checkHeat::checkBodyTemp()
{
	infraredTemp = InfraredTemperature();
	bodyTemp = infraredTemp.getObjectTempC();
        Serial.print("bodytemp= " );
	Serial.println(bodyTemp);
	if(bodyTemp <38){
		BodyTempdegree = 0;}
	else if(Temp <39){
		BodyTempdegree = 1;}
	else if(Temp <40){
		BodyTempdegree = 3;}
	else{
		BodyTempdegree = 5;}
}

void checkHeat::checkTemp()
{
        m_temp.temp_est(); 
	Temp = m_temp.getTemp();
        Serial.print("temp= " );
	Serial.println(Temp);
	if(Temp >= 30 && Temp <34 ){
		Tempdegree = 1;}
	else if(Temp>=34 && Temp <39){
		Tempdegree = 2;}
	else if(Temp >=39){
		Tempdegree = 3;}
	else{
		Tempdegree = 0;}
}

void checkHeat::checkHeart()
{
	Heart  = pulse.updateHeartRate();
 	if(Heart == -1){
    		Heart = pulse.getHeartRate();
 	 }
        Serial.print("pulse!!= " );
	Serial.println(Heart);
//	if(Heart <38){                   ����� �ɹڼ��� ���� ����
//		Heartdegree = 0;}
//	else if(Heart <39){
//		Heartdegree = 1;}
//	else if(Heart <40){
//		Heartdegree = 2;}
//	else{
//		Heartdegree = 3;}
}

void checkHeat::Allcheck(){
	checkBodyTemp();
	checkTemp();
	checkHeart();
}
