#include "../menu/menu.h"
#include <openssl/ssl.h>






int main()
{
    int sockfd;
    struct sockaddr_in servaddr;
    SSL_CTX *ctx;
    SSL *ssl;
    ctx = SSL_CTX_new(TLS_client_method());

    SSL_CTX_use_certificate_file(ctx, "mycert.pem", SSL_FILETYPE_PEM);

 
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
    //char IP[15];
    //printf("Rentrez l'ip : ");
    //int ip_size = read(1,IP,15);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
    
     ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sockfd);
    // connect the client socket to server socket
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))
        != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");
    SSL_connect(ssl);

    initfilter();
    menu(ssl);
    cleanfilter();
    // close the socket
    close(sockfd);
        SSL_free(ssl);

    SSL_CTX_free(ctx);
}