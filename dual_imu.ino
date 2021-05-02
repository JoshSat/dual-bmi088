/*
Multi BMI088 IMU ori as is.
                      
*/

#include <Orientation.h>
#include "BMI088.h"
#include <Wire.h>

/* accel objects */
Bmi088Accel accel00(Wire,0x18);
Bmi088Accel accel01(Wire,0x19);
/* gyro objects */
Bmi088Gyro gyro00(Wire,0x68);
Bmi088Gyro gyro01(Wire,0x69);

float x;
float y;
float z;

uint64_t thisLoopMicros = 0;

uint64_t lastGyroUpdate = 0;
uint64_t thisGyroUpdate = 0;
uint64_t dtGyroMicros = 0;

Orientation ori;
EulerAngles gyroMeasure;
EulerAngles oriMeasure;

void setup() {
  int status;
  Serial.begin(115200);
  delay(1000); //delay for imu to turn on
  /* start the sensors */
  status = accel00.begin();
  if (status < 0) {
    Serial.println("Accel Initialization Error 00"); //print error accel
    Serial.println(status);
  }
  status = gyro00.begin();
  if (status < 0) {
    Serial.println("Gyro Initialization Error 00");  //print error gyro
    Serial.println(status);
  }
  status = accel01.begin();
  if (status < 0) {
    Serial.println("Accel Initialization Error 01"); //print error accel
    Serial.println(status);
  }
  status = gyro01.begin();
  if (status < 0) {
    Serial.println("Gyro Initialization Error 01");  //print error gyro
    Serial.println(status);
  }

}

void loop() {
  /* read the accel */
  accel00.readSensor();
  accel01.readSensor();
  /* read the gyro */
  gyro00.readSensor();
  gyro01.readSensor();

  thisLoopMicros = micros();

  Serial.print(accel00.getAccelX_mss());
  Serial.print("\t");
  Serial.print(accel01.getAccelX_mss());
  Serial.print("\t");
  Serial.print(accel00.getAccelY_mss());
  Serial.print("\t");
  Serial.print(accel01.getAccelY_mss());
  Serial.print("\t");
  Serial.print(accel00.getAccelZ_mss());
  Serial.print("\t");
  Serial.print(accel01.getAccelZ_mss());
  Serial.print("\t");
  Serial.print(gyro00.getGyroX_rads());
  Serial.print("\t");
  Serial.print(gyro01.getGyroX_rads());
  Serial.print("\t");
  Serial.print(gyro00.getGyroY_rads());
  Serial.print("\t");
  Serial.print(gyro01.getGyroY_rads());
  Serial.print("\t");
  Serial.print(gyro00.getGyroZ_rads());
  Serial.print("\t");
  Serial.print(gyro01.getGyroZ_rads());
  Serial.print("\t");
  Serial.print(x);
  Serial.print("\t");
  Serial.print(y);
  Serial.print("\t");
  Serial.print(z);
  Serial.print("\n");

  x = gyro00.getGyroX_rads() + gyro01.getGyroX_rads() / 2;
  y = gyro00.getGyroY_rads() + gyro01.getGyroY_rads() / 2;
  z = gyro00.getGyroZ_rads() + gyro01.getGyroZ_rads() / 2;

  if(gyro00.getDrdyStatus())
  {
    float dtGyro = (float)(thisLoopMicros - lastGyroUpdate) / 1000000;
    lastGyroUpdate = thisLoopMicros;
    
    gyroMeasure.yaw = -(x);  
    gyroMeasure.pitch = -(y);  
    gyroMeasure.roll = -(z); 

    ori.update(gyroMeasure, dtGyro);
    oriMeasure = ori.toEuler();
  }

  x = oriMeasure.yaw * RAD_TO_DEG;
  y = oriMeasure.pitch * RAD_TO_DEG;
  z = oriMeasure.roll * RAD_TO_DEG; 
}
