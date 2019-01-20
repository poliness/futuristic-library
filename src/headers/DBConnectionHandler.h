#pragma once

#include "Utility.h"
#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

using namespace sql;

class DBConnectionHandler
{
public:

	sql::Driver *driver;
	sql::Connection *con;


	static DBConnectionHandler * getInstance(){
		static DBConnectionHandler instance;
		return &instance;
	}

	void ConnectToDB(string ipaddress, string username, string password){

		cout << "[System] Laczenie z baza danych: " << ipaddress << endl;
		driver = get_driver_instance();
		con = driver->connect(ipaddress,username, password);

		con->setSchema("ksiegarnia");

		cout << "Pomyslnie polaczona z baza danych!" << endl;
		
	}


private:
	DBConnectionHandler();
	~DBConnectionHandler();
};
#define sConnectionHandler DBConnectionHandler::getInstance()

