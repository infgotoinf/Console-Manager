#include <iostream>

class user {
private:
	std::string name;
	std::string login;
	std::string mail;
	std::string status; // Active; Needs correction; Not active; Deleted
	std::string password; // Encrypted; hash-salt, sha256-hash или аналогичные
	std::string access_level; //Admin; Manager; User
public:
	user(std::string nam = "", std::string log, std::string mal = "", std::string stt = "Needs correction", std::string pas, std::string acl) {
		bool allcorrect = true;
		name = nam;
		login = log;
		mail = mal;
		if (!(stt == "Active" || stt == "Needs correction" || stt == "Not active" || stt == "Deleted")) {
			allcorrect = false;
		}
		if (!(stt == "Admin" || stt == "Manager" || stt == "User")) {
			allcorrect = false;
		}
		if (name == "" || mail == "" || !allcorrect) {
			status = "Needs correction";
		}
		else {
			status = stt; // Active; Needs correction; Not active; Deleted
		}
		password = pas; // Encrypted; hash-salt, sha256-hash или аналогичные
		access_level = acl; //Admin; Manager; User
	}
};

int main() {

	return 0;
}