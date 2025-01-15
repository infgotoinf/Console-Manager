#include <iostream>
#include <windows.h> 
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

namespace fs = std::filesystem;


class user {
private:
	std::string login; // ������������
	std::string password; // ������������; ������ ���� ����������; hash-salt, sha256-hash ��� �����������
	std::string access_level; // Admin; Manager; User
	std::string name;
	std::string email;
	std::string status; // Active; Needs correction; Not active; Deleted
public:
	// �� ������� access_level = "User", name = "", mail = "", status = "Needs correction"
	user(char log[32], char pas[32], char acl[8] = const_cast<char*>("User"), char nam[16] = const_cast<char*>(""), char eml[32] = const_cast<char*>(""), char stt[18] = const_cast<char*>("Needs correction"))
	{
		login = log;
		password = pas;
		access_level = acl;
		name = nam;
		email = eml;
		if (// �������� �� ��, ��� ������ ����� ���������
			!(stt == "Active" || stt == "Needs correction" || stt == "Not active" || stt == "Deleted") || 
			// �������� �� ��, ��� ������� ������� ����� ���������
			!(acl == "Admin" || acl == "Manager" || acl == "User") ||
			//�������� �� ��, ��� ��� � ����� �������
			name == "" || email == "")
		{
			status = "Needs correction"; // ���� ���� ���� �� ������ ���� �� ��� �������, �� ������������� ������ "Needs correction"
		}
		else
		{
			status = stt; // ���� �� ���� ��������� �����, �� ���������� �������� ������
		}
	}

	// ������� ������ ���������� �� ������ � �� (�� ����������) 
	nlohmann::json operator+(nlohmann::json &Database) {
		nlohmann::json Data = {
			{"name", name},
			{"login", login},
			{"email", email},
			{"status", status},
			{"name", name},
			{"password", password},
			{"access_level", access_level}
		};
		Database.push_back(Data);
		return Database;
	}
};

// ������� ������ ������ �� �� ���������� �� � ���������� �� 
void save(nlohmann::json Database) {
	std::ofstream file("Database.json");
	file << Database;
}

// ������� ������ ��
void print(nlohmann::json Database) {
	std::cout << "|name";
	SetConsoleCursorPosition(console, { 16, 0 });
	std::cout << "|login";
	SetConsoleCursorPosition(console, { 48, 0 });
	std::cout << "|email";
	SetConsoleCursorPosition(console, { 80, 0 });
	std::cout << "|status";
	SetConsoleCursorPosition(console, { 98, 0 });
	std::cout << "|\n";
	std::cout << "---------------------------------------------------------------------------------------------------";
	for (short i = 0, i2 = 2; i < Database.size(); i++, i2++) {
		SetConsoleCursorPosition(console, { 0, i2 });
		std::cout << '|' << Database[i]["name"];
		SetConsoleCursorPosition(console, { 16, i2 });
		std::cout << '|' << Database[i]["login"];
		SetConsoleCursorPosition(console, { 48, i2 });
		std::cout << '|' << Database[i]["email"];
		SetConsoleCursorPosition(console, { 80, i2 });
		std::cout << '|' << Database[i]["status"];
		SetConsoleCursorPosition(console, { 98, i2 });
		std::cout << "|\n";
	}
	std::cout << "---------------------------------------------------------------------------------------------------";
}

