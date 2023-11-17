#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>

#define RSA_KEY_BITS 2048

// Function to encrypt a string using RSA
char* encryptRSA(const char* plainText, RSA* publicKey) {
    int rsa_len = RSA_size(publicKey);
    char* encryptedText = (char*)malloc(rsa_len + 1);

    int result = RSA_public_encrypt(strlen(plainText), (unsigned char*)plainText, (unsigned char*)encryptedText, publicKey, RSA_PKCS1_PADDING);
    if (result == -1) {
        fprintf(stderr, "Encryption failed\n");
        return NULL;
    }

    return encryptedText;
}

// Function to decrypt a string using RSA
char* decryptRSA(const char* encryptedText, RSA* privateKey) {
    int rsa_len = RSA_size(privateKey);
    char* decryptedText = (char*)malloc(rsa_len + 1);

    int result = RSA_private_decrypt(rsa_len, (unsigned char*)encryptedText, (unsigned char*)decryptedText, privateKey, RSA_PKCS1_PADDING);
    if (result == -1) {
        fprintf(stderr, "Decryption failed\n");
        return NULL;
    }

    return decryptedText;
}

int main() {
    // Generate RSA key pair
    RSA *keypair = RSA_generate_key(RSA_KEY_BITS, 65537, NULL, NULL);
    
    // Original string
    const char *originalText = "Hello, RSA!";

    // Encrypt the string
    char *encryptedText = encryptRSA(originalText, keypair);
    if (!encryptedText) {
        RSA_free(keypair);
        return 1;
    }
    printf("Encrypted: %s\n", encryptedText);

    // Decrypt the string
    char *decryptedText = decryptRSA(encryptedText, keypair);
    if (!decryptedText) {
        free(encryptedText);
        RSA_free(keypair);
        return 1;
    }
    printf("Decrypted: %s\n", decryptedText);

    // Clean up
    free(encryptedText);
    free(decryptedText);
    RSA_free(keypair);

    return 0;
}
