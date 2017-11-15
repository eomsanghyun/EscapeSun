#include "checkHeat.h"
#define TEMP 			0
#define BODY 			1
#define HEART			2
#define HUMI 			3
#define BODY_TEMP_MAX	50
#define BODY_TEMP_MIN	30
#define TEMP_MAX		50
#define TEMP_MIN		10
#define HEART_MAX		200
#define HEART_MIN		40

checkHeat::checkHeat()
{
	reset();
}

void checkHeat::reset(){
	currentMillis = millis();
	previousMillis = currentMillis-30000;
	boo = 0;
	bodyTemp= 0;
	temp = 0;
	heart = 0;
	bodyTempDegree = 0;
	tempDegree = 0;
	heartDegree = 0;
	humidityDegree = 0;
	count = 0;
	EMG_CODE = 0;
	EMG_level_01 = 0;
	EMG_level_02 = 0;
	buzzer = Bboobboo();
	Temperature_Time.init();
	Temperature_Time.resetTime();
	BodyHeat_Time.init();
	BodyHeat_Time.resetTime();
	HeartRate_Time.init();
	HeartRate_Time.resetTime();
	Humidity_Time.init();
	Humidity_Time.resetTime();
	Movement_Time.init();
	Movement_Time.resetTime();
	checkHeat_Time.init();
	checkHeat_Time.resetTime();
	EmergencyAlarmAlert = false;
	distance_display = 0;
    body_temp_display= 0;
    temp_display= 0;
    humid_display= 0;
    heart_display = 0;
    for(int i = 0 ; i < TEMPERATURE_STACK_SIZE; i++)Temperature_Score_Stack[i]=0;
    for(int i = 0 ; i < BODY_HEAT_STACK_SIZE; i++)BodyHeat_Score_Stack[i]=0;
    for(int i = 0 ; i < HEART_RATE_STACK_SIZE; i++)HeartRate_Score_Stack[i]=0;
    for(int i = 0 ; i < HUMIDITY_STACK_SIZE; i++)Humidity_Score_Stack[i]=0;
    Temperature_Score = 0;
    BodyHeat_Score = 0;
    HeartRate_Score = 0;
    Humidity_Score = 0;
    Movement_Score = 0;
    resetTestData();
    test_temperature = 0;
    test_heart_rate = 0;
    test_body_heat = 0;
    test_humidity = 0;

    
}

void checkHeat::init(BleManager *Manager){
  	manager = Manager;
}

void checkHeat::deBoo(){
	previousMillis = millis();
	boo = 0;
	buzzer.turnOff();
}


void checkHeat::sendCall(int zeroMotion)
{

	count = tempDegree + bodyTempDegree + heartDegree;
	
	if(zeroMotion)
		count++;
		
	
	if(count >= 5){
		boo = 1;
		long currentMillis = millis();
		long laterMillis;
		buzzer.turnOn();
		int buttonState;    

	    while(1) {
	    	laterMillis = millis();
	    	bool flag;
	    	if(laterMillis-currentMillis > 30000) {
	    			Serial.println("time 30 over");
	    			if(!flag){
	    				// send messeage
	    				//manager->setIntSensorValue(, int(119));
	    			}
	    			deBoo();
	    			delay(1500);
	    			buzzer.turnOn();
	    	}

	      buttonState = digitalRead(13);    
	      if(buttonState == LOW){         
	         deBoo();
	         break;
	      } 
	    }
	}
}

