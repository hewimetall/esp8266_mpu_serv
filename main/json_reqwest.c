//{
//    "age": "WEB",
//    "children": [
//        {
//            "X": "Alice",
//            "Y": 6,
//            "Z": 2
//        }
//    ]
//}
//
//
//    sprintf (str,"{ \"name\": \"%-1s\", \"X\": %f, \"Y\": %4.2f, \"Z\": %4.2f }", "one",(float)1, 212312., 3.1231123);
#include "header.h"

char *json_dump(char *str){
    sprintf (str,"{ \"name\": \"%-1s\", \"X\": %f, \"Y\": %4.2f, \"Z\": %4.2f }", "one",(float)1, 212312., 3.1231123);
    return str;
}
