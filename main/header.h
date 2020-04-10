/*
 * header.h
 */

#ifndef MAIN_HEADER_H_
#define MAIN_HEADER_H_

#include <string.h>
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_err.h"
#include "nvs_flash.h"

#include "driver/i2c.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

TaskHandle_t	server_tcp_task_hdl;
TaskHandle_t	server_mpu_task_hdl;

struct send_date{
	int socket;
	char *rx_buffer;
	size_t  len;
};
char *machine(void);
char *create_monitor(char verbose_name,int is_date);
/* MPU */
//#define PIN_SDA 7
//#define PIN_CLK 8
void i2c_task_example(void *arg);

/* Main file */
int server_init();

struct data {
	int16_t x;
	int16_t y;
	int16_t z;
};

/*       Wifi config           */
#define ESP_WIFI_SSID      "testAPS"
#define ESP_WIFI_PASS      "testAPS123123123"
#define MAX_CONN       4
#define PORT 80
#define MACADDR	{0x16,	0x34,	0x56,	0x78,	0x90,	0xab}

#endif /* MAIN_HEADER_H_ */
