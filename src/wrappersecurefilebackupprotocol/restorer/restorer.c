#include "restorer.h"


int restorefile(SSL* ssl)
{
    file_t file;
    SSL_read(ssl,&file,sizeof(file));
    sendfile(ssl,file.path);
    paquet_t paquet_end;
    paquet_end.type_paquet = END_OF_COMMUNICATION;
    SSL_write(ssl,&paquet_end,sizeof(paquet_t));
    return 0;
}



int restorefolder(SSL* ssl)
{

    folder_t folder;
    SSL_read(ssl,&folder,sizeof(folder));
    sendfolder(ssl,folder.path);
    paquet_t paquet_end;
    paquet_end.type_paquet = END_OF_COMMUNICATION;
    SSL_write(ssl,&paquet_end,sizeof(paquet_t));
    return 0;
}
