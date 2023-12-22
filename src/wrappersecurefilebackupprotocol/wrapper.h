#ifndef __WRAPPER_H
#define __WRAPPER_H 
#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <openssl/ssl.h>
#include "../crypto/aes/aes.h"
typedef struct sfbp_session
{
    SSL* ssl;
    unsigned char key[32];
    unsigned char iv[16];
    int cryption_active;
}sfbp_session_t;

#include "./receiver/receiver.h"
#include "./sender/sender.h"

#include "./restorer/restorer.h"
#include "../paquet/paquet.h"


#endif