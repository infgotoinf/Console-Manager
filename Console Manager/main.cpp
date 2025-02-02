#define _CRT_SECURE_NO_WARNINGS
#include "encrypt.h"
#include <windows.h>
#include <filesystem>
#include <fstream>
#include <ctime>
#include <nlohmann/json.hpp>

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

namespace fs = std::filesystem;


class user {
private:
	std::string login; // Обязательный
	std::string password; // Обязательный; должен быть зашифрован; hash-salt, sha256-hash или аналогичные
	std::string access_level; // Admin; Manager; User
	std::string name;
	std::string email;
	std::string status; // Active; Needs correction; Not active; Deleted
public:
	// По дефолту access_level = "User", name = " ", mail = " ", status = "Needs correction"
	user(char log[32], char pas[32], char acl[16] = const_cast<char*>("User"), char nam[16] = const_cast<char*>(" "), char eml[32] = const_cast<char*>(" "), char stt[18] = const_cast<char*>("Needs correction"))
	{
		login = log;
		password = encrypt(pas, log);
		access_level = acl;
		name = (strlen(nam) <= 1 ? " " : nam);
		email = (strlen(eml) <= 1 ? " " : eml);
		status = stt;
		if (// Проверка на то, что статус введён корректно
			!(status == "Active" || status == "Needs correction" || status == "Not active" || status == "Deleted") ||
			// Проверка на то, что уровень доступа введён корректно
			!(access_level == "Admin" || access_level == "Manager" || access_level == "User") ||
			//Проверка на то, что имя и имэил введены
			name.size() <= 1 || email.size() <= 1)
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

	// Функция записи переменных из класса в бд (не физическую) 
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


// Функция записи данных из не физической бд в физическую бд 
void save(nlohmann::json const Database, std::string filename) {
	std::ofstream file(filename);
	file << Database;
}

// Запись данных из физической бд в переменную
void import(nlohmann::json& Database, std::string filename) {
	std::ifstream database(filename);
	database >> Database;
}

// Функция обновления статуса
void status_upd(nlohmann::json& Database) {
	if (!(Database["status"] == "Active" || Database["status"] == "Needs correction" || Database["status"] == "Not active" || Database["status"] == "Deleted") ||
		!(Database["access_level"] == "Admin" || Database["access_level"] == "Manager" || Database["access_level"] == "User") ||
		((std::string)Database["name"]).size() <= 1 || ((std::string)Database["email"]).size() <= 1)
	{
		Database["status"] = "Needs correction";
	}
	else if (Database["status"] == "Needs correction") {
		Database["status"] = "Active";
	}
}

// Проверка нужно ли удалять старые записи
bool is_old(nlohmann::json& Deleted) {
	time_t t1 = time(NULL);
	tm t = *localtime(&t1);
	return (Deleted[Deleted.size() - 1]["Date"] < (t.tm_mday + t.tm_mon * 28 - 16) ? true : false);
}


// Функция вывода бд
void print(nlohmann::json const Database)
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

// Вывод дополнительной информации о юзере
void more_info_print(nlohmann::json const Database)
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



// Функция входа в систему
std::string login(nlohmann::json& Database, nlohmann::json const Deleted) {
	std::string user_access_level;
	do {
		// Ввод логина и пароля
		std::cout << "PLEASE ENTER YOUR LOGIN AND PASSWORD!\n";
		std::cout << "LOGIN: \n";
		std::cout << "PASSWORD: ";
		SetConsoleCursorPosition(console, { 7, 1 });
		std::string login;
		std::cin >> login; // Guy228
		SetConsoleCursorPosition(console, { 10, 2 });
		std::string password;
		std::cin >> password; // abc123
		// Проверка на соответствие введённых данных с данными из бд
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
		if (chek & Database[i]["status"] == "Not active")
		{
			std::cout << "YOUR ACCOUNT IS NOT ACTIVE!\n";
			std::cout << "PLESE CALL THE ADMIN AT NUMBER: 88005553535";

		}
		else if (chek)
		{
			user_access_level = Database[i]["access_level"];
			std::cout << "WELCOME, " << user_access_level << ' ' << Database[i]["name"] << '!';
			Sleep(1500);
			system("cls");
			break;
		}
		else
		{
			bool is_del = false;
			for (int i2 = 0; i2 < Deleted.size(); i2++)
			{
				if ((Deleted[i2]["login"] == login && Deleted[i2]["password"] == encrypt(password, login))) {
					std::cout << "\033[31mYOUR ACCOUNT WAS DELETED! " << char(1) << char(1) << char(1) << "\033[0m";
					is_del = true;
					break;
				}
			}
			if (!is_del) std::cout << "WRONG PASSWORD OR LOGIN!";
		}
		Sleep(1500);
		system("cls");
	} while (1);
	return user_access_level;
}

// Функция меню
int menu(std::string user_access_level) {
	if (user_access_level == "Admin") std::cout << "(USE ARROWS TO MOVE)\n";
	std::cout << "\033[30m\033[107mPRINT USERS\033[0m\033[40m\n";
	if (user_access_level == "Admin") std::cout << "CREATE A USER\n";
	std::cout << "(PRESS [TAB] TO CONTINUE)";
	int pos = 0;
	if (user_access_level == "Admin") {
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
	}
	else {
		while(true) if (GetAsyncKeyState(VK_TAB)) break;
	}
	return pos;
}

// Функция создания юзера
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
	save(Database, "Database.json"); // Сохранение логина и пароля юзера на случай краша

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
	save(Database, "Database.json"); // Повторное сохранение на случай краша

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
	save(Database, "Database.json");
	return Database;
}

// Функция управления бд
void table(nlohmann::json& Database, nlohmann::json& Deleted, std::string user_access_level)
{
	COORD table_pos = { 1, 2 };
table:
	system("cls");
	print(Database);
	std::cout << std::endl << "[ESC] - MENU; ";
	if (user_access_level == "Admin" || user_access_level == "Manager") {
		std::cout << "[~] - EDIT; ";
		std::cout << "[ALT] - SAVE CHANGES; ";
	}
	if (user_access_level == "Admin") {
		std::cout << "[NUMPAD +] - SHOW MORE INFO; ";
		std::cout << "[DELETE] - DELETE USER; ";
	}
	int siz = Database.size() + 1;
	bool more_info_printed = false;
	while (true) {
		if (GetAsyncKeyState(VK_ESCAPE)) return; // Выход в меню
		else if (GetAsyncKeyState(VK_UP) && table_pos.Y != 2) table_pos.Y--;
		else if (GetAsyncKeyState(VK_DOWN) && table_pos.Y != siz) table_pos.Y++;
		else if (GetAsyncKeyState(VK_LEFT) && table_pos.X != 0 && (user_access_level == "Manager" || user_access_level == "Admin")) {
			switch (table_pos.X) {
			case 131:
				if (more_info_printed) table_pos.X = 99;
				break;
			case 99:
				if (more_info_printed) table_pos.X = 80;
				break;
			case 80:
				table_pos.X = 49;
				break;
			case 49:
				table_pos.X = 17;
				if (user_access_level == "Admin") break;
			case 17:
				table_pos.X = 1;
				break;
			}
		}
		else if (GetAsyncKeyState(VK_RIGHT) && table_pos.X && (user_access_level == "Manager" || user_access_level == "Admin")) {
			switch (table_pos.X) {
			case 1:
				table_pos.X = 17;
				if (user_access_level == "Admin") break;
			case 17:
				table_pos.X = 49;
				break;
			case 49:
				table_pos.X = (user_access_level == "Admin" ? 80 : 49);
				break;
			case 80:
				if (more_info_printed) table_pos.X = 99;
				break;
			case 99:
				if (more_info_printed) table_pos.X = 131;
				break;
			}
		}

		else if (GetAsyncKeyState(VK_OEM_3)) { // Редактирование информации
			switch (table_pos.X) {
			case 1:
				char new_name[16];
				std::cout << "               ";
				SetConsoleCursorPosition(console, table_pos);
				std::cin >> new_name;
				Database[table_pos.Y - 2]["name"] = new_name;
				break;
			case 17:
				char new_login[32];
				std::cout << "                               ";
				SetConsoleCursorPosition(console, table_pos);
				std::cin >> new_login;
				Database[table_pos.Y - 2]["login"] = new_login;
				break;
			case 49:
				char new_email[32];
				std::cout << "                              ";
				SetConsoleCursorPosition(console, table_pos);
				std::cin >> new_email;
				Database[table_pos.Y - 2]["email"] = new_email;
				break;
			case 80:
				char new_status[18];
				std::cout << "                  ";
				SetConsoleCursorPosition(console, table_pos);
				std::cin >> new_status;
				Database[table_pos.Y - 2]["status"] = new_status;
				break;
			case 99:
				char new_password[32];
				std::cout << "                               ";
				SetConsoleCursorPosition(console, table_pos);
				std::cin >> new_password;
				Database[table_pos.Y - 2]["password"] = new_password;
				break;
			case 131:
				char new_access_level[16];
				std::cout << "               ";
				SetConsoleCursorPosition(console, table_pos);
				std::cin >> new_access_level;
				Database[table_pos.Y - 2]["access_level"] = new_access_level;
				break;
			}
			status_upd(Database[table_pos.Y - 2]);
		}
		else if (GetAsyncKeyState(VK_MENU) && (user_access_level == "Admin" || user_access_level == "Manager")) { // Сохранение изменений
			save(Database, "Database.json");
			save(Deleted, "Deleted.json");
			SetConsoleCursorPosition(console, { 0, (short)(siz + 3) });
			std::cout << "\033[32mDatabase was successfully saved!\033[0m";
			Sleep(1500);
			std::cout << "\r                                ";
		}
		else if (GetAsyncKeyState(VK_ADD) && user_access_level == "Admin") // Вывод дополнительной информации
		{
			more_info_print(Database);
			more_info_printed = true;
		}
		else if (GetAsyncKeyState(VK_DELETE) && user_access_level == "Admin") // Функция удаления пользователя
		{
			std::cout << "\033[31mAre you sure you want to delete user " << Database[table_pos.Y - 2]["login"] << "? Y - YES; N - NO\033[0m";
			do {
				if (GetAsyncKeyState(0x59)) {
					Deleted.push_back(Database[table_pos.Y - 2]);
					time_t t1 = time(NULL);
					tm t = *localtime(&t1);
					Deleted[Deleted.size() - 1]["Date"] = t.tm_mday + t.tm_mon * 28;
					if (table_pos.Y != 2) {
						--table_pos.Y;
						Database.erase(table_pos.Y - 1);
					}
					else {
						Database.erase(table_pos.Y - 2);
					}
					break;
				}
				else if (GetAsyncKeyState(0x4E)) break;
			} while (true);
			goto table;
		}
		SetConsoleCursorPosition(console, table_pos);
		Sleep(50);
	}
}



int main() {
	setlocale(0, "");
	
	if (fs::exists("Database.json")) // Проверка на существование бд
	{
		// Запись данных из физической бд в переменную
		nlohmann::json Database;
		import(Database, "Database.json");
		nlohmann::json Deleted;
		int i = 0;
		for (auto Data : Deleted) {
			if (is_old(Data["Date"])) Deleted.erase(i++);
		}
		import(Deleted, "Deleted.json");

		// Вход в систему
		std::string user_access_level = login(Database, Deleted);
		

		menu: // Главное меню
		import(Database, "Database.json"); // Подгрузка актуальных данных из бд
		system("cls");
		int pos = menu(user_access_level);
		system("cls");

		switch (pos) {
		case 0:
			table(Database, Deleted, user_access_level);
			break;
		case 1:
			Database = create(Database);
			break;
		}
		goto menu;
	}
	else
	{
		std::cout << "Database was not found!";
	}
	return 0;
}