void checkHeat::isLongPress(){
      int buttonState = digitalRead(13);    
      if(buttonState == LOW){
      	long currentMillis = millis();
		long laterMillis = millis();
		bool bzFlag = true;         
      	while(laterMillis-currentMillis < 5000){
      		if(laterMillis-currentMillis > 1000){
      			if(bzFlag){
      				buzzer.turnOn();
      				bzFlag = false;
      			}
      		}

      		laterMillis = millis();
      		if(buttonState == HIGH){
      			Serial.println("------------------> User Emergency OFF!");
      			buzzer.turnOff();
      			return;
      		}
      		buttonState = digitalRead(13);
      	}
      	// send message to smartphone hi:
      	// manager->setIntSensorValue(, int(119));
      	manager->setEmergency(getEMGCODE_set02(3));
      	Serial.println("------------------> User Emergency ON!");
      	buzzer.turnOff();
    } 
}
void checkHeat::checkBodyTemp(float bodyTemperature)
{
	if(BodyHeat_Time.Secondtime() > SENSOR_CHECK_TIME){
		BodyHeat_Time.resetTime();
		Serial.println("-----------------------------------------------------------");
		Serial.print("체온 : ");
		Serial.print(bodyTemperature);
		Serial.println(" ℃");
		if(bodyTemperature < 37){
			bodyTempDegree = 0;
		}
		else if(bodyTemperature < 38){
			bodyTempDegree = 1;
		}
		else if(bodyTemperature < 39){
			bodyTempDegree = 2;
		}
		else if(bodyTemperature < 40){
			bodyTempDegree = 3;
		}
		else{
			bodyTempDegree = 4;
		}

		// Add Score stack
		if(bodyTempDegree > 0)BodyHeat_Score_Stack[bodyTempDegree-1]++;
		
		// Remove Score stack
		for(int i = bodyTempDegree; i < BODY_HEAT_STACK_SIZE ; i++){
			if(BodyHeat_Score_Stack[i] > 0)BodyHeat_Score_Stack[i]--;
		}

		// Caculate Score
		BodyHeat_Score = 0;
		for(int i = 0 ; i < BODY_HEAT_STACK_SIZE; i++){
			BodyHeat_Score += BodyHeat_Score_Stack[i] * (i+1);
		}

		// Print Score
		Serial.print("/ 10초당 누적 점수 : ");
		Serial.println(BodyHeat_Score);	
	}
	
}

void checkHeat::checkTemp(int temperature)
{
	if(Temperature_Time.Secondtime() > SENSOR_CHECK_TIME){
		Temperature_Time.resetTime();
		Serial.print("외부온도 : " );
		Serial.print(temperature);
		Serial.println(" ℃");
		if(temperature >= 30 && temperature < 34 ){
			tempDegree = 1;
			Temperature_Score_Stack[0]++;	
		}
		else if(temperature >= 34 && temperature < 39){
			tempDegree = 2;
			Temperature_Score_Stack[1]++;	
		}
		else if(temperature >= 39){
			tempDegree = 3;
			Temperature_Score_Stack[2]++;
		}
		else{
			tempDegree = 0;
		}

		for(int i = tempDegree; i < TEMPERATURE_STACK_SIZE ; i++){
			if(Temperature_Score_Stack[i] > 0)Temperature_Score_Stack[i]--;
		}

		Temperature_Score = 0;
		for(int i = 0 ; i < TEMPERATURE_STACK_SIZE ; i++){
			Temperature_Score += (Temperature_Score_Stack[i] * (i+1));
		}
		
		Serial.print("/ 10초당 누적 점수 : ");
		Serial.println(Temperature_Score);
	}
}

void checkHeat::SetCurrentTemperatureScore(){
	Temperature_Score = 0;
	for(int i = 0 ; i < TEMPERATURE_STACK_SIZE ; i++){
		Temperature_Score += (Temperature_Score_Stack[i] * (i+1));
	}
	Serial.print("Temperature_Score : ");
	Serial.println(Temperature_Score);
}
void checkHeat::TemperatureScoreStackPop(int idx){
	for(int i = idx; i < TEMPERATURE_STACK_SIZE ; i++){
		if(Temperature_Score_Stack[i] > 0)Temperature_Score_Stack[i]--;
	}
}

void checkHeat::checkHeart(int heartRate)			
{
	if(HeartRate_Time.Secondtime() > SENSOR_CHECK_TIME){
		HeartRate_Time.resetTime();
		//Serial.print("pulse!!= " );
		//Serial.println(heartRate);

		if(heartRate < 50){
			heartDegree = 3;
		}else if(50 <= heartRate && heartRate < 60){
			heartDegree = 1;
		}else if(60 <= heartRate && heartRate < 80){
			heartDegree = 0;
		}else if(80 <= heartRate && heartRate < 110){
			heartDegree = 1;
		}else if(110 <= heartRate && heartRate < 130){
			heartDegree = 2;
		}else if(130 <= heartRate && heartRate < 150){
			heartDegree = 3;
		}else{
			heartDegree = 4;
		}

		// Add Score stack
		if(tempDegree){
			if(heartDegree > 0)HeartRate_Score_Stack[heartDegree-1]++;
		}
		// Remove Score stack
		for(int i = heartDegree; i < HEART_RATE_STACK_SIZE ; i++){
			if(HeartRate_Score_Stack[i] > 0)HeartRate_Score_Stack[i]--;
		}

		// Caculate Score
		HeartRate_Score = 0;
		for(int i = 0 ; i < HEART_RATE_STACK_SIZE; i++){
			HeartRate_Score += HeartRate_Score_Stack[i] * (i+1);
		}

		// Print Score
		// Serial.print("HeartRate_Score : ");
		// Serial.println(HeartRate_Score);	
	}										
	
}

