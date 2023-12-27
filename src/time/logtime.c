#include "logtime.h"

time_t current_time;

char* actualtime()
{
    current_time = time(NULL);
    char* time = ctime(&current_time);
    time[strlen(time)-1] = '\0';
    return time;
}