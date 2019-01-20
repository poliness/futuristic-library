#ifndef RENTEDBOOKSHANDER_H
#define RENTEDBOOKSHANDER_H
#include "Book.h"
#include "Utility.h"
#include "UserManager.h"
#include "DBConnectionHandler.h"
#include "User.h"
class Rent{
public:

	Rent(User * user, Book * book, long expirationDate) : user(user), book(book), expirationDate(expirationDate){};

	User * getUser(){ return user; }
	long getExpirationDate(){ return expirationDate; }
	Book * getBook(){ return book; }
	

	void PrintRent(ostream & stream);
	

private:

	User * user;
	long expirationDate;
	Book * book;

};





class RentedBooksHandler
{
public:
	
	static RentedBooksHandler * getInstance()
	{
		static RentedBooksHandler singleton;
		return &singleton;
	}

	bool SaveRents();
	void PrintHeader(ostream & stream);
	void AddRent(User * user, Book * book, long expDate, bool SYSTEM);
	bool AlreadyHasBook(User * user, Book * book);
	bool RemoveRent(User * user, Book * book);
	vector<Rent> findUsersRents(User * user);
	void printUsersRents(User * user);
	bool hasRent(User * user, Book * book);

private:
	RentedBooksHandler();
	~RentedBooksHandler();
	list<Rent> rents;
};
#define sRentsHandler RentedBooksHandler::getInstance()

#endif