#include "restorer.h"


int restorefile(int sockfd)
{
    file_t file;
    recv(sockfd,&file,sizeof(file),0);
    sendfile(sockfd,file.path);
    paquet_t paquet_end;
    paquet_end.type_paquet = END_OF_COMMUNICATION;
    send(sockfd,&paquet_end,sizeof(paquet_t),0);
    return 0;
}



int restorefolder(int sockfd)
{

    folder_t folder;
    recv(sockfd,&folder,sizeof(folder),0);
    sendfolder(sockfd,folder.path);
    paquet_t paquet_end;
    paquet_end.type_paquet = END_OF_COMMUNICATION;
    send(sockfd,&paquet_end,sizeof(paquet_t),0);
    return 0;
}
