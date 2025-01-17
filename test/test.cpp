#include "user.h"
#include <cassert>
#include <windows.h>

void test_user_creation() {
	//тест 1: норм создание пользователя с обычн значениями 
	{
		char login[32] = "login123";
		char password[32] = "password123";
		user default_user(login, password);
		assert(default_user.get_login() == "login123");
		assert(default_user.get_password() == encrypt("password123", login));
		assert(default_user.get_access_level() == "User");
		assert(default_user.get_name() == " ");
		assert(default_user.get_email() == " ");
		assert(default_user.get_status() == "Needs correction");
	}

	//тест 2: проверк созданиы юзера с заданными значениями
	{
		char login[32] = "login456";
		char password[32] = "password456";
		char access_level[16] = "Admin";
		char name[16] = "John Doe";
		char mail[32] = "john.doe@example.com";
		char status[18] = "Active";
		user custom_user(login, password, access_level, name, mail, status);
		assert(custom_user.get_login() == "login456");
		assert(custom_user.get_password() == encrypt("password456", login));
		assert(custom_user.get_access_level() == "Admin");
		assert(custom_user.get_name() == "John Doe");
		assert(custom_user.get_email() == "john.doe@example.com");
		assert(custom_user.get_status() == "Active");
	}

	//тест 3: проверкааа некорректных значений статуса и уровня доступа
	{
		char login[32] = "invalid_login";
		char password[32] = "invalid_password";
		char access_level[16] = "InvalidAccess";
		char name[16] = " ";
		char mail[32] = " ";
		char status[18] = "InvalidStatus";
		user invalid_user(login, password, access_level, name, mail, status);
		//assert(invalid_user.get_access_level() == "User");
		assert(invalid_user.get_status() == "Needs correction");
	}

	//тест 4: проверка  пустого имени и email
	{
		char login[32] = "login789";
		char password[32] = "password789";
		char access_level[16] = "User";
		char name[16] = " ";
		char mail[32] = " ";
		user empty_fields_user(login, password, access_level, name, mail);
		assert(empty_fields_user.get_name() == " ");
		assert(empty_fields_user.get_email() == " ");
		assert(empty_fields_user.get_status() == "Needs correction");
	}

	std::cout << "All tests passed successfully!\n";
}

int main() {
	test_user_creation();
	return 0;
}