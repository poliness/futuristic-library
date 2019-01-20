#ifndef USERMANAGER_H
#define USERMANAGER_H
#include "Utility.h"
#include "User.h"

enum{
	USER_DOES_NOT_EXIST = 1,
	WRONG_PASSWORD,
	CORRECT,
};


class UserManager
{
public:
	
	static UserManager * getInstance()
	{
		static UserManager singleton;
		return &singleton;
	}


	void Logout();
	bool IsLoggedIn(){return logged_in;}
	User * findUserByUserID(int ID);
	User * findUserByUsername(string uname);
	int Login(string, string);
	bool LoadUsers();
	void AddNewUser(User * user);

	list<User* > users;
	int MAX_ID = 0;
	User * logged_user = NULL;


private:
	UserManager();
	~UserManager();
	bool logged_in = false;
};
#define sUserManager UserManager::getInstance()
#endif