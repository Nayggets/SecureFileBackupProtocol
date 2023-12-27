#include "../wrapper.h"


void initfilter();
int cleanfilter();

int sendfile(sfbp_session_t* ssl,char* path);

int autosend(sfbp_session_t* sfbp_session);

int sendfolder(sfbp_session_t* ssl,char* path);