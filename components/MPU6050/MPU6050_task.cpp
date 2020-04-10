/*
 * MPU6050_task.cpp
 *
 *  Created on: Apr 10, 2020
 *      Author: j
 */

#include "mpu_task.h"
#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#define PIN_SDA 7
#define PIN_CLK 8

I2c_conf_d conf_rx;
MPU6050 mpu;

void task_initI2C(void *ignore) {
	conf_rx.packetSize = 42;   // expected DMP packet size (default is 42 bytes)
	conf_rx.fifoCount = 0;     // count of all bytes currently in FIFO

	i2c_config_t conf;
	conf.mode = I2C_MODE_MASTER;
	conf.sda_io_num = (gpio_num_t) PIN_SDA;
	conf.scl_io_num = (gpio_num_t) PIN_CLK;
	conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
	conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
	ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &conf));
	ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER));
	mpu = MPU6050();
	mpu.initialize();
	mpu.dmpInitialize();

	// This need to be setup individually
	mpu.setXGyroOffset(220);
	mpu.setYGyroOffset(76);
	mpu.setZGyroOffset(-85);
	mpu.setZAccelOffset(1788);

	mpu.setDMPEnabled(true);

}

//float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
void get_date(float *ypr) {
	conf_rx.mpuIntStatus = mpu.getIntStatus();
	// get current FIFO count
	conf_rx.fifoCount = mpu.getFIFOCount();

	if ((conf_rx.mpuIntStatus & 0x10) || conf_rx.fifoCount == 1024) {
		// reset so we can continue cleanly
		mpu.resetFIFO();
	}
	// otherwise, check for DMP data ready interrupt frequently)
	else if (conf_rx.mpuIntStatus & 0x02) {
		// wait for correct available data length, should be a VERY short wait
		while (conf_rx.fifoCount < conf_rx.packetSize) {
			conf_rx.fifoCount = mpu.getFIFOCount();
		}
		// read a packet from FIFO

		mpu.getFIFOBytes(conf_rx.fifoBuffer, conf_rx.packetSize);
		mpu.dmpGetQuaternion(&q, conf_rx.fifoBuffer);
		mpu.dmpGetGravity(&gravity, &q);
		mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
		printf("YAW: %3.1f, ", ypr[0] * 180/M_PI);
		printf("PITCH: %3.1f, ", ypr[1] * 180/M_PI);
		printf("ROLL: %3.1f \n", ypr[2] * 180/M_PI);
		//			Send_Q(ypr[0] * 180/M_PI,ypr[1] * 180/M_PI,ypr[2] * 180/M_PI);
		//		vTaskDelay(40/portTICK_PERIOD_MS);

	};

}