void checkHeat::checkHumidity(int humidity){
	if(Humidity_Time.Secondtime() > SENSOR_CHECK_TIME){
		Humidity_Time.resetTime();
		Serial.print("습도 : ");
		Serial.print(humidity);
		Serial.println(" (Percent)");	
		if(0 <=  humidity && humidity < 60 ){
			humidityDegree = 0;
		}else if(70 <=  humidity && humidity < 90 ){
			humidityDegree = 1;	
		}else if(90 <=  humidity && humidity < 100 ){
			humidityDegree = 2;	
		}else{
			//error!
			humidityDegree = 0;
		}	
		if(tempDegree){
			// Add Score stack
			if(humidityDegree > 0)Humidity_Score_Stack[humidityDegree-1]++;
		}
		// Remove Score stack
		for(int i = humidityDegree; i < HUMIDITY_STACK_SIZE ; i++){
			if(Humidity_Score_Stack[i] > 0)Humidity_Score_Stack[i]--;
		}

		// Caculate Score
		Humidity_Score = 0;
		for(int i = 0 ; i < HUMIDITY_STACK_SIZE; i++){
			Humidity_Score += Humidity_Score_Stack[i] * (i+1);
		}

		// Print Score
		Serial.print("/ 10초당 누적 점수 : ");
		Serial.println(Humidity_Score);	

	}
}

int checkHeat::checkMovement(StepDetection stepdetect){
	Movement_Time.resetTime();
	stepdetect.iammove = false;
	//stepdetect.updateStepCount();
	int movementnum = 0;
	while(Movement_Time.Secondtime() < SENSOR_CHECK_TIME){

		 if(stepdetect.iammove == 1){
		 	//움직임이 있다.
		 	movementnum = 1;
		 }
		delay(1000);
	}
	return movementnum;
}
void checkHeat::checkMedian(){
	infraredTemp = InfraredTemperature();
	mTemp.temp_est(); 
	for(int i = 0; i < MEDIAN_MAX_SIZE; i++){
		bodyTemp = infraredTemp.getObjectTempC();
		temp = mTemp.getTemp();
		humidity = mTemp.getHumi();
		heart  = pulse.updateHeartRate();	
		if(heart == -1){					
			heart = pulse.getHeartRate();
		}
		if(bodyTemp > BODY_TEMP_MAX || bodyTemp < BODY_TEMP_MIN 
			|| temp < TEMP_MIN || temp > TEMP_MAX 
			|| heart < HEART_MIN || heart > HEART_MAX ){
			i--;
			/*
			Serial.print("BodyTemp: ");
			Serial.print(bodyTemp);
			Serial.print(" / MAX :");
			Serial.print(BODY_TEMP_MAX);
			Serial.print(" / MIN :");
			Serial.println(BODY_TEMP_MIN);
			
			Serial.print("Temperature :");
			Serial.print(temp);
			Serial.print(" / MAX :");
			Serial.print(TEMP_MAX);
			Serial.print(" / MIN :");
			Serial.println(TEMP_MIN);
			
			Serial.print(heart);
			Serial.print(" ");
			Serial.print(HEART_MAX);
			Serial.print(" ");
			Serial.println(HEART_MIN);
			Serial.println();

			Serial.print(humidity);
			Serial.print(" ");
			Serial.println();
			*/
			delay(100);
			
			continue;
		}
		median[HEART].add(heart);
		median[TEMP].add(temp);
		median[BODY].add(bodyTemp);
		median[HUMI].add(humidity);
	}
	
	temp_display = int(median[manager->TEMPERATURE].getAverage(median[manager->TEMPERATURE].getMedian()));
	body_temp_display = int(median[manager->BODYHEAT].getAverage(median[manager->BODYHEAT].getMedian()));
	heart_display = int(median[manager->HEARTRATE].getAverage(median[manager->HEARTRATE].getMedian()));
	humid_display = int(median[manager->HUMIDITY].getAverage(median[manager->HUMIDITY].getMedian()));

	manager->setIntSensorValue(manager->TEMPERATURE, temp_display);
	manager->setIntSensorValue(manager->BODYHEAT, body_temp_display);
	manager->setIntSensorValue(manager->HEARTRATE, heart_display);
	manager->setIntSensorValue(manager->HUMIDITY, humid_display);
		
	/*checkBodyTemp(median[BODY].getAverage(median[BODY].getMedian()));
	checkTemp(median[TEMP].getAverage(median[TEMP].getMedian()));
	checkHeart(median[HEART].getAverage(median[HEART].getMedian()));
	checkHumidity(median[HUMI].getAverage(median[HUMI].getMedian()));*/

	checkBodyTemp(body_temp_display);
	checkTemp(temp_display);
	checkHeart(heart_display);
	checkHumidity(humid_display);

	for(int i = 0; i < MEDIAN_SIZE; i++){
		median[i].clear();
	}
}


