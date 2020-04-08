/*
 * server.c
 *
 */


#include "header.h"
#define ESP_WIFI_SSID      "testAPS"
#define ESP_WIFI_PASS      "testAPS123123123"
#define MAX_CONN       4
#define PORT 80
#define MACADDR	{0x16,	0x34,	0x56,	0x78,	0x90,	0xab}

static const char *TAG = "wifi server";
//
//int transiv(int socket,char *date){
//
//	return 0;
//}
//
//int received(int socket,char *messenge){
//    inet_ntoa_r(((struct sockaddr_in *)&sourceAddr)->sin_addr.s_addr, addr_str, sizeof(addr_str) - 1);
//    rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
//
//    int err = send(sock, rx_buffer, len, 0);
//
//    if (err < 0) {
//        ESP_LOGE(TAG, "Error occured during sending: errno %d", errno);
//        break;
//
//	return 0;
//}

int connect_hub_loop(int listen_socket){

	struct sockaddr_in remote_addr;
	uint addrLen = sizeof(remote_addr);
	int client_sock;
	ESP_LOGI(TAG,"ESP8266	TCP	server	task	>	wait	client\n");
	/*block	here	waiting	remote	connect	request*/
	client_sock = accept(listen_socket, (struct sockaddr *)&remote_addr, &addrLen);

	if (client_sock  < 0 ) {
		ESP_LOGE(TAG,"ESP8266	TCP	server	task	>	accept	fail\n");
		return -1;
	}
	ESP_LOGI(TAG,"ESP8266	TCP	server	task	>	Client	from	%s	%d\n",
			inet_ntoa(remote_addr.sin_addr),
			htons(remote_addr.sin_port));
	return client_sock;
	}


int serv_socket_ini(int bind_port){

	struct sockaddr_in destAddr;
	int addr_family;
	int ip_protocol;
    char addr_str[128];
    int err;
    int listen_sock;
	/*	Config protocol	*/
	destAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	destAddr.sin_family = AF_INET;
	destAddr.sin_port = htons(bind_port);
	addr_family = AF_INET;
	ip_protocol = IPPROTO_IP;

	inet_ntoa_r(destAddr.sin_addr, addr_str, sizeof(addr_str) - 1);
	do {
		listen_sock = socket(addr_family, SOCK_STREAM, ip_protocol);
		if (listen_sock == -1 ) {
			ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
			vTaskDelay(1000/portTICK_RATE_MS);
		}
	}while(listen_sock == -1);

	ESP_LOGI(TAG, "Socket created  %i",listen_sock);
	/*	Bind	to	the	local	port	*/

	do {
		err = bind(listen_sock, (struct sockaddr *)&destAddr, sizeof(destAddr));
		if (err != 0) {
			ESP_LOGE(TAG,"ESP8266 TCP server task>bind fail\n");
			vTaskDelay(1000/portTICK_RATE_MS);
		}
	} while (err != 0);

	ESP_LOGI(TAG, "Socket binded");

	do{
		err = listen(listen_sock, MAX_CONN);
		if (err != 0) {
				ESP_LOGE(TAG, "Error occured during listen: errno %d", errno);
				vTaskDelay(1000/portTICK_RATE_MS);
			}
	}while (err != 0);

	ESP_LOGI(TAG, "Socket listening");
	ESP_LOGI(TAG, "Server init");
//	struct sockaddr_in sourceAddr;
//	uint addrLen = sizeof(sourceAddr);
//	do{
//		int sock = accept(listen_sock, (struct sockaddr *)&sourceAddr, &addrLen);
//		if (sock < 0) {
//			ESP_LOGE(TAG, "Unable to accept connection: errno %d", errno);
//			vTaskDelay(1000/portTICK_RATE_MS);
//		}
//	}while(sock < 0);
//
//	ESP_LOGI(TAG, "Socket accepted %i",sock);

	return listen_sock; // @suppress("Return socket connected users")
}

static void tcp_server_task(void *pvParameters)
{
    char rx_buffer[128];
    int len =127;
    int listen_port = serv_socket_ini(PORT);
    int client_socket=-1;
    rx_buffer[127]=0;
    for(;;){
    	client_socket=connect_hub_loop(listen_port);
    	strcpy(rx_buffer, "hellow word");
    	int err = send(client_socket, rx_buffer, len, 0);
    	if (err < 0) {
    		ESP_LOGE(TAG, "Error occured during sending: errno %d", errno);
    		break;
    	}
    	vTaskDelay(100/portTICK_RATE_MS);

    }
    vTaskDelete(NULL);
}

static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                    int32_t event_id, void* event_data)
{
    if (event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" join, AID=%d",
                 MAC2STR(event->mac), event->aid);

    } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" leave, AID=%d",
                 MAC2STR(event->mac), event->aid);
    }

	switch (event_id) {
		default:
			printf("%i \n",event_id);
			break;
	}

}

void wifi_dhsp_conf(){
	ESP_ERROR_CHECK(tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_AP));
	tcpip_adapter_ip_info_t info;
	memset(&info, 0, sizeof(info));
	IP4_ADDR(&info.ip, 10, 0, 0, 1);
	IP4_ADDR(&info.gw, 10, 0, 0, 1);
	IP4_ADDR(&info.netmask, 255, 255, 255, 0);
	ESP_ERROR_CHECK(tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_AP, &info));
	ESP_ERROR_CHECK(tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_AP));

}

void wifi_init_softap()
{	uint8_t mac[6] = MACADDR;
    tcpip_adapter_init();
    wifi_dhsp_conf();
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = ESP_WIFI_SSID,
            .ssid_len = strlen(ESP_WIFI_SSID),
            .password = ESP_WIFI_PASS,
            .max_connection = MAX_CONN,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_mac(ESP_IF_WIFI_AP,mac));
	ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_softap finished. SSID:%s password:%s",
             ESP_WIFI_SSID, ESP_WIFI_PASS);

}

int server_init(void){
	wifi_init_softap();
	xTaskCreate(tcp_server_task, "tcp_server", 4096, NULL, 5, &server_tcp_task_hdl);
	return 0;
}

