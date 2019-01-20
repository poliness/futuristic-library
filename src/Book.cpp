#include "Book.h"
#include "Utility.h"
#include <iomanip>
Book::Book(string tytul, string autor, int id, Rodzaj rodzaj, string gatunek, string tresc)
{
	this->tytul = tytul;
	this->autor = autor;
	this->ID = id;
	this->rodzaj = rodzaj;
	this->gatunek = gatunek;
	this->tresc = tresc;
}

Book::~Book(){}


void Book::WypiszKsiazke(std::ostream & out){


	system("cls");

	
	out << "Aby zakonczyc czytanie, wpisz 'Zakoncz'" << endl;


	out << endl << endl;
	out << setw(4) << " " << "Tytul: " << getTitle() << endl << setw(4) << " " << "Autor: " << getAuthor() << endl << endl;

	list<string> parts;
	tresc.erase(std::remove(tresc.begin(), tresc.end(), '\n'), tresc.end());
	


	int ILE_MA_JEDEN_AKAPIT = 2000;
	for (int i = 0; i < tresc.size(); i+=ILE_MA_JEDEN_AKAPIT){
		parts.push_back(tresc.substr(i, ILE_MA_JEDEN_AKAPIT));
	}



	for (auto it : parts){
	
		list<string> lines;
		int ILE_MA_JEDNA_LINIA = 100;
		

		for (int i = 0; i < it.size(); i += ILE_MA_JEDNA_LINIA)
			lines.push_back(it.substr(i, ILE_MA_JEDNA_LINIA));

			
		

		for (auto it : lines){
			out << setw(9) << " " << it << endl;
		}

	
		cout << endl;
		string input;
		getline(cin, input);
		std::transform(input.begin(), input.end(), input.begin(), ::tolower);
		if (input.find("zak") != string::npos)
			return;
		

	}

	
}

void PrintHeader(ostream & stream){
	printEndlEndl("############################################################################################################");
	stream.width(9);
	stream << left << "ID" << " ";
	stream.width(30);
	stream << left << "Tytul" << " ";
	stream.width(30);
	stream << left << "Autor" << " ";
	stream.width(15);
	stream << left << "Rodzaj" << " ";
	stream << left << "Typ" << " ";
	printEndlEndl("############################################################################################################");
}
void Book::WypiszDaneKsiazki(ostream & stream){

	stream.width(8);
	stream << left << ID << " ";
	stream.width(30);
	stream << left << tytul << " ";
	stream.width(31);
	stream << left << autor << " ";
	stream.width(15);
	stream << left<< ((rodzaj == EBOOK) ? "Ebook" : "Audiobook") << " ";
	stream << left << gatunek << " ";
	stream << endl;
}
