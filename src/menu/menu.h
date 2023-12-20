#ifndef __MENU_H
#define __MENU_H

#include "../wrappersecurefilebackupprotocol/wrapper.h"
#include "../reader/reader.h"

#define PORT 9500

void menu(int sockfd);

typedef enum action
{
    SEND_FILE = 0x01,
    SEND_FOLDER = 0x02,
    RESTITUTE_FILE = 0x03,
    RESTITUTE_FOLDER = 0x04,
    QUIT = 0x05
} action_t;

#endif