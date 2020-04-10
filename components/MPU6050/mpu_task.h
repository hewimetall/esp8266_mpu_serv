/*
 * mpu_task.c
 *
 *  Created on: Apr 10, 2020
 *      Author: j
 */

#ifndef COMPONENTS_MPU6050_MPU_TASK_C_
#define COMPONENTS_MPU6050_MPU_TASK_C_
#include "driver/i2c.h"
#include <MPU6050.h>
#include <MPU6050_6Axis_MotionApps20.h>

Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
//float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

void task_initI2C(void *ignore);
void get_date(float *ypr);

struct MPU_yarl{
	float X;
	float Y;
	float Z;
};

struct I2c_conf_d{
	uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
	uint8_t fifoBuffer[64]; // FIFO storage buffer
	uint8_t mpuIntStatus;
	uint16_t fifoCount;     // count of all bytes currently in FIFO
};


#endif /* COMPONENTS_MPU6050_MPU_TASK_C_ */
