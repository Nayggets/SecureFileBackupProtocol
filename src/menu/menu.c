#include "menu.h"


int askforfile(SSL* ssl)
{
    paquet_t paquet;
    paquet.type_paquet = FILE_RESTITUTION;
    SSL_write(ssl,&paquet,sizeof(paquet_t));
    file_t file;
    read_line(file.path);
    printf("the path %s",file.path);
    SSL_write(ssl,&file,sizeof(file_t));
    return 0;
}
int askforfolder(SSL* ssl)
{
    paquet_t paquet;
    paquet.type_paquet = FOLDER_RESTITUTION;
    SSL_write(ssl,&paquet,sizeof(paquet_t));
    folder_t folder;
    read_line(folder.path);
    printf("the path %s",folder.path);
    SSL_write(ssl,&folder,sizeof(folder_t));
    return 0;
}

int file_exists (char *filename) {
  struct stat   buffer;   
  return (stat (filename, &buffer) == 0);
}


void menu(sfbp_session_t* sfbp_session)
{
    int check = 0;
    printf("HERE\n");
    fflush(stdout);
    if(!file_exists("userkey.key")){
        printf("Entrer une phrase attention il faut vous souvenir de cette phrase en cas de changement de machine ou remise a 0 de cette dernière : ");
        fflush(stdout);
        char message[256];
        int size = read(1,message,256);
        message[size] = '\0';
        digest_message((unsigned char*)message,29,(unsigned char**)&sfbp_session->key,(unsigned int*)&size);
        int fd = open("userkey.key",O_WRONLY | O_CREAT,0700);
        check = write(fd,sfbp_session->key,32);
    }
    else{
        int fd = open("userkey.key",O_RDONLY);
        check = read(fd,sfbp_session->key,32);
    }
    unsigned char iv[16] = { 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
                          0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35
                        };
    memcpy(sfbp_session->iv,iv,16);
    sfbp_session->cryption_active = 1;
    char number[2];
    char path[PATH_MAX];
    while(atoi(number) != QUIT)
    {
        printf("what do you want to do?\n");
        printf("send file          1.\n");
        printf("send folder        2.\n");
        printf("Restitute a file   3.\n");
        printf("Restitute a folder 4.\n");
        printf("Quit               5.\n");
        check = read(1,&number,2); // just pour eviter les warning du read on stock la valeur de retour
        if(check == -1){
            perror("read");
        }

        switch(atoi(number))
        {
            case SEND_FILE:
            {
                printf("Enter the path of the fichier : ");
                fflush(stdout);
                read_line(path);
                sendfile(sfbp_session,path);
                break;
            }
            case SEND_FOLDER:
            {
                printf("Enter the path of the folder : ");
                read_line(path);
                sendfolder(sfbp_session,path);
                break;
            }
            case RESTITUTE_FILE:
            {
                askforfile(sfbp_session->ssl);
                receivestation(sfbp_session,1);

                break;
            }
            case RESTITUTE_FOLDER:
            {
                askforfolder(sfbp_session->ssl);
                receivestation(sfbp_session,1);

                break;
            }
            case QUIT:
            {
                paquet_t end;
                end.type_paquet = END_OF_COMMUNICATION;
                SSL_write(sfbp_session->ssl,&end,sizeof(paquet_t));
                printf("application quited\n");
                break;
            }
        }
    }


}