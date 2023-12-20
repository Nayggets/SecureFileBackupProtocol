#ifndef __RECEIVER_H
#define __RECEIVER_H
#include "../wrapper.h"

int receivestation(int sockfd,int in_restoration);

int receivefile(int sockfd,int in_restoration);
int receivefolder(int sockfd);

#endif