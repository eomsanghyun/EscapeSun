#include "StepDetection.h"  
#include "arduino.h"
long StepDetection::lastStepCount = 0;     
boolean StepDetection::iammove = false;     
StepDetection::StepDetection(){
  StepDetection::iammove = false;
  StepDetection::stepEventsEnabeled = true;   // whether you're polling or using events
  StepDetection::lastStepCount = 0;              // step count on previous polling check
}

/*
2017-07-20
심박 수 측정
analog pin 0 사용
*/
void StepDetection::init(){
  CurieIMU.begin();
  // turn on step detection mode:
  CurieIMU.setStepDetectionMode(CURIE_IMU_STEP_MODE_SENSITIVE);
  // enable step counting:
  CurieIMU.setStepCountEnabled(true);
 // if (stepEventsEnabeled) {
    // attach the eventCallback function as the
    // step event handler:
   CurieIMU.attachInterrupt(eventCallbacka);
    CurieIMU.interrupts(CURIE_IMU_STEP);  // turn on step detection

    Serial.println("IMU initialisation complete, waiting for events...");
 // }
}

void StepDetection::updateStepCount(){
    // get the step count:
    int stepCount = CurieIMU.getStepCount();

    //int lastStepCount = CurieIMU.getStepCount();
    // if the step count has changed, print it:
    if (stepCount != lastStepCount) {
      Serial.print("Step count: ");
      Serial.println(stepCount);
      Serial.print("Step Meter: ");
      Serial.println(stepCount*0.8);
      
      // save the current count for comparison next check:
      lastStepCount = stepCount;
    }
}

void StepDetection::eventCallbacka(void){
    if (CurieIMU.stepsDetected()){
      movement();
      updateStepCount();
    }
  }

int StepDetection::movement(){
  Serial.println("movemove");
  iammove = true; 
}
