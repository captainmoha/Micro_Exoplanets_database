#include <iostream>
#include <algorithm>
#include <ctime>
#include <vector>
#include <cstdio>
#include <fstream>
#include <string>
#include "exoPlanets.hpp"



strVec splitStr(string inptstr, char delimiter) {

	/*
	type : helper
	role : takes a string and a delimiter and splits the string into a string vector
	*/

	string tmpStr="";
	strVec stringList;

	for(int i=0;i<inptstr.length();i++){
		if(inptstr[i]==delimiter){
			stringList.push_back(tmpStr);
			tmpStr="";
			continue;
		}
		tmpStr+=inptstr[i];
	}

	stringList.push_back(tmpStr);
	return stringList;

}


bool validateInput(string choice, string choices) {

	/*
	type : helper
	role : takes a string and tests if it matches one of the input choices
	*/

	if (choice.length() == 1) {

		for (char c : choices) {
			if (choice[0] == c) {
				return true;
			}
		}
	}

	return false;
}


bool validateInt(string i) {

	/*
	type : helper
	role : takes a string and tests if it's a digit
	*/

	for (char c : i) {

		if (c < '0' || c > '9') {
			return false;
		}
	}

	return true;

}


string recordify(int id, strVec recordVec) {

	/*
	type : helper
	role : takes an id and a record in the form of a vector and turns them into a nice string
	*/

	string record = "";
	char idC = id + '0';

	record += idC;
	record += ':';

	for (short i = 0; i < 16; i++) {
		record += recordVec.at(i) + ",";
	}

	record += recordVec.at(16);

	return record;
}


void showTable() {

	/*
	type : helper
	role : displays the table top
	*/

	printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	printf("%-6s%-25s%-4s%-8s%-5s%-7s%-6s%-5s%-5s%-15s%-8s%-18s%-18s%-9s%-5s%-5s%-5s%-6s\n",
	"id", "Name", "Bi", "M(J)", "R(J)", "P(d)", "A(au)", "Ex","Age","Discovery", "YOD", "RA", "Dec", "Dist","M*",
	"R*", "T*", "Age*");
	printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

}


string encrypt(string s) {

	/*
	type : helper
	role : encrypt a string
	*/

	string key = "nglsgailkznvlx";

	string out = s;

	
	for (int i = 0; i < s.length(); i++) {

		char enc = out[i] ^ key[i%s.length()];

		
		if (enc == '\n') {
			continue;
		}
		else {
			out[i] = enc;
		}
	}

	return out;

}


void wait(int seconds) {

	/*
	type : helper
	role : this function is used as a sleep function uses time.h
	*/

    int endwait; 
    endwait = clock() + seconds * CLOCKS_PER_SEC ; 
    while (clock() < endwait){} 

} 


const string getTimestamp() {

	/*
	type : helper
	role : gets current time and returns it as a string
	*/

	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time (&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer,80,"%d-%m-%Y %I:%M:%S",timeinfo);
	string str(buffer);

	// std::cout << str;
	return str;

}


void ExoPlanets::setMeta() {

	/*
	type : core
	role : writes to the meta file some information about the database
	*/

	ofstream meta;
	meta.open(META);

	meta << records.size() << "\n";
	(lastEdit == "") ? meta << getTimestamp() : meta << lastEdit;
	
	meta.close();

}


strVec ExoPlanets::getMeta() {

	/*
	type : helper
	role : core read the meta file and returns the data in a vector for display
	*/

	strVec metaVec;
	ifstream meta;
	meta.open(META);

	string nExoPlanetss, timestamp;

	getline(meta, nExoPlanetss);
	(nExoPlanetss == "") ? metaVec.push_back("0") : metaVec.push_back(nExoPlanetss);
	getline(meta, timestamp);
	(timestamp == "") ? metaVec.push_back("No edits yet") : metaVec.push_back(timestamp);
	meta.close();
	return metaVec;

}


ExoPlanets::ExoPlanets() {

	/*
	type : core
	role : constructor;
	*/

	recordCount = 1;
	readDB();
}


void ExoPlanets::readDB() {

	/*
	type : core
	role : read database file into a vector and increment recordCount;
	*/

	recordCount = 1;
	records.clear();
	ifstream db;
	db.open(FILE_NAME, ios::app);

	while(!db.eof()) {
		string record;
		getline(db, record);
		if (record != "") {
			records.push_back(record);
			// cout << record << endl;
			recordCount++;
		}
	}

	db.close();

}


