/*
 * header.h
 */

#pragma once

#include <string.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_err.h"
#include "nvs_flash.h"

#include "driver/i2c.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

TaskHandle_t	server_tcp_task_hdl ;
TaskHandle_t	server_mpu_task_hdl ;

struct date_i2c{
	float x; //pitch
	float y; //row
	float z; //yaw
}Date_ini;

struct send_date{
	int socket;
	char rx_buffer[128];
	size_t  len;
};

/* MPU */
esp_err_t mpu_init();
esp_err_t mpu_start(uint8_t prior,uint16_t mem_us);
size_t mpu_get_date(char *json_str);

/* WIFI file */
esp_err_t server_init();
esp_err_t server_start(uint8_t prior,uint16_t mem_us);


/*       Wifi config           */
#define ESP_WIFI_SSID      "testAPS"
#define ESP_WIFI_PASS      "testAPS123123123"
#define MAX_CONN       4
#define PORT 80
#define MACADDR	{0x16,	0x34,	0x56,	0x78,	0x90,	0xab}

