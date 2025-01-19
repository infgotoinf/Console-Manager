#include "encrypt.h"
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
	// �� ������� access_level = "User", name = " ", mail = " ", status = "Needs correction"
	user(char log[32], char pas[32], char acl[16] = const_cast<char*>("User"), char nam[16] = const_cast<char*>(" "), char eml[32] = const_cast<char*>(" "), char stt[18] = const_cast<char*>("Needs correction"))
	{
		login = log;
		password = encrypt(pas, log);
		access_level = acl;
		name = (strlen(nam) <= 1 ? " " : nam);
		email = (strlen(eml) <= 1 ? " " : eml);
		status = stt;
		if (// �������� �� ��, ��� ������ ����� ���������
			!(status == "Active" || status == "Needs correction" || status == "Not active" || status == "Deleted") ||
			// �������� �� ��, ��� ������� ������� ����� ���������
			!(access_level == "Admin" || access_level == "Manager" || access_level == "User") ||
			//�������� �� ��, ��� ��� � ����� �������
			name.size() <= 1 || email.size() <= 1)
		{
			status = "Needs correction"; // ���� ���� ���� �� ������ ���� �� ��� �������, �� ������������� ������ "Needs correction"
		}
	}

	auto get_login() { return login; }
	auto get_password() { return password; }
	auto get_access_level() { return access_level; }
	auto get_name() { return name; }
	auto get_email() { return email; }
	auto get_status() { return status; }

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

// ������ ������ �� ���������� �� � ����������
void import(nlohmann::json& Database) {
	std::ifstream database("Database.json");
	database >> Database;
}

// ������� ���������� �������
void status_upd(nlohmann::json& Database) {
	if ((Database["status"] == "Active" || Database["status"] == "Needs correction" || Database["status"] == "Not active" || Database["status"] == "Deleted") ||
		(Database["access_level"] == "Admin" || Database["access_level"] == "Manager" || Database["access_level"] == "User") ||
		!(Database["name"] == " " || Database["email"] == " "))
	{
		Database["status"] = "Active"; // ���� ���� ���� �� ������ ���� �� ��� �������, �� ������������� ������ "Needs correction"
	}
}



// ������� ������ ��
void print(nlohmann::json Database)
{
	std::cout << "|name";
	SetConsoleCursorPosition(console, { 16, 0 });
	std::cout << "|login";
	SetConsoleCursorPosition(console, { 48, 0 });
	std::cout << "|email";
	SetConsoleCursorPosition(console, { 79, 0 });
	std::cout << "|status";
	SetConsoleCursorPosition(console, { 98, 0 });
	std::cout << "|\n";
	std::cout << "---------------------------------------------------------------------------------------------------";
	for (short i = 0, i2 = 2; i < Database.size(); i++, i2++)
	{
		SetConsoleCursorPosition(console, { 0, i2 });
		std::cout << '|' << Database[i]["name"];
		SetConsoleCursorPosition(console, { 16, i2 });
		std::cout << '|' << Database[i]["login"];
		SetConsoleCursorPosition(console, { 48, i2 });
		std::cout << '|' << Database[i]["email"];
		SetConsoleCursorPosition(console, { 79, i2 });
		std::cout << '|' << Database[i]["status"];
		SetConsoleCursorPosition(console, { 98, i2 });
		std::cout << "|\n";
	}
	std::cout << "---------------------------------------------------------------------------------------------------";
}

// ����� �������������� ���������� � �����
void more_info_print(nlohmann::json Database)
{
	SetConsoleCursorPosition(console, { 98, 0 });
	std::cout << "|password";
	SetConsoleCursorPosition(console, { 130, 0 });
	std::cout << "|access_level";
	SetConsoleCursorPosition(console, { 146, 0 });
	std::cout << '|';
	SetConsoleCursorPosition(console, { 98, 1 });
	std::cout << "-------------------------------------------------";
	short i2 = 2;
	for (short i = 0; i < Database.size(); i++, i2++)
	{
		SetConsoleCursorPosition(console, { 98, i2 });
		std::cout << '|' << decrypt(Database[i]["password"], Database[i]["login"]);
		SetConsoleCursorPosition(console, { 130, i2 });
		std::cout << '|' << Database[i]["access_level"];
		SetConsoleCursorPosition(console, { 146, i2 });
		std::cout << '|';
	}
	SetConsoleCursorPosition(console, { 98, i2 });
	std::cout << "-------------------------------------------------";
}



// ������� ����� � �������
std::string login(nlohmann::json& Database) {
	std::string user_access_level;
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
			if ((Database[i]["login"] == login && Database[i]["password"] == encrypt(password, login))) {
				chek = true;
				break;
			}
		}
		system("cls");
		if (chek)
		{
			user_access_level = Database[i]["access_level"];
			std::cout << "WELCOME, " << user_access_level << ' ' << Database[i]["name"] << '!';
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
	return user_access_level;
}

