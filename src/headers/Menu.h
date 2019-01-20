#ifndef MENU_H
#define MENU_H
#include "Utility.h"
#include "RentedBooksHandler.h"
#include "UserManager.h"

string bookTypes[] = { "Sztuka", "Horror", "Socjologia", "Fantastyka", "Sensacja", "Romans", "Poradnik", "Literatura" };


class MenuOptions {
public:
	void add(int opt, void(*fn)()) { actions[opt] = fn; }
	bool getOpt();
	void run() {
		actions[option](); }
private:
	int option;
	map<int, void(*)()> actions;
};

class Menu {
public:
	Menu(){ };

	Menu(string omsg, bool logged_in) : msg(omsg), logged_in(logged_in){};
	void add(string opt) { options.push_back(opt); }
	ostream& display();

	bool logged_in;
	MenuOptions menuOptions;
private:

	string       msg;
	list<string> options;
};

ostream& Menu::display()
{
	system("cls");

	cout << "#######################################" << endl;
	cout << "######## BIBLIOTEKA XXII WIEKU ########" << endl;
	cout << "#######################################" << endl;


	if (logged_in){
		if (User * logged_user = sUserManager->logged_user){
			cout << "#######################################" << endl;
			cout << "Zalogowany/a jako " << logged_user->getName() << " " << logged_user->getSurname() << endl;
			cout << "#######################################" << endl;
		}
	}
	list<string>::const_iterator it = options.begin();

	cout << endl;
	while (it != options.end())
		cout << (*it++) << '\n';
	cout << endl;
	return cout << msg << flush;
}


bool MenuOptions::getOpt()
{
	string stringOption;
	getline(cin, stringOption);
	if (!isInteger(stringOption))
		return false;

	int intOption = stoi(stringOption);

	if (actions.find(intOption) != actions.end()){
		this->option = intOption;
		return true;
	}

	return false;
}


void Menu_REGISTER() {
	system("cls");
	cout << "########################" << endl;
	cout << "Rejestracja czytelnika" << endl;
	cout << "########################\n" << endl;

	string username, password, name, surname;

	cout << "Podaj nazwe uzytkownika: ";
	getline(cin, username);


	while (username.size() < 3){
		cout << "\nNazwa uzytkownika musi miec przynajmniej 3 znaki." << endl;
		cout << "Podaj nazwe uzytkownika: ";
		getline(cin, username);
	}

	while(sUserManager->findUserByUsername(username)){
		cout << "\nTaki uzytkownik juz istnieje. Prosze sprobowac ponownie." << endl;
		cout << "Podaj nazwe uzytkownika: ";
		getline(cin, username);
	}

	
	cout << "Podaj haslo: ";
	getline(cin, password);


	while (password.size() < 3){
		cout << "\nHaslo musi miec przynajmniej 3 znaki." << endl;
		cout << "Podaj haslo: ";
		getline(cin, password);
	}



	cout << "Podaj swoje imie: ";
	getline(cin, name);
	cout << "Podaj swoje nazwisko: ";
	getline(cin, surname);

	std::transform(name.begin(), name.end(), name.begin(), ::tolower);
	std::transform(surname.begin(), surname.end(), surname.begin(), ::tolower);
	
	if (name == "") name = "Nieznany";
	if (surname == "") surname = "Nieznany";

	sUserManager->AddNewUser(new User(++sUserManager->MAX_ID, name, surname, username, password));

	cout << "\nDziekujemy za rejestracje, " << name << "!" << endl;
	cout << "Mozesz sie teraz zalogowac.\n" << endl;
	pause();


}

void Menu_LOGIN() {
	system("cls");
	cout << "###################" << endl;
	cout << "Logowanie" << endl;
	cout << "###################\n" << endl;
	string username, password;
	cout << "Podaj nazwe uzytkownika: ";
	getline(cin, username);
	cout <<"Podaj haslo: ";
	getline(cin, password);

	switch (sUserManager->Login(username, password)){
	case CORRECT:
		cout << "\nZalogowano pomyslnie\n" << endl;
		pause();
		break;

	case USER_DOES_NOT_EXIST:
		cout << endl;
		cout << "Uzytkownik o podanej nazwie nie istnieje." << endl;
		break;

	case WRONG_PASSWORD:
		cout << endl;
		cout << "Podane haslo jest niepoprawne." << endl;
		break;

	}

}

void Menu_QUIT() {
	exit(0);
}



