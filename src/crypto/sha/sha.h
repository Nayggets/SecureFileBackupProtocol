#ifndef __SHA_H
#define __SHA_H
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
#include <stdio.h>

void digest_message(const unsigned char *message, size_t message_len, unsigned char **digest, unsigned int *digest_len);
#endif