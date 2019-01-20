#include "Utility.h"
#include "User.h"

//User * user = new User(userID, name, surname, username, password);
User::User(int userID, string name, string surname, string username, string password)
{
	this->userID = userID;
	this->name = name;
	this->surname = surname;
	this->username = username;
	this->password = password;
}


User::~User()
{
}


