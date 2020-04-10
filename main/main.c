#include "header.h"

static const char *TAG = "wifi softAP";

void app_main() {
	ESP_ERROR_CHECK(nvs_flash_init());
	ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");
//	xTaskCreate(&task_initI2C, "mpu_task", 2048, NULL, 1, NULL);
	/* Create queue for send to client */
	/* Create  server and setting wifi*/
	server_init(5);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    xTaskCreate(i2c_task_example, "i2c_task_example", 2048, NULL, 5, NULL);

//    xTaskCreate(&task_display, "disp_task", 6800, NULL, 4, NULL);
//	vTaskList();
}
