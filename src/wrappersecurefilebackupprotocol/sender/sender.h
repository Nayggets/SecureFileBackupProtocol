#include "../wrapper.h"


void initfilter();
int cleanfilter();

int sendfile(SSL* ssl,char* path);

int sendfolder(SSL* ssl,char* path);