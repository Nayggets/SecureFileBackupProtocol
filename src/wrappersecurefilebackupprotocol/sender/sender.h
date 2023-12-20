#include "../wrapper.h"


void initfilter();
int cleanfilter();

int sendfile(int sockfd,char* path);

int sendfolder(int sockfd,char* path);