void ExoPlanets::writeDB() {

	/*
	type : core
	role : write datebasefile from a vector and update meta file;
	*/

	ofstream db;
	db.open(FILE_NAME);
	//cout << records.size();
	for(int i = 0; i < records.size(); i++) {
		
		db << records.at(i) << "\n";
		// cout << records.at(i) << "\n";
	}

	lastEdit = getTimestamp();
	setMeta();
	db.close();

}


void ExoPlanets::dropDB() {

	/*
	type : core
	role : delete database and meta files completely;
	*/

	remove(FILE_NAME);
	remove(META);

}


void ExoPlanets::displayDB() {

	/*
	type : core
	role : display the database to stdout;
	*/

	readDB();
	system("clear || cls");
	showTable();
	for(int i = 0; i < records.size(); i++) {
		
		beautifyNprint(records.at(i), i);
	}

	showTable();

}


void ExoPlanets::addRecord() {

	/*
	type : core
	role : add a new record to the database and update the last update meta file;
	*/

	readDB();
	if (records.size() == 0) {
		lastEdit = getTimestamp();
		setMeta();
	}

	string data[17];

	system("clear || cls");
	cout << "Adding a new exoplanet\n";
	cout << "--------------------\n\n";
	cout << "Name of the exoplanet: ";
	cin.ignore();
	getline(cin, data[0]);
	cout << "Binary flag: [0=no companion; 1=P-type binary (circumbinary); 2=S-type binary; 3=orphan planet (no star)]: ";
	getline(cin, data[1]);
	cout << "Mass [Jupiter masses]: ";
	getline(cin, data[2]);
	cout << "Radius [Jupiter radii]: ";
	getline(cin, data[3]);
	cout << "Period [days]: ";
	getline(cin, data[4]);
	cout << "Semi-major axis [Astronomical Units]: ";
	getline(cin, data[5]);
	cout << "Eccentricity: ";
	getline(cin, data[6]);
	cout << "Age [Gyr]: ";
	getline(cin, data[7]);
	cout << "Discovery method: ";
	getline(cin, data[8]);
	cout << "Discovery year [yyyy]: ";
	getline(cin, data[9]);
	cout << "Right ascension [hh mm ss]: ";
	getline(cin, data[10]);
	cout << "Declination [+/-dd mm ss]: ";
	getline(cin, data[11]);
	cout << "Distance from Sun [parsec]: ";
	getline(cin, data[12]);
	cout << "Host star mass [Solar masses]: ";
	getline(cin, data[13]);
	cout << "Host star radius [Solar radii]: ";
	getline(cin, data[14]);
	cout << "Host star temperature [K]: ";
	getline(cin, data[15]);
	cout << "Host star age [Gyr]: ";
	getline(cin, data[16]);

	string record = to_string(recordCount) + ":";

	for (int i = 0; i < 17; i++) {
		record += data[i];
		if (i < 16) {record += ",";};
	}
	records.push_back(record);
	writeRecord(record);
}


void ExoPlanets::writeRecord(string record) {

	/*
	type : core
	role : write a single record to the database file and update meta file;
	*/

	ofstream db;
	db.open(FILE_NAME, ios::app);
	db << record << "\n";
	lastEdit = getTimestamp();
	setMeta();
	db.close();

}


void ExoPlanets::removeRecord(string ID) {

	/*
	type : core
	role : remove a record from the database based on its id and update meta;
	*/

	if (!validateInt(ID)) {

		cout << "invalid id!\n";
		wait(2);	
	}

	else {
		
		int id = atoi(ID.c_str());

		if (id < 1 || id > records.size()) {
			cout << "invalid id!\n";
			wait(2);
			return;
		}

		records.erase(records.begin()+id-1);
		writeDB();
		cout << "Exoplanet with id " << id << " was deleted.\n";
		wait(2);
	}

}


void updateMessage() {

	/*
	type : helper
	role : displays the update submenu
	*/

	// system("clear || cls");
	cout << "What do you want to update?\n";
	cout << "------------------------------\n\n";
	cout << "Enter (1) : Primary identifier of planet (Name)\n";
	cout << "Enter (2) : Binary flag [0=no companion; 1=P-type binary (circumbinary); 2=S-type binary; 3=orphan planet (no star)]\n";
	cout << "Enter (3) : Planetary mass [Jupiter masses]\n";
	cout << "Enter (4) : Radius [Jupiter radii]\n";
	cout << "Enter (5) : Period [days]\n";
	cout << "Enter (6) : Semi-major axis [Astronomical Units]\n";
	cout << "Enter (7) : Eccentricity\n";
	cout << "Enter (8) : Age [Gyr]\n";
	cout << "Enter (9) : Discovery method\n";
	cout << "Enter (a) : Discovery year [yyyy]\n";
	cout << "Enter (b) : Right ascension [hh mm ss]\n";
	cout << "Enter (c) : Declination [+/-dd mm ss]\n";
	cout << "Enter (d) : Distance from Sun [parsec]\n";
	cout << "Enter (e) : Host star mass [Solar masses]\n";
	cout << "Enter (f) : Host star radius [Solar radii]\n";
	cout << "Enter (g) : Host star temperature [K]\n";
	cout << "Enter (h) : Host star age [Gyr]\n";
	cout << "\nEnter choice here: ";

}


