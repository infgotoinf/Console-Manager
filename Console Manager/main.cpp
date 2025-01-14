#include <iostream>

class user {
private:
	std::string name;
	std::string login; // Должен быть уникальным
	std::string mail;
	std::string status; // Active; Needs correction; Not active; Deleted
	std::string password; // Должен быть зашифрован; hash-salt, sha256-hash или аналогичные
	std::string access_level; //Admin; Manager; User
public:
	user(std::string log, std::string pas, std::string acl, std::string nam = "", std::string mal = "", std::string stt = "Needs correction")
	{
		bool allcorrect = true;
		name = nam;
		login = log;
		mail = mal;
		if (!(stt == "Active" || stt == "Needs correction" || stt == "Not active" || stt == "Deleted")) { // Проверка на то, что статус введён корректно
			allcorrect = false;
		}
		if (!(acl == "Admin" || acl == "Manager" || acl == "User")) { // Проверка на то, что уровень доступа введён корректно
			allcorrect = false;
		}
		// При незаполнении имени, имейла или провале на одной из двух предыдущих проверок, пользователь получает статус "Needs correction"
		if (name == "" || mail == "" || !allcorrect)
		{
			status = "Needs correction";
		}
		else
		{
			status = stt;
		}
		password = pas;
		access_level = acl;
	}
};


int main() {



	return 0;
}