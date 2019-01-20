#include "Utility.h"

void sysMsg(string s){
	cout << "[System] " << s << endl;
}

void clear(){
	system("cls");
}

void error(string s){
	cout << endl << s << endl;
	system("pause");
}

void printEndlEndl(string s){
	cout << endl << s << endl;
}


void pause(){
	cout << "\nNacisnij dowolny klawisz, aby kontynuowac";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cin.get();

}

void toUpper(string& str) {
	for (int x = 0; x<str.length(); x++)
		str[x] = toupper(str[x]);
}

string Time_T_ToStamp(time_t tt){
	return to_string(tt);
}


string stampToNormalDate(long timestamp){


	struct tm * dt;
	char buffer[30];
	time_t converted = (time_t)timestamp;
	dt = localtime(&converted);



	strftime(buffer, sizeof(buffer), " %H:%M:%S %d/%m/%Y", dt);
	

	
	return string(buffer);


	
}


bool isInteger(const string & s)
{
	if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

	char * p;
	strtol(s.c_str(), &p, 10);

	return (*p == 0);
}