void Menu_BOOK_CATALOG(){
	system("cls");
	cout << "########################" << endl;
	cout << "## KATALOG / WYSZUKIWARKA" << endl;
	cout << "########################\n" << endl;

	cout << "Wybierz rodzaj wyszukiwania: \n" << endl;
	cout << "1) Szukaj wg autora" << endl;
	cout << "2) Szukaj wg tytulu" << endl;
	cout << "3) Szukaj wg kategorii" << endl;
	cout << "4) Wstecz" << endl << endl;

	string inputOption, searchInput;
	cout << "Wybierz opcje: ";
	getline(cin, inputOption);
	cout << endl;


	if (!isInteger(inputOption))
		return;

	int option = stoi(inputOption);


	switch (option){
	case 1:
	case 2:{

		cout << "Podaj " << ((option == 1) ? "imie i nazwisko autora lub jego czesc" : "tytul ksiazki lub jego czesc") << ": ";

		getline(cin, searchInput);

		while (searchInput.size() < 3){
			cout << "\nWpisana fraza powinna miec przynajmniej 3 znaki. Sprobuj ponownie" << endl;
			cout << "Podaj " << ((option == 1) ? "imie i nazwisko autora lub jego czesc" : "tytul ksiazki lub jego czesc") << ": ";
			getline(cin, searchInput);
		}


		list<Book *> books;
		if (option == 1) books = sBookManager->findBookByAuthor(searchInput);
		else books = sBookManager->findBookByTitle(searchInput);

		if (books.size() <= 0){
			cout << "\nNie znaleziono " << ((option == 1) ? "autora" : "tytulu") << " z podana fraza." << endl;
			pause();
			break;
		}

		PrintHeader(cout);
		for (auto it : books)
			it->WypiszDaneKsiazki(cout);


		string yesno;
		cout << endl << "Czy chcesz wypozyczyc ktoras z wyszukanych pozycji? (tak/nie): ";
		getline(cin, yesno);

		std::transform(yesno.begin(), yesno.end(), yesno.begin(), ::tolower);
		if (yesno != "tak")
			break;


		string choice;


		cout << "Podaj numer ID ksiazki, ktora chcesz wypozyczyc: ";
		getline(cin, choice);

		if (!isInteger(choice))
			break;

		int IDchoice = stoi(choice);

		if (Book * book = sBookManager->findBookByID(IDchoice))
			sBookManager->rentBook(book);
		else{
			cout << "Nie znaleziono ksiazki o podanym ID. Sprobuj ponownie." << endl;
			pause();
		}

		break;
	}

	case 3:{
	

		int arraySize = (sizeof(bookTypes) / sizeof(*bookTypes));

		cout << "Wybierz gatunek ksiazek, ktore chcesz wyswietlic: \n\n";

		for (int i = 0; i < arraySize; i++)
			cout << i + 1 << ") " << bookTypes[i] << endl;

		cout << arraySize + 1 << ") Wstecz" << endl << endl;


		string input;
		cout << "Wybierz opcje: ";

		getline(cin, input);

		if (!isInteger(input)){
			cout << "Podano bledne dane" << endl;
			pause();
			break;
		}

		int option = stoi(input);


		if (option == arraySize + 1)
			break;

		list<Book *> books = sBookManager->findBookByType(bookTypes[option - 1]);

		if (books.size() <= 0){
			cout << "Nie znaleziono zadnych ksiazek z wybranego dzialu" << endl;
			pause();
			break;
		}


		PrintHeader(cout);
		for (auto it : books)
			it->WypiszDaneKsiazki(cout);


		string yesno;
		cout << "\nCzy chcesz wypozyczyc ktoras z wyszukanych pozycji? (tak/nie) ";
		getline(cin, yesno);


		std::transform(yesno.begin(), yesno.end(), yesno.begin(), ::tolower);

		if (yesno != "tak")
			break;


		string choice;


		cout << "\nPodaj numer ID ksiazki, ktora chcesz wypozyczyc: ";
		getline(cin, choice);

		if (!isInteger(choice))
			break;

		int IDchoice = stoi(choice);

		if (Book * book = sBookManager->findBookByID(IDchoice))
			sBookManager->rentBook(book);
		else{
			cout << "Nie znaleziono ksiazki o podanym ID. Sprobuj ponownie." << endl;
			pause();
		}



		break;
	}

	case 4:
		return;
	}
	
}
void Menu_MY_RENTS(){
	system("cls");

	
	

	if (User * user = sUserManager->logged_user)
	{
		if (sRentsHandler->findUsersRents(user).size() <= 0){
			cout << "Nie posiadasz zadnej wypozyczonej ksiazki. Skorzystaj z katalogu w menu glownym." << endl;
			system("pause");
			return;
		}

		cout << "Oto lista Twoich wypozyczen: " << endl;
		sRentsHandler->printUsersRents(user);

	}

	cout << endl << "Czy chcesz czytac ktoras z powyzszych ksiazek?: ";

	string yesno;
	getline(cin, yesno);

	std::transform(yesno.begin(), yesno.end(), yesno.begin(), ::tolower);
	if (yesno != "tak")
		return;

	string title;


	cout << "Podaj tytul ksiazki: ";
	getline(cin, title);


	if (User * user = sUserManager->logged_user){
		
		// sprawdzamy czy istnieje
		
		list<Book *> books = sBookManager->findBookByTitle(title);

		if (books.size() != 1){
			cout << "Nie ma takiej ksiazki." << endl;
			system("pause");
			return;
		}

		Book * book = books.front();

		// check, czy user ma ksiazke

		if (!sRentsHandler->hasRent(user, book))
		{
			cout << "Nie posiadasz podanej ksiazki w swojej kolekcji." << endl;
			system("pause");
			return;
		}


		

		
		
		book->WypiszKsiazke(cout);
	

	}




	pause();

}

