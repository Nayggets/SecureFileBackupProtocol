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


void menu(SSL* ssl)
{
    char number[2];
    char path[PATH_MAX];
    int check = 0;
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
                sendfile(ssl,path);
                break;
            }
            case SEND_FOLDER:
            {
                printf("Enter the path of the folder : ");
                read_line(path);
                sendfolder(ssl,path);
                break;
            }
            case RESTITUTE_FILE:
            {
                askforfile(ssl);
                receivestation(ssl,1);

                break;
            }
            case RESTITUTE_FOLDER:
            {
                askforfolder(ssl);
                receivestation(ssl,1);

                break;
            }
            case QUIT:
            {
                paquet_t end;
                end.type_paquet = END_OF_COMMUNICATION;
                SSL_write(ssl,&end,sizeof(paquet_t));
                printf("application quited\n");
                break;
            }
        }
    }


}