#ifndef BOOK_H
#define BOOK_H
#include "Utility.h"


void PrintHeader(std::ostream & stream);


enum Rodzaj{
	EBOOK,
	AUDIOBOOK,
};


class Book
{
public:
	Book(std::string nazwa, std::string autor, int id, Rodzaj rodzaj, std::string gatunek, std::string tresc);
	~Book();


	void WypiszDaneKsiazki(std::ostream & stream);
	void WypiszKsiazke(std::ostream & stream);

	int getID(){ return ID; }
	std::string getType(){ return gatunek; }
	std::string getTitle(){ return tytul; }
	std::string getAuthor(){ return autor; }
	Rodzaj getGenre(){ return rodzaj; }
private:
	std::string gatunek;
	Rodzaj rodzaj;
	int ID;
	std::string tytul;
	std::string autor;
	std::string tresc;
};
#endif

