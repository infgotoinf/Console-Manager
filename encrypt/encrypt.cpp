#include <iostream>
#include <string>
#include <vector>

// Генерация динамического ключа (salt)
std::string generateKey(const std::string& baseKey, size_t length) {
    std::string key;
    for (size_t i = 0; i < length; ++i) {
        key += baseKey[i % baseKey.size()]; // Повторяем ключ до нужной длины
    }
    return key;
}

// Шифрование строки
std::string encrypt(const std::string& plaintext, const std::string& baseKey) {
    std::string key = generateKey(baseKey, plaintext.size()); // Генерируем ключ
    std::string ciphertext = plaintext;

    for (size_t i = 0; i < plaintext.size(); ++i) {
        ciphertext[i] ^= key[i]; // XOR с ключом
    }
    return ciphertext;
}

// Расшифровка строки
std::string decrypt(const std::string& ciphertext, const std::string& baseKey) {
    return encrypt(ciphertext, baseKey); // XOR работает симметрично
}

int main() {
    setlocale(0, "");
    // Исходный текст
    std::string plaintext = "password123";
    std::string baseKey = "my_secret_key"; // Базовый ключ

    // Шифрование
    std::string encrypted = encrypt(plaintext, baseKey);
    std::cout << "Зашифрованный текст: " << encrypted << std::endl;

    // Расшифровка
    std::string decrypted = decrypt(encrypted, baseKey);
    std::cout << "Расшифрованный текст: " << decrypted << std::endl;

    return 0;
}