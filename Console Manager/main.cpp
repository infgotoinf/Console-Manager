#include <iostream>

class user {
private:
	std::string name;
	std::string login; // ������ ���� ����������
	std::string mail;
	std::string status; // Active; Needs correction; Not active; Deleted
	std::string password; // ������ ���� ����������; hash-salt, sha256-hash ��� �����������
	std::string access_level; //Admin; Manager; User
public:
	user(std::string log, std::string pas, std::string acl, std::string nam = "", std::string mal = "", std::string stt = "Needs correction")
	{
		bool allcorrect = true;
		name = nam;
		login = log;
		mail = mal;
		if (!(stt == "Active" || stt == "Needs correction" || stt == "Not active" || stt == "Deleted")) { // �������� �� ��, ��� ������ ����� ���������
			allcorrect = false;
		}
		if (!(acl == "Admin" || acl == "Manager" || acl == "User")) { // �������� �� ��, ��� ������� ������� ����� ���������
			allcorrect = false;
		}
		// ��� ������������ �����, ������ ��� ������� �� ����� �� ���� ���������� ��������, ������������ �������� ������ "Needs correction"
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