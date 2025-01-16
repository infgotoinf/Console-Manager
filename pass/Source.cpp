#include <iostream>
#include <string>
#include <openssl/evp.h>
#include <openssl/rand.h>

// Константы
const int KEY_SIZE = 32;  // Размер ключа (256 бит)
const int BLOCK_SIZE = 16; // Размер блока (128 бит)

// Генерация ключа и инициализационного вектора
void generateKeyAndIV(unsigned char* key, unsigned char* iv) {
    RAND_bytes(key, KEY_SIZE);  // Генерация случайного ключа
    RAND_bytes(iv, BLOCK_SIZE); // Генерация случайного IV
}

// Шифрование строки
std::string encrypt(const std::string& plaintext, const unsigned char* key, const unsigned char* iv) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    std::string ciphertext;
    int len;
    int ciphertext_len;

    // Инициализация контекста для AES-256-CBC
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv);

    // Шифрование данных
    unsigned char outbuf[plaintext.size() + BLOCK_SIZE];
    EVP_EncryptUpdate(ctx, outbuf, &len, reinterpret_cast<const unsigned char*>(plaintext.c_str()), plaintext.size());
    ciphertext_len = len;

    // Завершение шифрования
    EVP_EncryptFinal_ex(ctx, outbuf + len, &len);
    ciphertext_len += len;

    // Конвертация в строку
    ciphertext = std::string(reinterpret_cast<char*>(outbuf), ciphertext_len);

    EVP_CIPHER_CTX_free(ctx);
    return ciphertext;
}

// Расшифровка строки
std::string decrypt(const std::string& ciphertext, const unsigned char* key, const unsigned char* iv) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    std::string plaintext;
    int len;
    int plaintext_len;

    // Инициализация контекста для AES-256-CBC
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv);

    // Расшифровка данных
    unsigned char outbuf[ciphertext.size() + BLOCK_SIZE];
    EVP_DecryptUpdate(ctx, outbuf, &len, reinterpret_cast<const unsigned char*>(ciphertext.c_str()), ciphertext.size());
    plaintext_len = len;

    // Завершение расшифровки
    EVP_DecryptFinal_ex(ctx, outbuf + len, &len);
    plaintext_len += len;

    // Конвертация в строку
    plaintext = std::string(reinterpret_cast<char*>(outbuf), plaintext_len);

    EVP_CIPHER_CTX_free(ctx);
    return plaintext;
}

int main() {
    // Исходный текст
    std::string plaintext = "password123";

    // Ключ и IV
    unsigned char key[KEY_SIZE];
    unsigned char iv[BLOCK_SIZE];
    generateKeyAndIV(key, iv);

    // Шифрование
    std::string encrypted = encrypt(plaintext, key, iv);
    std::cout << "Зашифрованный текст: " << encrypted << std::endl;

    // Расшифровка
    std::string decrypted = decrypt(encrypted, key, iv);
    std::cout << "Расшифрованный текст: " << decrypted << std::endl;

    return 0;
}