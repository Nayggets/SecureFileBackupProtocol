#ifndef __RECEIVER_H
#define __RECEIVER_H
#include "../wrapper.h"

int receivestation(SSL* ssl,int in_restoration);

int receivefile(SSL* ssl,int in_restoration);
int receivefolder(SSL* ssl);

#endif