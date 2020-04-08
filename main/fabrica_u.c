///*
// * fabrica_u.c
// * 3 type
// * 		 brovser,pc -> json(x3 ,data);
// * 		 machin -> raw data(x3);
// */
//#include "header.h"
//
//#include "freertos/FreeRTOS.h"
//#include "freertos/task.h"
//#include "cJSON.h"
//static const char *TAG = "fabrica";
//#define DEBUG 1
//enum Token {
//	MACHIVE,
//	PC,
//	WEB
//};
//char *create_monitor(char verbose_name,int is_date);
//char *web(void);
//char *machine(void);
//
//char *fabric_switch(int type){
//	switch (type) {
//	case MACHIVE:
//		return machine();
//		break;
//	case WEB:
//		return web();
//		break;
//	case PC:
//
//		break;
//	default:
//		return -1;
//	}
//	return -1;
//}
//
//
//
//
///* The f return str lens -1*/
//int my_strlen(const char* str) {
//	for (const char* c = str;;c++) if (!*c) return c-str;
//}
//
//char *get_header(){
//	// status[]="HTTP/1.1 200 OK\r\n";
//	// content_type[]=content_type[28]="Content-Type: text/html\r\n";
//	// blank_page[]="\r\n";
//	static char header[]="HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
//	return &header;
//}
//
//char *machine(void){
//	char *json_body= create_monitor("WEB",0);
//#ifdef DEBUG
//	ESP_LOGI(TAG, "The machine \n%s",json_body);
//#endif
//
//	return json_body;
//}
//
//char *web(void){
//	char *header=get_header();
//	char *json_body= create_monitor("WEB",1);
//	char *con_str=malloc(my_strlen(header)+my_strlen(json_body)+2);
//	snprintf(con_str, sizeof(*con_str), "%s%s", header, json_body);
//#ifdef DEBUG
//	ESP_LOGI(TAG, "The web \n%s",con_str);
//#endif
//	return con_str;
//}
//
//
//
//
//char *create_monitor(char verbose_name,int is_date)
//{
//	const unsigned int resolution_numbers[3][2] = {
//			{1280, 720,12},
//			{1920, 1080,32},
//			{3840, 2160,-11}
//	};
//	char *string = NULL;
//	cJSON *type_connect = NULL;
//	cJSON *resolutions = NULL;
//	cJSON *resolution = NULL;
//	cJSON *x= NULL;
//	cJSON *y= NULL;
//	cJSON *z= NULL;
//	cJSON *date= NULL;
//
//	size_t index = 0;
//
//	cJSON *monitor = cJSON_CreateObject();
//	if (monitor == NULL)
//	{
//		goto end;
//	}
//
//	type_connect = cJSON_CreateString(verbose_name);
//	if (type_connect== NULL)
//	{
//		goto end;
//	}
//
//	cJSON_AddItemToObject(monitor, "type_connect", type_connect);
//
//	resolutions = cJSON_CreateArray();
//	if (resolutions == NULL)
//	{
//		goto end;
//	}
//	cJSON_AddItemToObject(monitor, "resolutions", resolutions);
//
//	resolution = cJSON_CreateObject();
//	if (resolution == NULL)
//	{
//		goto end;
//	}
//	cJSON_AddItemToArray(resolutions, resolution);
//	for (index = 0; index < (sizeof(resolution_numbers) / (3 * sizeof(int))); ++index){
//		/////////////////////////////////////////////////////////////////////////
//		x = cJSON_CreateNumber(resolution_numbers[index][0]);
//		if (x == NULL)
//		{
//			goto end;
//		}
//		cJSON_AddItemToObject(resolution, "x", x);
//		/////////////////////////////////////////////////////////////////////////
//		y = cJSON_CreateNumber(resolution_numbers[index][1]);
//		if (y == NULL)
//		{
//			goto end;
//		}
//		cJSON_AddItemToObject(resolution, "y", y);
//		/////////////////////////////////////////////////////////////////////////
//		z = cJSON_CreateNumber(resolution_numbers[index][2]);
//		if (z == NULL) {
//			goto end;
//		}
//		cJSON_AddItemToObject(resolution, "z", z);
//		/////////////////////////////////////////////////////////////////////////
//		if(is_date){
//			date = cJSON_CreateString(verbose_name);
//			if (date== NULL)
//			{
//				goto end;
//			}
//			cJSON_AddItemToObject(monitor, "date", date);
//		}
//	}
//	string = cJSON_Print(monitor);
//	if (string == NULL)
//	{
//		    	ESP_LOGI(TAG,"Failed to print monitor.\n");
//	}
//
//	end:
//	cJSON_Delete(monitor);
//	return string;
//}
