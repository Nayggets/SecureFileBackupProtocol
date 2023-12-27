make install_package 
make build_server
mkdir /home/$USER/server

openssl req -x509 -nodes -days 365 -newkey rsa:2048 -sha256 -out /home/$USER/server/server.crt -keyout /home/$USER/server/server.key  1>/dev/null
cp ./build/server/server.elf /home/$USER/server/server.elf