void ExoPlanets::updateRecord(string ID) {

	/*
	type : core
	role : change a property of a record and update database and meta files;
	*/


	if (!validateInt(ID)) {
		cout << "invalid id!\n";
		wait(2);
	}

	else {
		int id = atoi(ID.c_str());

		if (id < 1 || id > records.size()) {
			cout << "invalid id!\n";
			wait(2);
			return;
		}

		system("clear || cls");
		cout << "Selected exoplanet:\n";
		showTable();

		string record = records.at(id-1);
		beautifyNprint(record, id-1);
		cout << "\n\n\n";

		strVec splitRecord = splitStr(splitStr(record, ':').at(1),',');
		char choice;
		updateMessage();
		cin >> choice;

		string property;
		switch(choice) {
			case '1' : {
				
				cout << "Enter new Primary identifier of planet (Name) : ";
				cin.ignore();
				getline(cin, property);
				splitRecord.at(0) = property;

				break;
			}

			case '2' : {

				cout << "Enter new Binary flag [0=no companion/ 1=P-type binary (circumbinary)/ 2=S-type binary/ 3=orphan planet (no star)] : ";
				cin.ignore();
				getline(cin, property);
				splitRecord.at(1) = property;
				
				break;
			}

			case '3' : {

				cout << "Enter new Planetary mass [Jupiter masses] : ";
				cin.ignore();
				getline(cin, property);
				splitRecord.at(2) = property;
				
				break;
			}

			case '4' : {

				cout << "Enter new Radius [Jupiter radii] : ";
				cin.ignore();
				getline(cin, property);
				splitRecord.at(3) = property;
				
				break;
			}

			case '5' : {

				cout << "Enter new Period [days] : ";
				cin.ignore();
				getline(cin, property);
				splitRecord.at(4) = property;
				
				break;
			}

			case '6' : {

				cout << "Enter new Semi-major axis [Astronomical Units] : ";
				cin.ignore();
				getline(cin, property);
				splitRecord.at(5) = property;
				
				break;
			}

			case '7' : {

				cout << "Enter new Eccentricity : ";
				cin.ignore();
				getline(cin, property);
				splitRecord.at(6) = property;
				
				break;
			}

			case '8' : {

				cout << "Enter new Age [Gyr] : ";
				cin.ignore();
				getline(cin, property);
				splitRecord.at(7) = property;
				
				break;
			}

			case '9' : {

				cout << "Enter new Discovery method : ";
				cin.ignore();
				getline(cin, property);
				splitRecord.at(8) = property;
				
				break;
			}

			case 'a' : {

				cout << "Enter new Discovery year [yyyy] : ";
				cin.ignore();
				getline(cin, property);
				splitRecord.at(9) = property;
				
				break;
			}

			case 'b' : {

				cout << "Enter new Right ascension [hh mm ss] : ";
				cin.ignore();
				getline(cin, property);
				splitRecord.at(10) = property;
				
				break;
			}

			case 'c' : {

				cout << "Enter new Declination [+/-dd mm ss] : ";
				cin.ignore();
				getline(cin, property);
				splitRecord.at(11) = property;
				
				break;
			}

			case 'd' : {

				cout << "Enter new Distance from Sun [parsec] : ";
				cin.ignore();
				getline(cin, property);
				splitRecord.at(12) = property;
				
				break;
			}

			case 'e' : {

				cout << "Enter new Host star mass [Solar masses] : ";
				cin.ignore();
				getline(cin, property);
				splitRecord.at(13) = property;
				
				break;
			}

			case 'f' : {

				cout << "Enter new Host star radius [Solar radii] : ";
				cin.ignore();
				getline(cin, property);
				splitRecord.at(14) = property;
				
				break;
			}

			case 'g' : {

				cout << "Enter new Host star temperature [K] : ";
				cin.ignore();
				getline(cin, property);
				splitRecord.at(15) = property;
				
				break;
			}

			case 'h' : {

				cout << "Enter new Host star age [Gyr] : ";
				cin.ignore();
				getline(cin, property);
				splitRecord.at(16) = property;
				
				break;
			}

			default: {

				cout << "Invalid choice!\n";
				wait(2);
				return;
			}
		}

		record = recordify(id, splitRecord);
		records.at(id-1) = record;

		cout << "\n\nExoplanet after update\n";
		showTable();
		beautifyNprint(records.at(id-1), id-1);
		wait(4);

		writeDB();
	}

}


