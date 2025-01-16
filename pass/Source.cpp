#include <iostream>
#include <string>
#include <openssl/evp.h>
#include <openssl/rand.h>

// ���������
const int KEY_SIZE = 32;  // ������ ����� (256 ���)
const int BLOCK_SIZE = 16; // ������ ����� (128 ���)

// ��������� ����� � ������������������ �������
void generateKeyAndIV(unsigned char* key, unsigned char* iv) {
    RAND_bytes(key, KEY_SIZE);  // ��������� ���������� �����
    RAND_bytes(iv, BLOCK_SIZE); // ��������� ���������� IV
}

// ���������� ������
std::string encrypt(const std::string& plaintext, const unsigned char* key, const unsigned char* iv) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    std::string ciphertext;
    int len;
    int ciphertext_len;

    // ������������� ��������� ��� AES-256-CBC
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv);

    // ���������� ������
    unsigned char outbuf[plaintext.size() + BLOCK_SIZE];
    EVP_EncryptUpdate(ctx, outbuf, &len, reinterpret_cast<const unsigned char*>(plaintext.c_str()), plaintext.size());
    ciphertext_len = len;

    // ���������� ����������
    EVP_EncryptFinal_ex(ctx, outbuf + len, &len);
    ciphertext_len += len;

    // ����������� � ������
    ciphertext = std::string(reinterpret_cast<char*>(outbuf), ciphertext_len);

    EVP_CIPHER_CTX_free(ctx);
    return ciphertext;
}

// ����������� ������
std::string decrypt(const std::string& ciphertext, const unsigned char* key, const unsigned char* iv) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    std::string plaintext;
    int len;
    int plaintext_len;

    // ������������� ��������� ��� AES-256-CBC
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv);

    // ����������� ������
    unsigned char outbuf[ciphertext.size() + BLOCK_SIZE];
    EVP_DecryptUpdate(ctx, outbuf, &len, reinterpret_cast<const unsigned char*>(ciphertext.c_str()), ciphertext.size());
    plaintext_len = len;

    // ���������� �����������
    EVP_DecryptFinal_ex(ctx, outbuf + len, &len);
    plaintext_len += len;

    // ����������� � ������
    plaintext = std::string(reinterpret_cast<char*>(outbuf), plaintext_len);

    EVP_CIPHER_CTX_free(ctx);
    return plaintext;
}

int main() {
    // �������� �����
    std::string plaintext = "password123";

    // ���� � IV
    unsigned char key[KEY_SIZE];
    unsigned char iv[BLOCK_SIZE];
    generateKeyAndIV(key, iv);

    // ����������
    std::string encrypted = encrypt(plaintext, key, iv);
    std::cout << "������������� �����: " << encrypted << std::endl;

    // �����������
    std::string decrypted = decrypt(encrypted, key, iv);
    std::cout << "�������������� �����: " << decrypted << std::endl;

    return 0;
}