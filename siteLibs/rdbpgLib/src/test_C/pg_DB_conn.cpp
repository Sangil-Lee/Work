#include <iostream>
#include <pqxx/pqxx>

using namespace std;
using namespace pqxx;

int main(int argc, char* argv[])
{
	try{
		//connection C("dbname=testdb user=postgres password=qwer1234 \
				//hostaddr=127.0.0.1 port=5432");
		connection C("dbname=testdb user=postgres password=qwer1234 \
				hostaddr=10.1.4.173 port=5432");
		if (C.is_open()) {
			cout << "Opened database successfully: " << C.dbname() << endl;
		} else {
			cout << "Can't open database" << endl;
			return 1;
		}
		C.disconnect ();
	}catch (const std::exception &e){
		cerr << e.what() << std::endl;
		return 1;
	}
}

