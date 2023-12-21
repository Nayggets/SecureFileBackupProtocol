#include "../wrapper.h"


void initfilter();
int cleanfilter();

int sendfile(sfbp_session_t* ssl,char* path);

int sendfolder(sfbp_session_t* ssl,char* path);