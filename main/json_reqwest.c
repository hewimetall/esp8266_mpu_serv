/*
 * json_reqwest.c
 *
 *      Author: j
 */

/*
 * { name: test,
 * 	 var : {
 * 	 		x:12,
 * 	 		y:4,
 * 	 		z:4
 * 	 		}
 * 	}
 */
#include "header.h"

#include "cJSON.h"
static const char *TAG = "Cjson";
#define DEBUG 1

char *create_monitor(char verbose_name,int is_date)
{
	const unsigned int resolution_numbers[3][2] = {
			{1280, 720,12},
			{1920, 1080,32},
			{3840, 2160,-11}
	};
	char *string = NULL;
	cJSON *type_connect = NULL;
	cJSON *resolutions = NULL;
	cJSON *resolution = NULL;
	cJSON *x= NULL;
	cJSON *y= NULL;
	cJSON *z= NULL;
	cJSON *date= NULL;

	size_t index = 0;

	cJSON *monitor = cJSON_CreateObject();
	if (monitor == NULL)
	{
		goto end;
	}

	type_connect = cJSON_CreateString(verbose_name);
	if (type_connect== NULL)
	{
		goto end;
	}

	cJSON_AddItemToObject(monitor, "type_connect", type_connect);

	resolutions = cJSON_CreateArray();
	if (resolutions == NULL)
	{
		goto end;
	}
	cJSON_AddItemToObject(monitor, "resolutions", resolutions);

	resolution = cJSON_CreateObject();
	if (resolution == NULL)
	{
		goto end;
	}
	cJSON_AddItemToArray(resolutions, resolution);
	for (index = 0; index < (sizeof(resolution_numbers) / (3 * sizeof(int))); ++index){
		/////////////////////////////////////////////////////////////////////////
		x = cJSON_CreateNumber(resolution_numbers[index][0]);
		if (x == NULL)
		{
			goto end;
		}
		cJSON_AddItemToObject(resolution, "x", x);
		/////////////////////////////////////////////////////////////////////////
		y = cJSON_CreateNumber(resolution_numbers[index][1]);
		if (y == NULL)
		{
			goto end;
		}
		cJSON_AddItemToObject(resolution, "y", y);
		/////////////////////////////////////////////////////////////////////////
		z = cJSON_CreateNumber(resolution_numbers[index][2]);
		if (z == NULL) {
			goto end;
		}
		cJSON_AddItemToObject(resolution, "z", z);
		/////////////////////////////////////////////////////////////////////////
		if(is_date == 1){
			date = cJSON_CreateString(verbose_name);
			if (date== NULL)
			{
				goto end;
			}
			cJSON_AddItemToObject(monitor, "date", date);
		}
	}
	string = cJSON_Print(monitor);
	if (string == NULL)
	{
		    	ESP_LOGE(TAG,"Failed to print monitor.\n");
	}

	end:
#ifdef DEBUG
	ESP_LOGI(TAG,"json f.\n");
	ESP_LOGI(TAG,"json f %s .\n", string);
#endif
	cJSON_Delete(monitor);
	return string;
}
