#include "restorer.h"


int restorefile(sfbp_session_t* sfbp_session)
{
    file_t file;
    SSL_read(sfbp_session->ssl,&file,sizeof(file));
    sendfile(sfbp_session,file.path);
    paquet_t paquet_end;
    paquet_end.type_paquet = END_OF_COMMUNICATION;
    SSL_write(sfbp_session->ssl,&paquet_end,sizeof(paquet_t));
    return 0;
}



int restorefolder(sfbp_session_t* sfbp_session)
{

    folder_t folder;
    SSL_read(sfbp_session->ssl,&folder,sizeof(folder));
    sendfolder(sfbp_session,folder.path);
    paquet_t paquet_end;
    paquet_end.type_paquet = END_OF_COMMUNICATION;
    SSL_write(sfbp_session->ssl,&paquet_end,sizeof(paquet_t));
    return 0;
}
