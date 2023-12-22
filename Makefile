#Build Variable
#Compiler flag Variable
CC = gcc
C_VERSION = -std=gnu18
C_OPTIMISE_FLAGS = -pedantic -O3 
C_WARNING_FLAGS = -Wall -Werror
C_EXTERNE_LIBRARY_FLAGS = -lssl -lcrypto
C_DEBUG_FLAGS = -g
SERVER_APP_NAME = ./build/server/server.elf
CLIENT_APP_NAME = ./build/client/client.elf
#Source files
SOURCES_FILES_CLIENT = ./src/main/clientmain.c \
					   ./src/wrappersecurefilebackupprotocol/receiver/receiver.c \
					   ./src/wrappersecurefilebackupprotocol/sender/sender.c \
					   	./src/wrappersecurefilebackupprotocol/restorer/restorer.c \
					   ./src/reader/reader.c \
					   ./src/crypto/aes/aes.c \
					   ./src/crypto/sha/sha.c \
					   ./src/menu/menu.c 

SOURCES_FILES_SERVER = ./src/main/servermain.c \
					   ./src/wrappersecurefilebackupprotocol/receiver/receiver.c \
					   ./src/wrappersecurefilebackupprotocol/sender/sender.c \
					   	./src/wrappersecurefilebackupprotocol/restorer/restorer.c \
						./src/crypto/aes/aes.c \
					   ./src/crypto/sha/sha.c 
						



global_make: build_client build_server 

#Build section 
#normal build for elf
build_client:
	$(CC) $(C_VERSION) $(SOURCES_FILES_CLIENT) $(C_OPTIMISE_FLAGS) $(C_WARNING_FLAGS) $(C_EXTERNE_LIBRARY_FLAGS) -o $(CLIENT_APP_NAME)

build_server:
	$(CC) $(C_VERSION) $(SOURCES_FILES_SERVER) $(C_OPTIMISE_FLAGS) $(C_WARNING_FLAGS) $(C_EXTERNE_LIBRARY_FLAGS) -o $(SERVER_APP_NAME)
#debug build for elf
debug_build_client:
	$(CC) $(C_VERSION) $(SOURCES_FILES_CLIENT) $(C_OPTIMISE_FLAGS) $(C_WARNING_FLAGS) $(C_EXTERNE_LIBRARY_FLAGS) $(C_DEBUG_FLAGS) -o $(CLIENT_APP_NAME)

debug_build_server:
	$(CC) $(C_VERSION) $(SOURCES_FILES_SERVER) $(C_OPTIMISE_FLAGS) $(C_WARNING_FLAGS) $(C_EXTERNE_LIBRARY_FLAGS) $(C_DEBUG_FLAGS) -o $(SERVER_APP_NAME)


#Install package for the project to build
install_package:
	sudo apt-get -y update
	sudo apt-get -y install openssl
	sudo apt-get -y install gcc
	sudo apt-get -y upgrade openssl
	sudo apt-get -y upgrade gcc
	sudo apt-get -y libssl-dev
