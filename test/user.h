#pragma once

#include <iostream>

class user {
private:
	std::string login; // Обязательный
	std::string password; // Обязательный; должен быть зашифрован; hash-salt, sha256-hash или аналогичные
	std::string access_level; // Admin; Manager; User
	std::string name;
	std::string email;
	std::string status; // Active; Needs correction; Not active; Deleted
public:
	// По дефолту access_level = "User", name = "", mail = "", status = "Needs correction"
	user(char log[32], char pas[32], char acl[16] = const_cast<char*>("User"), char nam[16] = const_cast<char*>(" "), char eml[32] = const_cast<char*>(" "), char stt[18] = const_cast<char*>("Needs correction"))
	{
		login = log;
		password = pas;
		access_level = acl;
		name = nam;
		email = eml;
		status = stt;
		if (// Проверка на то, что статус введён корректно
			!(status == "Active" || status == "Needs correction" || status == "Not active" || status == "Deleted") ||
			// Проверка на то, что уровень доступа введён корректно
			!(access_level == "Admin" || access_level == "Manager" || access_level == "User") ||
			//Проверка на то, что имя и имэил введены
			name == " " || email == " ")
		{
			status = "Needs correction"; // Если хоть один из тестов выше не был пройден, то присваивается статус "Needs correction"
		}
	}

	auto get_login() { return login; }
	auto get_password() { return password; }
	auto get_access_level() { return access_level; }
	auto get_name() { return name; }
	auto get_email() { return email; }
	auto get_status() { return status; }
};