#ifndef __PAQUET_H
#define __PAQUET_H
#include <linux/limits.h>
#include <unistd.h>
#include <time.h>
typedef unsigned char byte;

//Command pattern 

typedef enum paquet_type
{
    FILE_PAQUET = 0x00,
    FOLDER_PAQUET = 0x01,
    FOLDER_RESTITUTION = 0x02,
    FILE_RESTITUTION = 0x03,
    END_OF_COMMUNICATION = 0x04,
    ERROR = 0xFF
    
}paquet_type_t;


typedef struct paquet
{
    int type_paquet;
    
} paquet_t;

typedef struct file  
{
    unsigned long file_size;
    time_t last_mod;
    char path[PATH_MAX];
} file_t;

typedef struct folder 
{
    char path[PATH_MAX];
} folder_t;


//error code not implemented anymore
typedef enum error_code
{
    FOLDER_NOT_EXIST = 0x00,
    FILE_NOT_EXIST = 0x01,
    SERVER_FULL = 0x02
} error_code_t;


typedef struct error 
{
    error_code_t error_code;
} error_t;
#endif