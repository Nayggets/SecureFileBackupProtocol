#include "receiver.h"
int receivestation(sfbp_session_t* sfbp_session,int in_restoration)
{
    paquet_t paquet;
    paquet.type_paquet = 0;
    int check = 0;
    check = SSL_read(sfbp_session->ssl,&paquet,sizeof(paquet_t));
    if(check <= 0){
        return -1;
    }
    while(paquet.type_paquet != END_OF_COMMUNICATION){
        switch(paquet.type_paquet)
        {
            case FILE_PAQUET:
            {
                receivefile(sfbp_session,in_restoration);
                break;
            }
            case FOLDER_PAQUET:
            {
                receivefolder(sfbp_session);
                break;
            }
            case FILE_RESTITUTION:
            {
                restorefile(sfbp_session);
                break;
            }
            case FOLDER_RESTITUTION:
            {
                restorefolder(sfbp_session);
                break;
            }
        }
        check = SSL_read(sfbp_session->ssl,&paquet,sizeof(paquet_t));
        if(check <= 0){
            return -1;
        }
        printf("paquet type %d\n",paquet.type_paquet);

    }

    return 0;
}

int file_exists_and_no_mod (file_t* file) {
    struct stat   buffer;   
    if(stat (file->path, &buffer) != 0){
        printf("File non existant");
        return 1;
    }
    else{
        printf("Temps fichier serveur = %s", ctime(&buffer.st_mtime));
        printf("Temps fichier client = %s", ctime(&file->last_mod));

        if(buffer.st_mtime < file->last_mod){

            return 1;
        }
    }

  return 0;

}



int receivefile(sfbp_session_t* sfbp_session,int in_restoration)
{
    file_t file_received;
    int check = SSL_read(sfbp_session->ssl,&file_received,sizeof(file_t));
    if(check <= 0){
        return -1;
    }
    if(file_exists_and_no_mod(&file_received) || in_restoration){
        int fd = open(file_received.path,O_CREAT | O_WRONLY | O_TRUNC,S_IRWXU);
        if(fd == -1){
            printf("%s ",file_received.path);
            perror("open");
        }


        char file_data_buffer[4096];
        int readed_byte = 0;
        unsigned long diff = 0;
        unsigned long total_readed_byte = 0;

        do {
            diff = file_received.file_size - total_readed_byte;
            if(diff > 4096){
                readed_byte = SSL_read(sfbp_session->ssl,file_data_buffer,4096);
                if(readed_byte <= 0){
                    return -1;
                }
            }
            else{
                readed_byte = SSL_read(sfbp_session->ssl,file_data_buffer,diff);
                if(readed_byte <= 0){
                    return -1;
                }
            }
            int writed_byte = write(fd,file_data_buffer,readed_byte);
            if(writed_byte <= 0){
                perror("write");
                close(fd);
                return -1;
            }
            total_readed_byte += readed_byte;

        }
        while(total_readed_byte != file_received.file_size);
        close(fd);
    }
    else{
        char file_data_buffer[4096];
        int readed_byte = 0;
        unsigned long diff = 0;
        unsigned long total_readed_byte = 0;
        do {
            diff = file_received.file_size - total_readed_byte;
            if(diff > 4096){
                readed_byte = SSL_read(sfbp_session->ssl,file_data_buffer,4096);
            }
            else{
                readed_byte = SSL_read(sfbp_session->ssl,file_data_buffer,diff);
            }
            total_readed_byte += readed_byte;

        }
        while(total_readed_byte != file_received.file_size);
    }

    return 0;
}

int receivefolder(sfbp_session_t* sfbp_session)
{
    folder_t folder_receive;
    SSL_read(sfbp_session->ssl,&folder_receive,sizeof(folder_t));
    mkdir(folder_receive.path,0700);
    return 0;
}