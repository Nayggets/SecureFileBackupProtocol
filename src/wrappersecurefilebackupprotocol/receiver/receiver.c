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
    if(stat (file->path, &buffer) == 0){
        printf("file exist \n\r");
        printf("Temps fichier serveur = %s\n\r", ctime(&buffer.st_mtime));
        printf("Temps fichier client = %s\n\r", ctime(&file->last_mod));

        if(buffer.st_mtime < file->last_mod){

            return 1;
        }
    }
    else{
        printf("file not exist\n\r");
            fflush(stdout);

        return 1;
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
    if(in_restoration || file_exists_and_no_mod(&file_received)){
        int fd = open(file_received.path,O_WRONLY | O_CREAT  | O_TRUNC,0644);
        if(fd == -1){
            printf("%s \n",file_received.path);
            perror("open");
        }

        printf("file_received size %ld\n\r",file_received.file_size);
        unsigned char file_data_buffer[4112];
        unsigned char decipher_data_buffer[4096];
        int readed_byte = 0;
        unsigned long diff = 0;
        unsigned long total_readed_byte = 0;
        do {
            diff = file_received.file_size - total_readed_byte;
            fflush(stdout);
            if(diff > 4112){
                readed_byte = SSL_read(sfbp_session->ssl,file_data_buffer,4112);
                if(readed_byte <= 0){
                    perror("SSL_read");
                    fflush(stdout);
                    return -1;
                }
            }
            else if (diff < 4112 && diff != 0){
                readed_byte = SSL_read(sfbp_session->ssl,file_data_buffer,diff);
                if(readed_byte <= 0){
                    perror("SSL_read");
                    fflush(stdout);

                    return -1;
                }
            }
            if(diff != 0){
                if(sfbp_session->cryption_active == 1){
                    int decryptLength = decrypt(file_data_buffer,readed_byte,sfbp_session->key,sfbp_session->iv,decipher_data_buffer);
                    if(decryptLength == -1){
                        perror("decrypt");
                    }
                    printf("crypted length %d\n\r",readed_byte);
                    printf("decrypted length%d\n\r",decryptLength);
                    int writed_byte = write(fd,decipher_data_buffer,decryptLength);
                    if(writed_byte <= 0){
                        perror("write");
                        fflush(stdout);

                        close(fd);
                        return -1;
                    }
                }
                else{
                    int writed_byte = write(fd,file_data_buffer,readed_byte);
                    if(writed_byte <= 0){
                        perror("write");
                        fflush(stdout);

                        close(fd);
                        return -1;
                    }
                }

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
    printf("folder receive : %s\n",folder_receive.path);

    int check = mkdir(folder_receive.path,0700);
    if(check != 0){
        perror("mkdir");
    }
    return 0;
}