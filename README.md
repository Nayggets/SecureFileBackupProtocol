# SecureFileBackupProtocol
A secure file backup protocol that you can use to store file in a server. Simple and efficient

## History

This is a project at the IUT of Montpellier-Sète we have to developped a simple application that can save file in a server, i realy like the subject so i add a lot of other features like send folder recursively, restoration of file and folder, securise connection using TLSv1.3 and cryption server side 

## MAIN TODO
* [x] file saving
* [x] folder saving
* [x] folder restoration 
* [x] file restoration 
* [x] Tlsv1.3 Integration 
* [x] AES 256 for storing encrypted data on the server side and authenticate tag to prevent modification.
* [x] Key Generation using sentence
* [x] Menu
* [x] Makefile 
* [x] Installation Script for Client
* [x] Installation Script for Server
* [x] Don't rewrite the file if it hasn't been modified since the last backup.
* [ ] Versionning
* [ ] list the files stored on the server side for the client.
* [ ] Deleting file or folder the client don't want to save anymore


### Architecture Diagram

![Untitled-2023-04-13-1857(1)](https://github.com/Nayggets/SecureFileBackupProtocol/assets/79973358/e12162a7-0dd0-4f6d-b7f4-dab07fa9a3cd)


## Thanks
### Thanks to Francis Garcia, a professor at the IUT of Montpellier-Sète
for accepting my end-of-module project idea and enabling me to carry it out. I'm grateful for his continuous critical feedback throughout the development, offering improvement suggestions, and highlighting the strong aspects of my project.
Also, thank you for teaching me that a proper backup is a very complex task, involving both security demands and the implementation of numerous mechanisms.
