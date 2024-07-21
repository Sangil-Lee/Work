#include <fstream>
#include <iostream>
#include <string>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rand.h>

bool encrypt(const std::string& plaintext, std::string& ciphertext, const std::string& key, const std::string& iv) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return false;

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, (unsigned char*)key.c_str(), (unsigned char*)iv.c_str())) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    int len;
    unsigned char outbuf[1024];
    if (1 != EVP_EncryptUpdate(ctx, outbuf, &len, (unsigned char*)plaintext.c_str(), plaintext.length())) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    ciphertext.assign((char*)outbuf, len);

    if (1 != EVP_EncryptFinal_ex(ctx, outbuf, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    ciphertext.append((char*)outbuf, len);

    EVP_CIPHER_CTX_free(ctx);
    return true;
}

void encryptDataToFile(const std::string& filename, const std::string& data, const std::string& key, const std::string& iv) {
    std::string encryptedData;
    if (!encrypt(data, encryptedData, key, iv)) {
        std::cerr << "Encryption failed" << std::endl;
        return;
    }

    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        std::cerr << "Failed to open file for writing" << std::endl;
        return;
    }

    outFile.write(encryptedData.c_str(), encryptedData.size());
    outFile.close();
}

int main() {
    std::string key = "01234567890123456789012345678901"; // 32 bytes key for AES-256
    std::string iv = "0123456789012345"; // 16 bytes IV for AES-256

    std::string data = "This is the data to be encrypted";
    std::string filename = "encrypted_data.bin";

    encryptDataToFile(filename, data, key, iv);

    return 0;
}

//g++ -o test main.cpp -lssl -lcrypto

