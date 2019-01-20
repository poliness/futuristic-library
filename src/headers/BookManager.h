#ifndef BOOKMANAGER_H
#define BOOKMANAGER_H

#include "Book.h"
class BookManager
{
public:

	static BookManager * getInstance()
	{
		static BookManager singleton;
		return &singleton;
	}

	

	bool LoadBooks();
	void PrintBooks(ostream & stream);
	list<Book*> baza;

	Book* findBookByID(int bookID);
	list<Book*> findBookByAuthor(string phrase);
	list<Book*> findBookByTitle(string phrase);
	list<Book*> findBookByType(string type);

	void rentBook(Book * book);

private:
	BookManager();
	~BookManager();
};
#define sBookManager BookManager::getInstance()
#endif
