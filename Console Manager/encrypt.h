#pragma once
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