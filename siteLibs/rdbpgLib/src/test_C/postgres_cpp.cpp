#include <iostream>
#include <pqxx/pqxx> 

using namespace std;
using namespace pqxx;

int main(int argc, char* argv[])
{
	try{
		connection C("dbname=testdb user=postgres password=cohondob \
				hostaddr=127.0.0.1 port=5432");
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


//create table
#include <iostream>
#include <pqxx/pqxx> 

using namespace std;
using namespace pqxx;

int main(int argc, char* argv[])
{
	char * sql;

	try{
		connection C("dbname=testdb user=postgres password=cohondob \
				hostaddr=127.0.0.1 port=5432");
		if (C.is_open()) {
			cout << "Opened database successfully: " << C.dbname() << endl;
		} else {
			cout << "Can't open database" << endl;
			return 1;
		}
		/* Create SQL statement */
		sql = "CREATE TABLE COMPANY("  \
			   "ID INT PRIMARY KEY     NOT NULL," \
			   "NAME           TEXT    NOT NULL," \
			   "AGE            INT     NOT NULL," \
			   "ADDRESS        CHAR(50)," \
			   "SALARY         REAL );";

		/* Create a transactional object. */
		work W(C);

		/* Execute SQL query */
		W.exec( sql );
		W.commit();
		cout << "Table created successfully" << endl;
		C.disconnect ();
	}catch (const std::exception &e){
		cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}


//Insert Operation
#include <iostream>
#include <pqxx/pqxx> 

using namespace std;
using namespace pqxx;

int main(int argc, char* argv[])
{
	char * sql;

	try{
		connection C("dbname=testdb user=postgres password=cohondob \
				hostaddr=127.0.0.1 port=5432");
		if (C.is_open()) {
			cout << "Opened database successfully: " << C.dbname() << endl;
		} else {
			cout << "Can't open database" << endl;
			return 1;
		}
		/* Create SQL statement */
		sql = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
			   "VALUES (1, 'Paul', 32, 'California', 20000.00 ); " \
			   "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
			   "VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "     \
			   "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
			   "VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );" \
			   "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
			   "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";

		/* Create a transactional object. */
		work W(C);

		/* Execute SQL query */
		W.exec( sql );
		W.commit();
		cout << "Records created successfully" << endl;
		C.disconnect ();
	}catch (const std::exception &e){
		cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}

//Select Operation
#include <iostream>
#include <pqxx/pqxx> 

using namespace std;
using namespace pqxx;

int main(int argc, char* argv[])
{
	char * sql;

	try{
		connection C("dbname=testdb user=postgres password=cohondob \
				hostaddr=127.0.0.1 port=5432");
		if (C.is_open()) {
			cout << "Opened database successfully: " << C.dbname() << endl;
		} else {
			cout << "Can't open database" << endl;
			return 1;
		}
		/* Create SQL statement */
		sql = "SELECT * from COMPANY";

		/* Create a non-transactional object. */
		nontransaction N(C);

		/* Execute SQL query */
		result R( N.exec( sql ));

		/* List down all the records */
		for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
			cout << "ID = " << c[0].as<int>() << endl;
			cout << "Name = " << c[1].as<string>() << endl;
			cout << "Age = " << c[2].as<int>() << endl;
			cout << "Address = " << c[3].as<string>() << endl;
			cout << "Salary = " << c[4].as<float>() << endl;
		}
		cout << "Operation done successfully" << endl;
		C.disconnect ();
	}catch (const std::exception &e){
		cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}

//Update Operation
#include <iostream>
#include <pqxx/pqxx> 

using namespace std;
using namespace pqxx;

int main(int argc, char* argv[])
{
	char * sql;

	try{
		connection C("dbname=testdb user=postgres password=cohondob \
				hostaddr=127.0.0.1 port=5432");
		if (C.is_open()) {
			cout << "Opened database successfully: " << C.dbname() << endl;
		} else {
			cout << "Can't open database" << endl;
			return 1;
		}

		/* Create a transactional object. */
		work W(C);
		/* Create  SQL UPDATE statement */
		sql = "UPDATE COMPANY set SALARY = 25000.00 where ID=1";
		/* Execute SQL query */
		W.exec( sql );
		W.commit();
		cout << "Records updated successfully" << endl;

		/* Create SQL SELECT statement */
		sql = "SELECT * from COMPANY";

		/* Create a non-transactional object. */
		nontransaction N(C);

		/* Execute SQL query */
		result R( N.exec( sql ));

		/* List down all the records */
		for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
			cout << "ID = " << c[0].as<int>() << endl;
			cout << "Name = " << c[1].as<string>() << endl;
			cout << "Age = " << c[2].as<int>() << endl;
			cout << "Address = " << c[3].as<string>() << endl;
			cout << "Salary = " << c[4].as<float>() << endl;
		}
		cout << "Operation done successfully" << endl;
		C.disconnect ();
	}catch (const std::exception &e){
		cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}

//Delete 
#include <iostream>
#include <pqxx/pqxx> 

using namespace std;
using namespace pqxx;

int main(int argc, char* argv[])
{
	char * sql;

	try{
		connection C("dbname=testdb user=postgres password=cohondob \
				hostaddr=127.0.0.1 port=5432");
		if (C.is_open()) {
			cout << "Opened database successfully: " << C.dbname() << endl;
		} else {
			cout << "Can't open database" << endl;
			return 1;
		}

		/* Create a transactional object. */
		work W(C);
		/* Create  SQL DELETE statement */
		sql = "DELETE from COMPANY where ID = 2";
		/* Execute SQL query */
		W.exec( sql );
		W.commit();
		cout << "Records deleted successfully" << endl;

		/* Create SQL SELECT statement */
		sql = "SELECT * from COMPANY";

		/* Create a non-transactional object. */
		nontransaction N(C);

		/* Execute SQL query */
		result R( N.exec( sql ));

		/* List down all the records */
		for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
			cout << "ID = " << c[0].as<int>() << endl;
			cout << "Name = " << c[1].as<string>() << endl;
			cout << "Age = " << c[2].as<int>() << endl;
			cout << "Address = " << c[3].as<string>() << endl;
			cout << "Salary = " << c[4].as<float>() << endl;
		}
		cout << "Operation done successfully" << endl;
		C.disconnect ();
	}catch (const std::exception &e){
		cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}
