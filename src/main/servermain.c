#include "../wrappersecurefilebackupprotocol/wrapper.h"
#define PORT 9500


int main() 
{
    int sockfd, connfd, len; 
    struct sockaddr_in servaddr, cli; 
   
    // socket create and verification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
   
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
   
    // Binding newly created socket to given IP and verification 
    if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully binded..\n"); 
   
    // Now server is ready to listen and verification 
    if ((listen(sockfd, 5)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    else
        printf("Server listening..\n"); 
    len = sizeof(cli); 
   
    // Accept the data packet from client and verification 

    while(1)
    {
        connfd = accept(sockfd, (struct sockaddr *)&cli,(socklen_t*) &len); 
        if (connfd < 0) { 
            printf("server accept failed...\n"); 
            exit(0); 
        } 
        else{
            printf("server accept the client...\n"); 
            pid_t pid = fork();
            if(pid == 0){
                receivestation(connfd,0);
                printf("Client left\n");
                exit(0);
            }

        }

    }
    // Function for chatting between client and server 
    /*
    file_t file_received;
    recv(connfd,&file_received,sizeof(file_t),0);
    

    int fd = open(file_received.path,O_CREAT | O_WRONLY ,S_IRWXU);
    char file_data_buffer[4096];
    int readed_byte = 0;
    unsigned long total_readed_byte = 0;
    do {
        readed_byte = recv(connfd,file_data_buffer,4096,0);
        int writed_byte = write(fd,file_data_buffer,readed_byte);
        if(writed_byte <= 0){
            perror("write");
        }
        total_readed_byte += readed_byte;

    }
    while(total_readed_byte != file_received.file_size);

    // After chatting close the socket 
    */
    close(sockfd); 
}