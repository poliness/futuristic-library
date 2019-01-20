#include "BookManager.h"
#include "UserManager.h"
#include "Menu.h"
#include "Utility.h"
#include "DBConnectionHandler.h"

int main(){


	sConnectionHandler->ConnectToDB("51.255.85.162", "ksiegarnia", "ksiegarnia123");

	////////////////////////
	// ZAWSZE WCZYTYWAC KSIAZKI PRZED USERAMI, ZAWSZE.
	(!sBookManager->LoadBooks()) ? sysMsg("Nie udalo sie otworzyc pliku z ksiazkami") : sysMsg("Ksiazki wczytane");
	(!sUserManager->LoadUsers()) ? sysMsg("Nie udalo sie otworzyc pliku z userami") : sysMsg("Uzytkownicy wczytani");

	system("pause");
	
	////////////////////////


	Menu * mainMenu = new Menu("Wybierz opcje: ", false);

	mainMenu->add("1) Zarejestruj sie");
	mainMenu->add("2) Zaloguj sie");
	mainMenu->add("3) Informacje");
	mainMenu->add("4) Zakoncz program");

	mainMenu->menuOptions.add(1, Menu_REGISTER);
	mainMenu->menuOptions.add(2, Menu_LOGIN);
	mainMenu->menuOptions.add(3, Menu_INFO);
	mainMenu->menuOptions.add(4, Menu_QUIT);



	Menu * libraryMenu = new Menu("Wybierz opcje: ", true);

	libraryMenu->add("1) Wyszukiwarka / Katalog");
	libraryMenu->add("2) Lista moich wypozyczen");
	libraryMenu->add("3) Zwrot pozycji przed terminem");
	libraryMenu->add("4) Polecane");
	libraryMenu->add("5) O mnie");
	libraryMenu->add("6) Wyloguj sie");

	libraryMenu->menuOptions.add(1, Menu_BOOK_CATALOG);
	libraryMenu->menuOptions.add(2, Menu_MY_RENTS);
	libraryMenu->menuOptions.add(3, Menu_RETURN_BEFORE_EXPIRATION);
	libraryMenu->menuOptions.add(4, Menu_RECOMMENDED_BOOKS);
	libraryMenu->menuOptions.add(5, Menu_ABOUT_ME);
	libraryMenu->menuOptions.add(6, Menu_LOGOUT);



	Menu * currentMenu;

	while (1)
	{
		if (sUserManager->IsLoggedIn())
			currentMenu = libraryMenu;
		else
			currentMenu = mainMenu;

		currentMenu->display();

		if (currentMenu->menuOptions.getOpt()){
			cout << endl;
			currentMenu->menuOptions.run();
		}
		else
			cerr << "\nNiepoprawna opcja\n" << endl;
	}
	
	
	

	return 0;
}