// ������� ����
int menu(std::string user_access_level) {
	std::cout << "(USE ARROWS TO MOVE)\n";
	std::cout << "PRINT USERS\n";
	if (user_access_level == "Admin") std::cout << "CREATE A USER\n";
	std::cout << "(PRESS [TAB] TO CONTINUE)";
	int pos = 0;
	do {
		if (GetAsyncKeyState(VK_TAB)) break;
		else if (GetAsyncKeyState(VK_UP)) pos = 0;
		else if (GetAsyncKeyState(VK_DOWN)) pos = 1;
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
	return pos;
}

// ������� �������� �����
nlohmann::json create(nlohmann::json Database) {
	SetConsoleCursorPosition(console, { 0, 0 });
	system("cls");
	std::cout << "Enter user's login: \n";
	std::cout << "Enter user's password: ";
	SetConsoleCursorPosition(console, { 20, 0 });
	char login[32];
	std::cin >> login;
	SetConsoleCursorPosition(console, { 23, 1 });
	char password[32];
	std::cin >> password;
	user a(login, password);
	system("cls");
	Database = a + Database;
	save(Database); // ���������� ������ � ������ ����� �� ������ �����

	std::cout << "Enter user's name: \n";
	std::cout << "Enter user's email: ";
	SetConsoleCursorPosition(console, { 19, 0 });
	char name[16];
	std::cin >> name;
	SetConsoleCursorPosition(console, { 20, 1 });
	char email[32];
	std::cin >> email;
	system("cls");
	Database.erase(Database.end() - 1);
	user b(login, password, (char[8])"User", name, email);
	Database = b + Database;
	save(Database); // ��������� ���������� �� ������ �����

	std::cout << "Select user's access_level: Admin Manager User\n";
	std::cout << "Select user's status: Active Not active";
	int acl = 0;
	int stt = 0;
	bool switcher = true;
	do {
		if (GetAsyncKeyState(VK_TAB)) break;
		else if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(VK_DOWN)) switcher = !switcher;
		else if (GetAsyncKeyState(VK_RIGHT)) {
			if (switcher) {
				if (acl != 2) acl++;
			}
			else {
				if (stt != 3) stt++;
			}
		}
		else if (GetAsyncKeyState(VK_LEFT)) {
			if (switcher) {
				if (acl != 0) acl--;
			}
			else {
				if (stt != 0) stt--;
			}
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
			std::cout << "\033[30m\033[107mActive\033[0m\033[40m Not active";
			break;
		case 1:
			SetConsoleCursorPosition(console, { 22, 1 });
			std::cout << "Active \033[30m\033[107mNot active\033[0m\033[40m";
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
		status = "Not active";
		break;
	}
	Database.erase(Database.end() - 1);
	user c(login, password, access_level.data(), name, email, status.data());
	Database = c + Database;
	save(Database);
	return Database;
}

// ������� ���������� ��
void table(nlohmann::json& Database, std::string user_access_level)
{
	COORD coords = { 1, 2 };
table:
	system("cls");
	print(Database);
	std::cout << std::endl << "[ESC] - MENU; ";
	if (user_access_level == "Admin" || user_access_level == "Manager") {
		std::cout << "S - SAVE CHANGES; ";
	}
	if (user_access_level == "Admin") {
		std::cout << "[NUMPAD +] - SHOW MORE INFO; ";
		std::cout << "[DELETE] - DELETE USER; ";
	}
	int siz = Database.size() + 1;
	bool more_info_printed = false;
	while (true) {
		if (GetAsyncKeyState(VK_ESCAPE)) return; // ����� � ����
		else if (GetAsyncKeyState(VK_UP) && coords.Y != 2) coords.Y--;
		else if (GetAsyncKeyState(VK_DOWN) && coords.Y != siz) coords.Y++;

		else if (GetAsyncKeyState(VK_ADD) && user_access_level == "Admin") // ����� �������������� ����������
		{
			more_info_print(Database);
			more_info_printed = true;
		}
		else if (GetAsyncKeyState(VK_DELETE) && user_access_level == "Admin") // ������� �������� ������������
		{
			std::cout << "\033[31mAre you sure you want to delete user " << Database[coords.Y - 2]["login"] << "? Y - YES; N - NO\033[0m";
			do {
				if (GetAsyncKeyState(0x59)) {
					if (coords.Y != 2) {
						--coords.Y;
						Database.erase(coords.Y - 1);
					}
					else {
						Database.erase(coords.Y - 2);
					}
					break;
				}
				else if (GetAsyncKeyState(0x4E)) break;
			} while (true);
			goto table;
		}
		else if (GetAsyncKeyState(0x53) && (user_access_level == "Admin" || user_access_level == "Manager")) { // ���������� ���������
			save(Database);
			SetConsoleCursorPosition(console, { 0, (short)(siz + 3) });
			std::cout << "\033[32mDatabase was successfully saved!\033[0m";
			Sleep(1500);
			std::cout << "\r                                ";
		}
		SetConsoleCursorPosition(console, coords);
		Sleep(50);
	}
}



int main() {
	setlocale(0, "");

	if (fs::exists("Database.json")) // �������� �� ������������� ��
	{
		// ������ ������ �� ���������� �� � ����������
		nlohmann::json Database;
		import(Database);

		// ���� � �������
		std::string user_access_level = login(Database);
		

		menu: // ������� ����
		import(Database); // ��������� ���������� ������ �� ��
		system("cls");
		int pos = menu(user_access_level);
		system("cls");

		switch (pos) {
		case 0:
			table(Database, user_access_level);
			goto menu;
			break;
		case 1:
			Database = create(Database);
			break;
		}
	}
	else
	{
		std::cout << "Database was not found!";
	}
	return 0;
}