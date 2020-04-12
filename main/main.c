#include "header.h"

static const char *TAG = "wifi softAP";

void app_main() {
	ESP_ERROR_CHECK(nvs_flash_init());
	server_init();
	mpu_init();

	/* Create  server and setting wifi*/
	server_start((uint8_t) 5,(uint16_t) 2048);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    /* Create  i2c task and setting wifi*/
    mpu_start((uint8_t) 6,(uint16_t) 1024);

}
