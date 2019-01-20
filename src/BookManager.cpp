#include "BookManager.h"
#include "Utility.h"
#include "Book.h"
#include "User.h"
#include "UserManager.h"
#include "RentedBooksHandler.h"
#include "DBConnectionHandler.h"

bool _DEBUG_MODE = false;

enum{
	// sekundy
	MINUTE = 60,
	HOUR = 3600,
	DAY = 86400,
	WEEK = 604800,
};


BookManager::BookManager(){}
BookManager::~BookManager(){}


void BookManager::rentBook(Book * book){


	if (!book)
		return;
	system("cls");
	string answer;
	string type = ((book->getGenre() == EBOOK) ? "EBOOK" : "AUDIOBOOK");
	cout << "Czy jestes pewien/pewna, ze chcesz wypozyczyc " + type + " " + book->getTitle() + "?" << endl;
	cout << " (Tak/Nie): ";
	getline(cin, answer);

	std::transform(answer.begin(), answer.end(), answer.begin(), ::tolower);
	system("cls");
	if (answer == "tak"){


		string option;
		cout << "Na jaki okres chcesz wypozyczyc podana ksiazke?" << endl;

		cout << "1) 3 dni" << endl;
		cout << "2) 7 dni" << endl;
		cout << "3) 2 tygodnie" << endl;
		cout << "4) 4 tygodnie" << endl;
		if (_DEBUG_MODE)
		cout << "5) 30 sekund" << endl;

		cout << "Podaj numer opcji: ";
		getline(cin, option);

		if (!isInteger(option)){
			cout << "Podano niepoprawne dane" << endl;
			pause();
			return;
		}

		int rentOption = stoi(option);
		long rentTime;

		switch (rentOption){
		case 1: rentTime = time(0) + DAY * 3; break;
		case 2: rentTime = time(0) + WEEK * 1; break;
		case 3: rentTime = time(0) + WEEK * 2; break;
		case 4: rentTime = time(0) + WEEK * 4; break;
		if (_DEBUG_MODE)
		case 5: rentTime = time(0) + 30; break;
		default: rentTime = time(0) + WEEK * 2; break;

		}
			

		
		if (User * user = sUserManager->logged_user){


			// CHECK, CZY USER MA JUZ DANA KSIAZKE WYPOZYCZONA!
			if (sRentsHandler->AlreadyHasBook(user, book))
			{
				cout << "Wypozyczyles juz te ksiazke. Prosze wyszukac inna pozycje." << endl;
				pause();
				return;
			}

			sRentsHandler->AddRent(user, book, rentTime, false);
			cout << "Pomyslnie wypozyczono ksiazke " + book->getTitle() + "\n" << endl;
			pause();
			return;
		}
		else
			return;
	}

	cout << "Podano niepoprawne dane" << endl;
	pause();
}

void BookManager::PrintBooks(ostream & stream){
	for (auto it : baza){
		it->WypiszDaneKsiazki(stream);
	}
}


Book* BookManager::findBookByID(int bookID){
	for (auto it : baza){
		if (it->getID() == bookID)
			return it;
	}

	return NULL;
}


list<Book*> BookManager::findBookByTitle(string phrase){
	list<Book *> out;

	//if (s1.find(s2) != string::npos) {
	string input = phrase;
	toUpper(input);

	for (auto it : baza){
		string temp = it->getTitle();
		toUpper(temp);
	
		if (temp.find(input) != string::npos){
			out.push_back(it);
		}
	}

	return out;
}


list<Book*> BookManager::findBookByAuthor(string phrase){
	list<Book *> out;

	string input = phrase;
	toUpper(input);

	for (auto it : baza){
		string temp = it->getAuthor();
		toUpper(temp);

		if (temp.find(input) != string::npos){
			out.push_back(it);
		}
	}

	return out;
}

list<Book*> BookManager::findBookByType(string type){
	list<Book *> out;


	string input = type;
	toUpper(input);
	for (auto it : baza){
		string temp = it->getType();
		toUpper(temp);
		if (temp.find(input) != string::npos){
			out.push_back(it);
		}
	}

	return out;
}


bool BookManager::LoadBooks(){
	

	sql::Statement * stmt = sConnectionHandler->con->createStatement();
	sql::ResultSet * res = stmt->executeQuery("SELECT * FROM baza_ksiazek;"); // replace with your statement

	while (res->next()) {

		int ID, rodzaj;
		string gatunek, nazwa, autor, tresc;

		ID = res->getInt("id");
		rodzaj = res->getInt("rodzaj");

		gatunek = res->getString("gatunek");
		nazwa = res->getString("nazwa");
		autor = res->getString("autor");
		tresc = res->getString("tresc");


		baza.push_back(new Book(nazwa, autor, ID, static_cast<Rodzaj>(rodzaj), gatunek, tresc));
	}

	delete res;
	delete stmt;

	return true;
}


