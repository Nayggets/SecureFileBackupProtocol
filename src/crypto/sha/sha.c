#include "sha.h"
void handleErrorsSha(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}


void digest_message(const unsigned char *message, size_t message_len, unsigned char **digest, unsigned int *digest_len)
{
	EVP_MD_CTX *mdctx;

	if((mdctx = EVP_MD_CTX_new()) == NULL)
		handleErrorsSha();

	if(1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL))
		handleErrorsSha();

	if(1 != EVP_DigestUpdate(mdctx, message, message_len))
		handleErrorsSha();

	if((*digest = (unsigned char *)OPENSSL_malloc(EVP_MD_size(EVP_sha256()))) == NULL)
		handleErrorsSha();

	if(1 != EVP_DigestFinal_ex(mdctx, *digest, digest_len))
		handleErrorsSha();

	EVP_MD_CTX_free(mdctx);
}