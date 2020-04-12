////{
////    "age": "WEB",
////    "children": [
////        {
////            "X": "Alice",
////            "Y": 6,
////            "Z": 2
////        }
////    ]
////}
////
////
////    sprintf (str,"{ \"name\": \"%-1s\", \"X\": %f, \"Y\": %4.2f, \"Z\": %4.2f }", "one",(float)1, 212312., 3.1231123);
//#include "header.h"
//static const char *TAG = "json_re";
//
//char *json_dump(struct date_i2c *date,char *str,size_t len_str){
//	memset(str,0,len_str);
//    sprintf (str,"{ \"name\": \"%-1s\", \"X\": %i, \"Y\": %i, \"Z\": %i }", "acel",date->x,date->y,date->z);
//    return &str;
//}
//
//void json_serilizete_task(void *Point){
//	char str[128];
//	struct date_i2c *date;
//	memset(str,0,128);
//	for(;;){
//		memset(str,0,128);
//		   if( Qdata_mpu != NULL )
//		   {
//			   if(xQueueReceive( Qdata_mpu,&( date ),100 / portTICK_RATE_MS)){
//				   sprintf (str,"{ \"name\": \"%-1s\", \"X\": %i, \"Y\": %i, \"Z\": %i }", "acel",date->x,date->y,date->z);
//			   }
//
////			   str=json_dump(&date,&str[0],(size_t) 128);
//
//			   ESP_LOGI(TAG,"%s",str);
//		   }
//
//	}
//    vTaskDelete(NULL);
//}
