#ifndef __RECEIVER_H
#define __RECEIVER_H
#include "../wrapper.h"

int receivestation(sfbp_session_t* sfbp_session,int in_restoration);

int receivefile(sfbp_session_t* sfbp_session,int in_restoration);
int receivefolder(sfbp_session_t* sfbp_session);

#endif