void Menu_INFO(){
	system("cls");
	cout << "###########################################\n " << endl;
	cout << "Aplikacja napisana w calosci w jezyku C++, bez" << endl;
	cout << "wykorzystania dodatkowych bibliotek, dzialajaca" << endl;
	cout << "zarowno na systemach Windows jak i Unix.\n" << endl;

	cout << "Autorzy: Paulina Zapletal, Michal Kowalski\n" << endl;
	cout << "########################################### " << endl;


	pause();
}


void Menu_RECOMMENDED_BOOKS(){
	system("cls");
	auto books = sBookManager->baza;

	time_t theTime = time(NULL);
	struct tm *aTime = localtime(&theTime);

	int day = aTime->tm_mday;
	int week = 1;

	if (day > 7 && day < 15) week = 2;
	else if (day >= 14 && day < 21) week = 3;
	else if (day >= 21 && day < 30) week = 4;


	
	list<Book *> recommendedBooks;

	
	for (int i = week; i < books.size(); i += 3){
		if (Book * book = sBookManager->findBookByID(i))
			recommendedBooks.push_back(book);
	}



	cout << endl << "Oto pozycje polecane w tym tygodniu: "<< endl;

	PrintHeader(cout);

	// wypisujemy tylko 10 pozycji proponowanych
	int i = 0;
	for (auto it : recommendedBooks){
		it->WypiszDaneKsiazki(cout);
		if (i++ == 10)
			break;
	}

	cout << endl;

	string yesno;
	cout << endl << "Czy chcesz wypozyczyc ktoras z wyszukanych pozycji? (tak/nie): ";
	getline(cin, yesno);

	std::transform(yesno.begin(), yesno.end(), yesno.begin(), ::tolower);
	if (yesno != "tak")
		return;


	string choice;


	cout << "Podaj numer ID ksiazki, ktora chcesz wypozyczyc: ";
	getline(cin, choice);

	if (!isInteger(choice))
		return;

	int IDchoice = stoi(choice);

	if (Book * book = sBookManager->findBookByID(IDchoice))
		sBookManager->rentBook(book);
	else{
		cout << "Nie znaleziono ksiazki o podanym ID. Sprobuj ponownie." << endl;
		pause();
	}

}

void Menu_ABOUT_ME(){
	system("cls");



	if (User * user = sUserManager->logged_user){
		cout << "######################################" << endl;
		cout << "############ TWOJE KONTO #############" << endl;
		cout << "######################################\n" << endl;
		cout << "Twoj numer identyfikacyjny: " << user->getUserID() << endl;
		cout << "Twoja nazwa uzytkownika: " << user->getUsername() << endl;
		cout << "Twoje imie: " << user->getName() << endl;
		cout << "Twoje nazwisko: " << user->getSurname() << endl;
	}
	else cout << "WYSTAPIL BLAD!" << endl;

	pause();
}


void Menu_RETURN_BEFORE_EXPIRATION(){

	system("cls");
	
	if (User * user = sUserManager->logged_user){


		auto usersRents = sRentsHandler->findUsersRents(user);
		list<Book *> usersBooks;

		for (auto it : usersRents){
			if (it.getUser() == user)
				usersBooks.push_back(it.getBook());
		}


		
		if (usersBooks.size() <= 0)
			return;

		PrintHeader(cout);
		for (auto it : usersBooks)
			it->WypiszDaneKsiazki(cout);

		string input;
		cout << "\nPodaj numer ID ksiazki, ktora chcesz zwrocic: ";
		getline(cin, input);

		if (!isInteger(input)){
			cout << "Podano bledne dane" << endl;;
			pause();
			return;
		}

		int choice = stoi(input);

		if (Book * book = sBookManager->findBookByID(choice)){
			if (sRentsHandler->RemoveRent(user, book)){
				cout << "Pomyslnie usunieto ksiazke" << endl;
				pause();
				return;
			}

			
		}

		cout << "Nie posiadasz takiej pozycji" << endl;
		pause();
		return;
	}
}
void Menu_LOGOUT(){
	sUserManager->Logout();
}

#endif