void checkHeat::checkTestData(StepDetection stepdetect){
	currentMillis = millis();
	getTestDataFromBLE();
	checkBodyTemp(test_body_heat);
	checkTemp(test_temperature);
	checkHeart(test_heart_rate);
	checkHumidity(test_humidity);
	heatAllcheck(stepdetect);
	// Test?
	isLongPress();
}


void checkHeat::resetTestData(){
    if(manager != NULL){
        manager->setIntSensorValue(0,28);
        manager->setIntSensorValue(1,35);
        manager->setIntSensorValue(2,75);
        manager->setIntSensorValue(3,30);
    }
    test_temperature = 28;
    test_body_heat = 35;
    test_heart_rate = 75;
    test_humidity = 30;
}

void checkHeat::getTestDataFromBLE(){
	test_temperature = manager->getIntSensorValue(0);
    test_body_heat = manager->getIntSensorValue(1);
    test_heart_rate = manager->getIntSensorValue(2);
    test_humidity = manager->getIntSensorValue(3);
}


void checkHeat::allcheck(StepDetection stepdetect){
	currentMillis = millis();	
	checkMedian();
	heatAllcheck(stepdetect);
	// Test?
	isLongPress();
}


bool checkHeat::heatCramps(){
	if(Humidity_Score + Temperature_Score > 60){// 600
		return true;
	}
	return false;
}

bool checkHeat::heatExhaustion(){

	if(bodyTempDegree > 1){
		if(Humidity_Score + Temperature_Score + HeartRate_Score > 120){//1200
			return true;
		
		}
	}
	else if(bodyTempDegree >= 0){
		if(Humidity_Score + Temperature_Score + HeartRate_Score > 150){//1500
			return true;
		}
	}
	return false;

}

bool checkHeat::heatStroke(){
	if(bodyTempDegree > 3 ){
		return true;
	}
	else if(bodyTempDegree > 2){
		if(Humidity_Score + Temperature_Score + HeartRate_Score > 90){//900
			return true;
		}
	}
	return false;
}

