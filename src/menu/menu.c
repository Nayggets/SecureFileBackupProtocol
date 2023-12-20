#include "menu.h"


int askforfile(int sockfd)
{
    paquet_t paquet;
    paquet.type_paquet = FILE_RESTITUTION;
    send(sockfd,&paquet,sizeof(paquet_t),0);
    file_t file;
    read_line(file.path);
    printf("the path %s",file.path);
    send(sockfd,&file,sizeof(file_t),0);
    return 0;
}
int askforfolder(int sockfd)
{
    paquet_t paquet;
    paquet.type_paquet = FOLDER_RESTITUTION;
    send(sockfd,&paquet,sizeof(paquet_t),0);
    folder_t folder;
    read_line(folder.path);
    printf("the path %s",folder.path);
    send(sockfd,&folder,sizeof(folder_t),0);
    return 0;
}


void menu(int sockfd)
{
    char number[2];
    char path[PATH_MAX];
    int check = 0;
    while(atoi(number) != QUIT)
    {
        printf("what do you want to do?\n");
        printf("Send file          1.\n");
        printf("Send folder        2.\n");
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
                sendfile(sockfd,path);
                break;
            }
            case SEND_FOLDER:
            {
                printf("Enter the path of the folder : ");
                read_line(path);
                sendfolder(sockfd,path);
                break;
            }
            case RESTITUTE_FILE:
            {
                askforfile(sockfd);
                receivestation(sockfd,1);

                break;
            }
            case RESTITUTE_FOLDER:
            {
                askforfolder(sockfd);
                receivestation(sockfd,1);

                break;
            }
            case QUIT:
            {
                paquet_t end;
                end.type_paquet = END_OF_COMMUNICATION;
                send(sockfd,&end,sizeof(paquet_t),0);
                printf("application quited\n");
                break;
            }
        }
    }


}