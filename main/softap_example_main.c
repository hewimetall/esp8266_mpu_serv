#include "header.h"

static const char *TAG = "wifi softAP";

void app_main() {
	ESP_ERROR_CHECK(nvs_flash_init());
		ESP_LOGI(TAG, "ESP_WIFI_MODE_AP %s",create_monitor("WEB",0));
//	ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");
//	/* Create  server and setting wifi*/
//	server_init();
		for(;;){

		}
}