void checkHeat::heatAllcheck(StepDetection stepdetect){

	// Infinity Loop --> After send the Emergency Message and alert Buzzer
	while(EmergencyAlarmAlert){
		int buttonState = digitalRead(13);    
        if(buttonState == LOW){  
            deBoo();
            Serial.println("<긴급 구호 버저가 종료되었습니다>");
        	reset();
        	EmergencyAlarmAlert = false;
        }else{
        	buzzer.turnOn();
	        delay(300); 
	        deBoo();
	        delay(300);
        }
	} 



	// Where is the Time Check? (ERROR)
	if(checkHeat_Time.Secondtime() > SENSOR_CHECK_TIME){
		checkHeat_Time.resetTime();
		// 움직임 : 있음/없음
		int distance_val = manager->getDistance();
		Serial.print("이동거리 : ");
		Serial.print(distance_val*(0.8));
		Serial.println(" (M)");
		// 누적점수 : 누적점수총합
		int total_score = Temperature_Score + BodyHeat_Score + Humidity_Score;
		Serial.print("총 누적 점수 : ");
		Serial.print(total_score);
		Serial.println(" 점");



		int degree = 0;
		if(heatStroke()){
			degree = 3;
		}
		else if(heatExhaustion()){
			degree = 2;
		}
		else if(heatCramps()){
			degree = 1;
		}

		if(degree == 1){
			// Advice Level 1 Emergency advice CODE x1
			manager->setEmergency(getEMGCODE_set01(1));
			Serial.println("평가 : 온열질환의 위험이 다소 있습니다. 열경련이나 열실신이 의심되니 쉬면서 다니세요. ");	
		}
		else if(degree == 2){
			// Advice Level 2 Emergency advice CODE x2
			manager->setEmergency(getEMGCODE_set01(2));
			Serial.println("평가 : 더운 날씨에 너무 오래 야외에 있으신건 아닌가요? 일사병이 발생 할 수 있으니 조금 쉬시는걸 추천합니다. ");
		}
		else if(degree == 3){
			// Advice Level 3 Emergency advice CODE x3 
			manager->setEmergency(getEMGCODE_set01(3));
			Serial.println("평가 : 외부 환경과 신체지수가 많이 위험해 보입니다. 열사병의 위험이 있으니 조심하셔야 합니다! ");
		}else{
			// Advice Level 0 Emergency advice CODE x0
			manager->setEmergency(getEMGCODE_set01(0));
			Serial.println("평가 : 야외활동을 하기 좋은 날입니다. ");
		}

		if(degree != 0 ){
			if(tempDegree  == 0){
				// In shade state Emergency advice CODE 0x
				manager->setEmergency(getEMGCODE_set02(0));	
				Serial.println("조언 : 지금 처럼 쉬어주면 온열 질환을 예방할 수 있습니다 ");
				Serial.print("식별 코드 : ");
    				Serial.println(getEmergencyCode());
			}
			else{
				if(checkMovement(stepdetect) == 1){
					// ... detect movement Emergency advice CODE 1x
					manager->setEmergency(getEMGCODE_set02(1));	
					Serial.println("조언 : 계속 움직이시면 위험할 수도 있어요. 잠시 휴식을 취해주세요");	
					Serial.print("식별 코드 : ");
    				Serial.println(getEmergencyCode());
				}
				else{
					// silent.. Buzzer On! Emergency advice CODE 2x
					manager->setEmergency(getEMGCODE_set02(2));
					Serial.println("조언 : 좀 더 시원한 곳에서 쉬시는걸 추천합니다. 하지만 지금 의식이 있으신지 의심됩니다. 버저 버튼을 눌러주세요!");
					Serial.print("식별 코드 : ");
    				Serial.println(getEmergencyCode());
    				buzzer.turnOn();
					if(checkBuzzer()){
						manager->setEmergency(0);
						manager->setDistance(0);	
						manager->setIntSensorValue(0,0);
						manager->setIntSensorValue(1,0);
						manager->setIntSensorValue(2,0);
						manager->setIntSensorValue(3,0);
						reset();
					}
				}
			}
		}else{
			// Emergency advice CODE 0x
			manager->setEmergency(getEMGCODE_set02(0));
			Serial.println("조언 : 지금 처럼 쉬어주면 온열 질환을 예방할 수 있습니다 ");
			Serial.print("식별 코드 : ");
    				Serial.println(getEmergencyCode());
		}
		
	}

}
bool checkHeat::checkBuzzer(){
	long currentMillis = millis();
	long laterMillis;
	int buttonState = digitalRead(13);
	while(1){
    	laterMillis = millis();
    	if(laterMillis-currentMillis > 10000){// 10s
			// silent.. Buzzer On! Emergency advice CODE 3x
			manager->setEmergency(getEMGCODE_set02(3));
			Serial.println("+ 조언 : 버저 버튼이 30초간 눌러지지 않아 긴급 구조 문자가 발송되었습니다. 지금 몸에 이상이 있으시다면 꼭 쉬어야 합니다. ");
			manager->setEmergency(getEmergencyCode());
			Serial.print("+ 식별 코드 : ");
			Serial.println(getEmergencyCode());
			
			buzzer.turnOn();
			EmergencyAlarmAlert = true;
			return false;
			/*
			여기서 리턴을 해야 메인 루프로 돌아가서 블루투스로 긴급 신호가 전송 된다. 따라서 해당 루프에서 벗어나야함.
			*/
			
			/*
			그런 이유로 이 코드는 의미가 없음
			buttonState = digitalRead(13);
	        if(buttonState == LOW){  
	            deBoo();
	            return true;
	        }
			delay(300);
			deBoo();
			delay(300);
			buzzer.turnOn();*/
    	}
    	else{
	    	buttonState = digitalRead(13);
	    	if(buttonState == LOW){  
	            deBoo();
	            return true;
	        }
	        buzzer.turnOn();
	    	delay(300);
			deBoo();
			delay(300);
		}
    }
    return true;
}

int checkHeat::getEMGCODE_set01(int new_level_01){
	EMG_level_01 = new_level_01;
	EMG_CODE = EMG_level_01 + EMG_level_02 * 10;
	return EMG_CODE;
}

int checkHeat::getEMGCODE_set02(int new_level_02){
	EMG_level_02 = new_level_02;
	EMG_CODE = EMG_level_01 + EMG_level_02 * 10;
	return EMG_CODE;
}