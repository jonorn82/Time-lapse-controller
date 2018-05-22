/*
 * Jon Orn Omarsson
 */
#include <Arduino.h>
#include "A4988.h"

// Define motor steps per full rev.
// Motor RPM, low RPM means slower movement
#define MOTOR_STEPS 200
#define RPM 10

// Define Pin layout
#define DIR 8
#define STEP 9
#define ENABLE 13 // optional (just delete ENABLE from everywhere if not used)
#define MS1 10
#define MS2 11
#define MS3 12

// Configure motor driver
A4988 stepper(MOTOR_STEPS, DIR, STEP, ENABLE, MS1, MS2, MS3);


// ---------- TIME LAPSE SETTINGS ----------

const int clipLength = 10; // Clip length in seconds
const int eventDuration = 240; // Event duration in minutes
const int fps = 24; // Video FPS
const boolean startAtMotor = false; // Slider will start on the motor side of the slider

// ---------- END --------------------------

const int shots = fps * clipLength;
const int sliderLength() { // Measured length of the slider in steps
  if(startAtMotor){
    return -3240;
  } 
  else {
    return 3240;
  }
}

const int moveLength = sliderLength() / shots;
  
unsigned long moveInterval;


unsigned long currentMillis = 0;
unsigned long prevMillis = 0;
int stepsTraveled = 0;

// #include "BasicStepperDriver.h" // genericv
// BasicStepperDriver stepper(DIR, STEP);



void setup() {
    /*
     * Open serial communication.
     * Set target motor RPM.
     * Set target microstep 1,2,4,8 or 16
     * Set millisecond counter
     */
    //Serial.begin(115200); // used only for debugging
    stepper.begin(RPM);
    stepper.enable();
    stepper.setMicrostep(16);
    prevMillis = millis();
    moveInterval = (eventDuration * 60) / (fps * clipLength);
    moveInterval = moveInterval * 1000;
}

void loop() {

    currentMillis = millis();
    moveSlider();
}

void moveSlider() {
  if (startAtMotor) {
    
    if(stepsTraveled >= sliderLength())
    {
      if(currentMillis - prevMillis >= moveInterval)
      {
        stepper.rotate(moveLength);
        prevMillis = currentMillis;
        stepsTraveled = stepsTraveled + moveLength;
      }
    }
    else
    {
      Serial.println("Slider has stopped");  
      while(1) 
      {
        // statement block
      }
    }
  }
  else {
    if(stepsTraveled <= sliderLength())
    {
      if(currentMillis - prevMillis >= moveInterval)
      {
        stepper.rotate(moveLength);
        prevMillis = currentMillis;
        stepsTraveled = stepsTraveled + moveLength;
      }
    }
    else
    {
      Serial.println("Slider has stopped");  
      while(1) 
      {
        // statement block
      }
    }
  }
}