// ������� �������� �����
nlohmann::json create(nlohmann::json Database) {
	system("cls");
	auto old_Database = Database;
	std::cout << "Enter user's login: \n";
	std::cout << "Enter user's password: ";
	SetConsoleCursorPosition(console, { 20, 0 });
	char login[32];
	SetConsoleCursorPosition(console, { 20, 0 });
	std::cin >> login;
	SetConsoleCursorPosition(console, { 23, 1 });
	char password[32];
	std::cin >> password;
	user a(login, password);
	system("cls");
	Database = a + old_Database;
	save(Database); // ���������� ������ � ������ ����� �� ������ �����

	std::cout << "Enter user's name: \n";
	std::cout << "Enter user's email: \n";
	SetConsoleCursorPosition(console, { 19, 0 });
	char name[16];
	std::cin >> name;
	SetConsoleCursorPosition(console, { 20, 1 });
	char email[32];
	std::cin >> email;
	system("cls");
	user b(login, password, (char[8])"User", name, email);
	Database = b + old_Database;
	save(Database); // ��������� ���������� �� ������ �����

	std::cout << "Select user's access_level: Admin Manager User\n";
	std::cout << "Select user's status: Active Needs correction Not active Deleted\n";
	int acl = 0;
	int stt = 0;
	bool switcher = true;
	do {
		if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(VK_DOWN)) {
			switcher = !switcher;
		}
		else if (GetAsyncKeyState(VK_RIGHT)) {
			if (switcher) {
				if (acl != 2) {
					acl++;
				}
			}
			else {
				if (stt != 3) {
					stt++;
				}
			}
		}
		else if (GetAsyncKeyState(VK_LEFT)) {
			if (switcher) {
				if (acl != 0) {
					acl--;
				}
			}
			else {
				if (stt != 0) {
					stt--;
				}
			}
		}
		else if (GetAsyncKeyState(VK_TAB)) {
			break;
		}
		switch (switcher) {
		case true:
			SetConsoleCursorPosition(console, { 14, 0 });
			std::cout << "\033[30m\033[107maccess_level\033[0m\033[40m";
			SetConsoleCursorPosition(console, { 14, 1 });
			std::cout << "status";
			break;
		case false:
			SetConsoleCursorPosition(console, { 14, 0 });
			std::cout << "access_level";
			SetConsoleCursorPosition(console, { 14, 1 });
			std::cout << "\033[30m\033[107mstatus\033[0m\033[40m";
			break;
		}
		switch (acl) {
		case 0:
			SetConsoleCursorPosition(console, { 28, 0 });
			std::cout << "\033[30m\033[107mAdmin\033[0m\033[40m Manager User";
			break;
		case 1:
			SetConsoleCursorPosition(console, { 28, 0 });
			std::cout << "Admin \033[30m\033[107mManager\033[0m\033[40m User";
			break;
		case 2:
			SetConsoleCursorPosition(console, { 28, 0 });
			std::cout << "Admin Manager \033[30m\033[107mUser\033[0m\033[40m";
			break;
		}
		switch (stt) {
		case 0:
			SetConsoleCursorPosition(console, { 22, 1 });
			std::cout << "\033[30m\033[107mActive\033[0m\033[40m Needs correction Not active Deleted";
			break;
		case 1:
			SetConsoleCursorPosition(console, { 22, 1 });
			std::cout << "Active \033[30m\033[107mNeeds correction\033[0m\033[40m Not active Deleted";
			break;
		case 2:
			SetConsoleCursorPosition(console, { 22, 1 });
			std::cout << "Active Needs correction \033[30m\033[107mNot active\033[0m\033[40m Deleted";
			break;
		case 3:
			SetConsoleCursorPosition(console, { 22, 1 });
			std::cout << "Active Needs correction Not active \033[30m\033[107mDeleted\033[0m\033[40m";
			break;
		}
		Sleep(100);
	} while (true);

	std::string access_level;
	switch (acl) {
	case 0:
		access_level = "Admin";
		break;
	case 1:
		access_level = "Manager";
		break;
	case 2:
		access_level = "User";
		break;
	}
	std::string status;
	switch (stt) {
	case 0:
		status = "Active";
		break;
	case 1:
		status = "Needs correction";
		break;
	case 2:
		status = "Not active";
		break;
	case 3:
		status = "Deleted";
		break;
	}
	user c(login, password, access_level.data(), name, email, status.data());
	Database = c + old_Database;
	save(Database);
	return Database;
}

int main() {
	setlocale(0, "");
	if (fs::exists("Database.json")) // �������� �� ������������� ��
	{
		// ������ ������ �� ���������� �� � ���������� 
		std::ifstream database("Database.json");
		nlohmann::json Database;
		database >> Database;

		// ���� � �������
		do {
			// ���� ������ � ������
			std::cout << "PLEASE ENTER YOUR LOGIN AND PASSWORD!\n";
			std::cout << "LOGIN: \n";
			std::cout << "PASSWORD: ";
			SetConsoleCursorPosition(console, { 7, 1 });
			std::string login;
			std::cin >> login; // Guy228
			SetConsoleCursorPosition(console, { 10, 2 });
			std::string password;
			std::cin >> password; // abc123
			// �������� �� ������������ �������� ������ � ������� �� ��
			bool chek = false;
			int i = 0;
			for (; i < Database.size(); i++)
			{
				if (Database[i]["login"] == login && Database[i]["password"] == password) {
					chek = true;
					break;
				}
			}
			system("cls");
			if (chek)
			{
				std::cout << "WELCOME, " << Database[i]["name"] << '!';
				Sleep(1500);
				break;
			}
			else
			{
				std::cout << "WRONG PASSWORD OR LOGIN!";
				Sleep(1500);
				system("cls");
			}
		} while (1);

		menu:
		system("cls");
		// ������� ����
		std::cout << "(USE ARROWS TO MOVE)\n";
		std::cout << "PRINT USERS\n";
		std::cout << "CREATE A USER\n";
		std::cout << "(PRESS [TAB] TO CONTINUE)";
		int pos = 0;
		do {
			if (GetAsyncKeyState(VK_UP)) {
				pos = 0;
			}
			else if (GetAsyncKeyState(VK_DOWN)) {
				pos = 1;
			}
			else if (GetAsyncKeyState(VK_TAB)) {
				break;
			}
			switch (pos) {
			case 0:
				SetConsoleCursorPosition(console, { 0, 1 });
				std::cout << "\033[30m\033[107mPRINT USERS\033[0m\033[40m\n";
				std::cout << "CREATE A USER";
				break;
			case 1:
				SetConsoleCursorPosition(console, { 0, 1 });
				std::cout << "PRINT USERS\n";
				std::cout << "\033[30m\033[107mCREATE A USER\033[0m\033[40m";
				break;
			}
		} while (true);
		system("cls");

		switch (pos) {
		case 0:
			print(Database);
			break;
		case 1:
			Database = create(Database);
			break;
		}
		std::cout << std::endl << "(PRESS [BACKSPACE], TO GO BACK)";
		while (true) {
			if (GetAsyncKeyState(VK_BACK)) {
				goto menu;
			}
		}
	}
	else
	{
		std::cout << "Database was not found!";
	}
	return 0;
}