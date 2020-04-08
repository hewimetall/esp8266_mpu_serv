/*
 * header.h
 */

#ifndef MAIN_HEADER_H_
#define MAIN_HEADER_H_

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

TaskHandle_t	server_tcp_task_hdl;
TaskHandle_t	server_mpu_task_hdl;
char *machine(void);
char *create_monitor(char verbose_name,int is_date);

/* Main file */
int server_init();

struct data {
	int16_t x;
	int16_t y;
	int16_t z;
};

QueueHandle_t xQueue1; // date Q

#endif /* MAIN_HEADER_H_ */
