#include "RentedBooksHandler.h"

#ifndef USER_H
#define USER_H

class User
{
public:
	User(int userID, string name, string surname, string username, string password);
	~User();

	string getName(){return name;}
	string getSurname(){return surname;}
	string getUsername(){return username;}
	string getPassword(){return password;}

	int getUserID(){return userID;}

private:
	int userID;
	string name, surname, username, password;
};
#endif

