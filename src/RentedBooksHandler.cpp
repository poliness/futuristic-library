#include "RentedBooksHandler.h"

RentedBooksHandler::RentedBooksHandler()
{
}

RentedBooksHandler::~RentedBooksHandler()
{	
}

bool RentedBooksHandler::AlreadyHasBook(User * user, Book * book){

	if (!user || !book)
		return false;


	for (auto it : rents){
		if (it.getUser() == user){
			if (it.getBook()->getID() == book->getID())
				return true;
		}
	}

	return false;
}

bool RentedBooksHandler::RemoveRent(User * user, Book * book){

	if (!user || !book)
		return false;

	
	list<Rent>::iterator it = rents.begin();
	while (it != rents.end())
	{
		bool to_delete = false;
		if (it->getBook() == book && it->getUser() == user)
			to_delete = true;

		if (to_delete)
		{
			rents.erase(it++);  // albo it = items.erase(i);
			return true;
		}
		else
		{
			++it;
		}
	}

	return false;

}
void RentedBooksHandler::PrintHeader(ostream & stream){
	printEndlEndl("###################################################################################################");
	stream.width(32);
	stream << left << "Tytul";
	stream.width(28);
	stream << left << "Autor";
	stream.width(15);
	stream << left << "Rodzaj";
	stream.width(13);
	stream << left << "Ostateczna data zwrotu";
	printEndlEndl("###################################################################################################");
}
void Rent::PrintRent(ostream & stream){

	stream.width(29);
	stream << left << book->getTitle() << " ";
	stream.width(29);
	stream << left << book->getAuthor() << " ";
	stream.width(13);
	stream << left << ((book->getGenre() == EBOOK) ? "Ebook" : "Audiobook") << " ";
	stream.width(13);
	stream << left << stampToNormalDate(getExpirationDate());
	stream << endl;
}

void RentedBooksHandler::AddRent(User * user, Book * book, long expDate, bool SYSTEM){

	if (!user || !book)
		return;

	rents.push_back(Rent(user, book, expDate));


	// JEŒLI TO USER ODPALA FJE ADDRENT TO DODAJEMY KSIAZKE DO BAZY DANYCH
	// JESLI TO SYSTEM, A ON ROBI TO TYLKO PRZY LADOWANIU, NIE DODAJEMY.

	if (!SYSTEM){
		sql::PreparedStatement * stmt = sConnectionHandler->con->prepareStatement("INSERT INTO wypozyczenia VALUES(?,?,?);");

		stmt->setInt(1, user->getUserID());
		stmt->setInt(2, book->getID());
		stmt->setInt(3, expDate);
		stmt->execute();
	}
}
bool RentedBooksHandler::hasRent(User * user, Book * book){
	for (auto it : rents){
		if (it.getUser()->getName() == user->getName() && it.getBook()->getTitle() == book->getTitle())
			return true;
	}

	return false;
}
vector<Rent> RentedBooksHandler::findUsersRents(User * user){

	if (!user){
		std::vector<Rent> pusty;
		return pusty;
	}


	vector<Rent> out;

	for (auto it : rents)
		if (it.getUser() == user)
			out.push_back(it);

	return out;
}

void RentedBooksHandler::printUsersRents(User * user){

	if (!user)
		return;

	vector<Rent> userRents = findUsersRents(user);


	PrintHeader(cout);
	for (auto it : userRents)
		it.PrintRent(cout);

}

bool RentedBooksHandler::SaveRents(){
}