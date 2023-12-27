#include "sender.h"

int isdirectory(const char *path) {
   struct stat statbuf;
   if (stat(path, &statbuf) != 0)
       return 0;
   return S_ISDIR(statbuf.st_mode);
}
static  char **extension_accept;


void initfilter()
{
    int fd = open("files.accept",O_RDONLY);
    char buffer[4096];
    int read_byte = read(fd,buffer,4096);
    buffer[read_byte] = '\0';
    int extension_number = 0;
    for(int i = 0 ; i < read_byte ; i++){
        if(buffer[i] == '.'){
            extension_number++;
        }
    }
    extension_accept = malloc(sizeof(char*) * extension_number);
    char *toTread = strtok(buffer,".");
    int j = 0;
    for(int i = 0 ; i < extension_number ; i++){
        while(toTread[j] != '\0' && toTread[j] != '\n'){
            j++;
        }
        extension_accept[i] = malloc(sizeof(char) * j+1);
    
        memcpy(extension_accept[i],toTread,j);
        extension_accept[i][j] = '\0';
        j = 0;
        toTread = strtok(NULL,".");

    }
    extension_accept[extension_number] = NULL;
    
}

int filter(char* name,int size)
{
    if(extension_accept == NULL){
        return 1;
    }
    int i;
    for(i = size; i != 0 && name[i] != '.' ; i--);
    i++;
    int j = 0;
    while(extension_accept[j] != NULL){
        if(strcmp(name+i,extension_accept[j]) == 0){
            return 1;
        }
        j++;
    }
    return 0;

}

int cleanfilter()
{
    int j = 0;
    while(extension_accept[j] != NULL){
        free(extension_accept[j]);
        j++;
    }
    free(extension_accept);
    return 0;
}


int gettime(char* path,file_t* file)
{

    struct stat   buffer;   
    if(stat (path, &buffer) != 0){

        return -1;
    }
    else{
        if(file != NULL){
            file->last_mod = buffer.st_mtime;
        }
        return 0;
    }

}


int autosend(sfbp_session_t* sfbp_session)
{
    FILE *fichier;
    char ligne[PATH_MAX]; // Taille maximale de la ligne

    // Ouvre le fichier en mode lecture ("r")
    fichier = fopen("autosave", "r");

    // Vérifie si le fichier est ouvert avec succès
    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier.\n");
        return -1; // Quitte le programme avec un code d'erreur
    }
    printf("Launching automatic backup of launch files this may take a few seconds or even a minute depend on your autosave file\n");
    int i = 0;
    // Lit chaque ligne du fichier et l'affiche
    while (fgets(ligne, PATH_MAX, fichier) != NULL) {
        i = 0;
        while(ligne[i] != '\n'){
            i++;
        }
        ligne[i] = '\0';
        printf("%s\n",ligne);

        if(isdirectory(ligne) > 0){
            printf("auto send folder %s \n",ligne);
            sendfolder(sfbp_session,ligne);
        }
        else{
            printf("auto send file %s \n",ligne);
            sendfile(sfbp_session,ligne);
        }
    }

    // Ferme le fichier après avoir terminé la lecture
    fclose(fichier);

    return 0; // Fin du programme

}





int sendfile(sfbp_session_t* sfbp_session,char* path)
{
    file_t file;
    int check = 0;
    if(gettime(path,&file) != 0){
        return -1;
    }
    paquet_t paquet;
    paquet.type_paquet = FILE_PAQUET;
    check = SSL_write(sfbp_session->ssl,&paquet,sizeof(paquet));
    if(check == -1){
        return -1;
    }
    printf("Fichier en cour d'envois : %s\n", path);
    memcpy(file.path,path,PATH_MAX);
    int fd = open(path,O_RDONLY);
    if(fd == -1){
        perror("open");

    }
    off_t size = lseek(fd,0,SEEK_END);
    if(size == -1){
        perror("lseek");
    } 

    lseek(fd,0,SEEK_SET);



            /*
            if(size < 16){
                file.file_size = 16;
            }
            else{
                file.file_size = size + (((size / 4096)) * 16);
                file.file_size += (16 - file.file_size % 16);
                printf("size %ld + size / 4096 * 16 %ld + size modulo 16 %ld file crypt : %ld",size,(size/4096)*16,size%16,file.file_size);
            }
            file.file_size = size;

            printf("file size : %ld",file.file_size);
            fflush(stdout);
            */
    if(sfbp_session->cryption_active == 1 && size != 0){
        file.file_size = size + ((size / 4096)* 16);
        if(file.file_size % 16 == 0){
            file.file_size++;
        }
        while(file.file_size % 16 != 0){
            file.file_size++;
        }
        printf("Size : %ld\n",file.file_size);

    }
    else{
        file.file_size = size;

    }

    int read_size = -1;
    SSL_write(sfbp_session->ssl,&file,sizeof(file_t));
        unsigned char buffer[4112];
        unsigned char cipher_buffer[4112];

        int cipher_length = 0;
        int total_size = 0;
    while(read_size != 0){
        if(sfbp_session->cryption_active == 1){
            read_size = read(fd,buffer,4096);
            if(read_size > 0){

                cipher_length = encrypt(buffer,read_size,sfbp_session->key,sfbp_session->iv,cipher_buffer);
                total_size += cipher_length;
                int check = SSL_write(sfbp_session->ssl,cipher_buffer,cipher_length);
                if(check <= 0){
                    perror("write");
                }
            }

        }
        else{
            read_size = read(fd,buffer,4112);
            if(read_size > 0){
                int check = SSL_write(sfbp_session->ssl,buffer,read_size);
                if(check <= 0){
                    perror("write");
                }
            }

        }
    }
    fflush(stdout);
    close(fd);
    return 0;
}

int sendfolder(sfbp_session_t* sfbp_session,char* path)
{
    if(gettime(path,NULL) == -1){
        printf("Dossier inexistant");
        return -1;
    }
    struct dirent *dir;
    // opendir() renvoie un pointeur de type DIR. 
    DIR *d = opendir(path); 
    printf("directory open %s\n\r",path);
    paquet_t paquet;
    paquet.type_paquet = FOLDER_PAQUET;
    SSL_write(sfbp_session->ssl,&paquet,sizeof(paquet));
    folder_t folder;
    memcpy(folder.path,path,PATH_MAX);
    printf("directory send %s\n\r",folder.path);

    SSL_write(sfbp_session->ssl,&folder,sizeof(folder));

    char realPath[PATH_MAX];
    int size = strlen(path);
    
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if(strcmp(dir->d_name,".\0") != 0 && strcmp(dir->d_name,"..\0") != 0){
                for(int i = 0 ; i < PATH_MAX ; i++){
                    realPath[i] = '\0';
                }
                memcpy(realPath,path,size);
                strcpy(realPath+size,dir->d_name);

                if(isdirectory(realPath)){
                    memcpy(realPath+size+strlen(dir->d_name),"/",1);
                    sendfolder(sfbp_session,realPath);
                }
                else{
                    if(filter(dir->d_name,strlen(dir->d_name))){
                        sendfile(sfbp_session,realPath);
                    }
                }
                printf("%s\n", dir->d_name);
            }

        }
        closedir(d);
    }

    return 0;
}