#include <iostream>
#include <string>
#include <vector>

// ��������� ������������� ����� (salt)
std::string generateKey(const std::string& baseKey, size_t length) {
    std::string key;
    for (size_t i = 0; i < length; ++i) {
        key += baseKey[i % baseKey.size()]; // ��������� ���� �� ������ �����
    }
    return key;
}

// ���������� ������
std::string encrypt(const std::string& plaintext, const std::string& baseKey) {
    std::string key = generateKey(baseKey, plaintext.size()); // ���������� ����
    std::string ciphertext = plaintext;

    for (size_t i = 0; i < plaintext.size(); ++i) {
        ciphertext[i] ^= key[i]; // XOR � ������
    }
    return ciphertext;
}

// ����������� ������
std::string decrypt(const std::string& ciphertext, const std::string& baseKey) {
    return encrypt(ciphertext, baseKey); // XOR �������� �����������
}

int main() {
    setlocale(0, "");
    // �������� �����
    std::string plaintext = "password123";
    std::string baseKey = "my_secret_key"; // ������� ����

    // ����������
    std::string encrypted = encrypt(plaintext, baseKey);
    std::cout << "������������� �����: " << encrypted << std::endl;

    // �����������
    std::string decrypted = decrypt(encrypted, baseKey);
    std::cout << "�������������� �����: " << decrypted << std::endl;

    return 0;
}