void ExoPlanets::beautifyNprint(string record, int i) {

	/*
	type : helper
	role : takes a record and an id, formats them into a pretty string, and prints it
	*/

	strVec propVec = splitStr(records.at(i), ':');
	strVec prop = splitStr(propVec.at(1), ',');
	int id = i+1;

	string data[17];

	// cout << prop.size() << endl;
	for (int i = 0; i < 17; i++) {

		data[i] = prop.at(i);
	}

	printf("%-6d%-25s%-4s%-8s%-5s%-7s%-6s%-5s%-5s%-15s%-8s%-18s%-18s%-9s%-5s%-5s%-5s%-6s\n",
	id, data[0].c_str(), data[1].c_str(), data[2].c_str(), data[3].c_str(), data[4].c_str(), data[5].c_str(),
	data[6].c_str(), data[7].c_str(), data[8].c_str(), data[9].c_str(), data[10].c_str(), data[11].c_str(),
	data[12].c_str(), data[13].c_str(), data[14].c_str(), data[15].c_str(), data[16].c_str()
	);

}


char getSearchChoice() {

	/*
	type : helper
	role : takes a record and an id, formats them into a pretty string, and prints it
	*/

	system("clear || cls");
	cout << "Choose how you wanna search\n";
	cout << "---------------------------------\n\n";
	cout << "Enter (0) : id of a planet (primary key)\n";
	cout << "Enter (1) : Primary identifier of planet (Name)\n";
	cout << "Enter (2) : Binary flag [0=no companion; 1=P-type binary (circumbinary); 2=S-type binary; 3=orphan planet (no star)]\n";
	cout << "Enter (3) : Planetary mass [Jupiter masses]\n";
	cout << "Enter (4) : Radius [Jupiter radii]\n";
	cout << "Enter (5) : Period [days]\n";
	cout << "Enter (6) : Semi-major axis [Astronomical Units]\n";
	cout << "Enter (7) : Eccentricity\n";
	cout << "Enter (8) : Age [Gyr]\n";
	cout << "Enter (9) : Discovery method\n";
	cout << "Enter (a) : Discovery year [yyyy]\n";
	cout << "Enter (b) : Right ascension [hh mm ss]\n";
	cout << "Enter (c) : Declination [+/-dd mm ss]\n";
	cout << "Enter (d) : Distance from Sun [parsec]\n";
	cout << "Enter (e) : Host star mass [Solar masses]\n";
	cout << "Enter (f) : Host star radius [Solar radii]\n";
	cout << "Enter (g) : Host star temperature [K]\n";
	cout << "Enter (h) : Host star age [Gyr]\n";
	cout << "\nEnter choice here : ";

	string choice;
	cin.ignore();
	getline(cin, choice);
	string choices = "0123456789abcdefgh";

	if (validateInput(choice, choices)) {
		return choice[0];
	}
	
	return 'E'; // E for error

}


void ExoPlanets::search(string query, char prop) {

	/*
	type : core
	role : gets called from the interface with the query and the property on which the search is based
			searches the database and prints the resutls (if any were found)
	*/

	
	// if we're searching by id

	if (prop == '0') {

		if (validateInt(query)) {

			int id = atoi(query.c_str());

			if (id > 0 && id <= records.size()) {

				cout << "Search results for " << query << "\n";
				cout << "----------------------------------\n\n";

				showTable();
				beautifyNprint(records.at(id-1), id-1);
			}

			else {
				cout << "id doesn't exist!\n";
				wait(1);
			}
		}
		
		else {
			cout << "Invalid id!\n";
			wait(1);
		}
	}

	else {

		cout << "Search results for " << query << "\n";
		cout << "----------------------------------\n\n";
		showTable();

		int nResults = 0;

		for (int i = 0; i < records.size(); i++) {
			string record = records.at(i);
			strVec propVec = splitStr(splitStr(record, ':').at(1), ',');

			string currentProp;

			// if the prop is one of the options that are letters
			if (prop > '9') {

				short propIndex;

				switch(prop) {

					case 'a': {

						propIndex = 9;
						break;
					}

					case 'b': {

						propIndex = 10;
						break;
					}

					case 'c': {

						propIndex = 11;
						break;
					}

					case 'd': {

						propIndex = 12;
						break;
					}

					case 'e': {

						propIndex = 13;
						break;
					}

					case 'f': {

						propIndex = 14;
						break;
					}

					case 'g': {

						propIndex = 15;
						break;
					}

					case 'h': {

						propIndex = 16;
						break;
					}

				}
				currentProp = propVec.at(propIndex);
			}

			else {
				currentProp = propVec.at(prop - '0' - 1); // convert the index to an int first and subtract 1 because 0-based indexing doh!
			}

			if (currentProp == query || (currentProp.find(query) != string::npos && currentProp[0] == query[0])) {
				beautifyNprint(record, i);
				nResults++;
			}
		}

		cout << "\n\nFound " << nResults << " results for " << query << "\n"; 
	}
}


