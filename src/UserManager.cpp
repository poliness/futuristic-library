
#include "Utility.h"

#include "BookManager.h"
#include "RentedBooksHandler.h"
#include "UserManager.h"
#include "Book.h"
#include "DBConnectionHandler.h"
UserManager::UserManager(){}


UserManager::~UserManager(){}


int UserManager::Login(string username, string password){

	// pierwszy check
	bool found_username = false;

	for (auto it : users){
		if (it->getUsername() == username){
			found_username = true;
			if (it->getPassword() == password){
				logged_in = true;
				logged_user = findUserByUsername(username);
				return CORRECT;
			}
			else
				return WRONG_PASSWORD;
		}
	}

	if (!found_username)
		return USER_DOES_NOT_EXIST;
	

	
	return 0;

}

User * UserManager::findUserByUserID(int ID){
	for (auto it : users)
		if (it->getUserID() == ID)
			return it;

	return NULL;
}

User * UserManager::findUserByUsername(string uname){

	for (auto it : users)
		if (it->getUsername() == uname)
			return it;

	return NULL;
}

void UserManager::AddNewUser(User * user){

	if (!user)
		return;

	users.push_back(user);


	// DO BAZY TE¯!
	sql::PreparedStatement * stmt = sConnectionHandler->con->prepareStatement("INSERT INTO uzytkownicy VALUES (?,?,?,?,?);");
	stmt->setInt(1, user->getUserID());
	stmt->setString(2, user->getUsername());
	stmt->setString(3, user->getPassword());
	stmt->setString(4, user->getName());
	stmt->setString(5, user->getSurname());
	stmt->execute();
}

void UserManager::Logout(){
	logged_user = NULL;
	logged_in = false;
}

bool UserManager::LoadUsers(){


	// pierwszym query wyci¹gamy wszystkich u¿ytkowników i zapisujemy w ResultSecie
	sql::Statement * stmt = sConnectionHandler->con->createStatement();
	sql::ResultSet * res = stmt->executeQuery("SELECT * FROM uzytkownicy;"); 

	// iterujemy po ca³ym ResultSecie
	while (res->next()) {

		// robimy 5 zmiennych, tyle kolumn ma tabela 'uzytkownicy' i tyle tez parametrów ma ka¿dy user
		int userID;
		string name, surname, username, password;


		// przepisujemy do zmiennych wartoœci - do resultSeta (res) mo¿emy siê odwo³ywaæ poprzez numery kolumn (1,2,3,4,5) lub po prostu nazwy, np kolumna username
		userID = res->getInt("id");
		username = res->getString("username");
		password = res->getString("password");
		name = res->getString("name");
		surname = res->getString("surname");

		// robimy usera
		User * user = new User(userID, name, surname, username, password);

		// zwiêkszamy max user id w programie
		if (userID > MAX_ID)
			MAX_ID = userID;
	

		// teraz jak juz mamy tego usera wczytanego, wczytujemy jego wypozyczenia
		// robimy nowe query, pobieramy gwiazdke czyli WSZYSTKO z tabeli 'wypozyczenia' gdzie id_uzytkownika jest tym, ktore podajemy
		// ustawiamy znak zapytania

		sql::PreparedStatement * stmt = sConnectionHandler->con->prepareStatement("SELECT * FROM wypozyczenia WHERE id_uzytkownika = ?;");
		stmt->setInt(1, userID);
		stmt->execute();
		// zapisujemy wynik w resultsecie
		sql::ResultSet * res2 = stmt->getResultSet();
		
		// robimy wektor zakonczonych wypozyczen, bedzie przechowywal pary < id_uzytkownika - id_ksiazki >
		// potem jak juz zczytamy wszystko to wysadzimy z bazy wszystkie pozycje z tego wektora
		vector<pair<int, int > > zakonczoneWypozyczenia;

		// lecimy po otrzymanym resultsecie
		while (res2->next()) {
			// zczytujemy kolumny id_ksiazki i doKiedy (uzytkownika juz mamy, to po nim szukalismy)
			int id_ksiazki = res2->getInt("id_ksiazki");
			long expiration = res2->getInt("doKiedy");

			// sprawdzamy czy doKiedy ju¿ minê³o
			if (std::stol(Time_T_ToStamp(time(0))) >= expiration){
				// jeœli tak - pushujemy parê user-ksiazka do vectora
				zakonczoneWypozyczenia.push_back(make_pair(userID, id_ksiazki));
				

				// nie chcemy dopuszowywaæ tej ksi¹¿ki do programu, robimy cont. i pêtla wraca do while(res2->next())
				continue;
			}

			// dodajemy ksi¹¿kê do wypozyczen w programie
			if (Book * book = sBookManager->findBookByID(id_ksiazki))
				sRentsHandler->AddRent(user, book, expiration, true);
		}

		// jak ktos przekroczyl czas wypozyczenia, to jego ksiazka jest juz w wektorze 'zakonczoneWypozyczenia'
		// iterujemy
		for (auto it : zakonczoneWypozyczenia){
			// przygotowujemy query
			// DELETE FROM - usuwamy z wypozyczenia WHERE - gdzie: i nasze dwie dane
			sql::PreparedStatement * tmpstmt = sConnectionHandler->con->prepareStatement("DELETE FROM wypozyczenia WHERE id_uzytkownika = ? AND id_ksiazki = ?;");
			// ustawiamy 1 znak zapytania na it.first (wektor przechowuje pary user_id-ksiazka_id)
			tmpstmt->setInt(1, it.first);
			// 2 znak zapytania na it.second
			tmpstmt->setInt(2, it.second);
			// odpalamy i on sobie usuwa pozycje z tabeli 'wypozyczenia' o danym user_id i book_id
			tmpstmt->execute();

			delete tmpstmt;
		}


		// dodajemy usera do programu
		users.push_back(user);

		delete res2;
		delete stmt;
	}

	delete res;
	delete stmt;

	return true;
}

