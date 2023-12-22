make install_package 
make build_client
openssl req -x509 -nodes -days 365 -newkey rsa:2048 -sha256 -out /home/$USER/mycert.pem  1>/dev/null
cp ./build/client/client.elf /home/$USER/client.elf