void ExoPlanets::interface() {

	/*
	type : core method
	role : displays main menu and gives control over the database
	*/

	string choice;

	while (1) {

		strVec meta = getMeta();
		string ts = getTimestamp();
		system("clear || cls");
		cout << "*******************************************************************************************************\n\n";
		cout << "******************************Welcome to the m4e micro database interface!*****************************\n\n";
		cout << "*******************************************************************************************************\n\n";

		cout << "Datebase info\n";
		cout << "-------------------\n";
		cout << "Number of ExoPlanets in the database: " << meta.at(0) << "\n";
		cout << "Last edited: " << meta.at(1);

		cout << "\nCurrent time: " << ts << "\n";
		cout << "\n\n";

		cout << "Datebase controls\n";
		cout << "-----------------------\n";
		cout << "Enter ( 1 ) to display the database.\n";
		cout << "Enter ( 2 ) to add a new record.\n";
		cout << "Enter ( 3 ) to update an existing record.\n";
		cout << "Enter ( 4 ) to remove a record.\n";
		cout << "Enter ( 5 ) to search the database.\n";
		cout << "\n\n";

		cout << "Extra features\n";
		cout << "--------------------\n";
		cout << "Enter ( 6 ) to play Exoplanets video courtesy of Ethan Kruse (Washington University).\n";
		cout << "Enter ( 7 ) to see the location of confirmed Kepler exoplanets on the sky.\n";
		cout << "\n\n";

		cout << "!!!!DANGER ZONE!!!!\n";
		cout << "-------------------------\n";
		cout << "Enter ( c ) to drop the database (this will delete all data!).\n";
		cout << "\n\n";
		cout << "Enter ( e ) to exit the program.\n";
		cout << "\n\nEnter choice here $ ";

		cin >> choice;
		string choices = "1234567ce";

		if (validateInput(choice, choices)) {
			switch (choice[0]) {

				case '1': {
					displayDB();
					cout << "\n\n";
					cout << "Enter any letter to go back to main menu: ";
					char c;
					cin >> c;
					break;
				}

				case '2': {
					addRecord();
					break;
				}

				case '3': {
					system("clear || cls");
					cout << "Updating an existing exoplanet\n";
					cout << "-------------------------------\n\n";
					
					cout << "Enter id of the exoplanet you wanna update: ";
					cin.ignore();
					string id;
					getline(cin, id);
					updateRecord(id);
					break;
				}

				case '4': {
					system("clear || cls");
					cout << "Remving an exoplanet\n";
					cout << "-------------------------------\n\n";
					
					cout << "Enter id of the exoplanet you wanna remove: ";
					cin.ignore();
					string id;
					getline(cin, id);
					removeRecord(id);
					break;
				}

				case '5': {
					char type = getSearchChoice();

					if (type == 'E') {
						cout << "Enter a valid choice!\n";
						wait(2);
					}

					else {
						system("clear || cls");
						cout << "\nEnter your query : ";
						string query;
						getline(cin, query);
						search(query, type);
						cout << "\n\nEnter any letter to go back to main menu: ";
						char c;
						cin >> c;
					}

					break;
				}

				case '6': {
					system("vlc -f --play-and-exit keplerOrrery.mp4");
					interface();
					break;
				}

				case '7': {
					system("stellarium --startup-script s.ssc");
					interface();
					break;
				}

				case 'c': {
					dropDB();
					break;
				}

				case 'e': {
					cout << "\nSee you :D\n";
					wait(1);
					system("clear || cls");
					exit(0);
				}
			}
		}

		else {
			cout << "Enter a valid choice!\n";
			wait(2);
			interface();
		}
	}
}
