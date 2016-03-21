#include <vector>
#include <time.h>

using namespace std;

const char FILE_NAME[] = "exoplanets.m4e";
const char META[] = "db.meta";

typedef vector<string> strVec;

class ExoPlanets {

	private:
		int recordCount;
		string lastEdit;
		strVec records;
		void readDB();
		void writeDB();
		void writeRecord(string);
		void beautifyNprint(string, int);
		void getSearchResult(string, char);
		strVec getMeta();
		void setMeta();

	public:
		ExoPlanets();
		void addRecord();
		void removeRecord(string);
		void updateRecord(string);
		void dropDB();
		void interface();
		void search(string, char);
		void displayDB();
};