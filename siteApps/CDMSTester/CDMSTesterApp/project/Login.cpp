#include <QMessageBox>
#include <iostream>
#include "Login.h"
#include "Tester.h"
#include "ui_Login.h"

using namespace std;
//g++ mariadb_conn.cpp -I/usr/include -L/usr/lib/x86_64-linux-gnu -lmysqlclient

Login::Login(QWidget *parent):QDialog(parent), ui(new Ui::Login)
{
    // Define application wide macro substitutions.
    applicationMacros = "APPL=demo";

    // Initialize constants
    MY_HOSTNAME = "localhost";
    MY_DATABASE = "cdmstester";
    MY_USERNAME = "root";
    MY_PASSWORD = "qwer1234";
    MY_SOCKET   = NULL;

    // Read the UI
    ui->setupUi(this);

	// Format a MySQL object
	conn = mysql_init(NULL);

	// Establish a MySQL connection
	if (!mysql_real_connect(
				conn,
				MY_HOSTNAME, MY_USERNAME,
				MY_PASSWORD, MY_DATABASE,
				MY_PORT_NO, MY_SOCKET, MY_OPT)) {
	}
}

Login::~Login()
{
    delete ui;
}


void Login::close()
{
	hide();
}

void Login::accept()
{
#if 1
	QString userid = ui->login->text();
	QString passwd = ui->passwd->text();
	cout << "UserID: " << userid.toUtf8().constData() <<", Passwd: " << passwd.toUtf8().constData() << endl;
	//cout << "UserID: " << userid.toStdString() <<", Passwd: " << passwd.toStdString() << endl;

#endif

	//const string userid = ui->login->text().toUtf8().constData();
	//const string passwd = ui->passwd->text().toUtf8().constData();
	//string userid = ui->login->text().toStdString();
	//string passwd = ui->passwd->text().toStdString();
	try {
#if 0
		// Execute a sql statement
		if (mysql_query(conn, "SHOW TABLES")) {
			cerr << mysql_error(conn) << endl;
			return;
		}
#endif
		// Get a result set
		res = mysql_use_result(conn);

#if 0
		// Fetch a result set
		cout << "* MySQL - SHOW TABLES in `"
			<< MY_DATABASE << "`" << endl;
		while ((row = mysql_fetch_row(res)) != NULL)
			cout << row[0] << endl;
#endif

		// SELECT 쿼리
		string sql = string("SELECT user_id, passwd, grp FROM user_t");
		MYSQL_RES* result = NULL;
		m_userList.clear();
		m_passwdList.clear();
		m_grpList.clear();
		if(mysql_query(conn, sql.c_str()) == 0)
		{
			result = mysql_store_result(conn);
			while((row = mysql_fetch_row(result)) != NULL){
				std::cout << "UserID:" << row[0] << " Passwd: " << row[1] << std::endl;
				m_userList << row[0];
				m_passwdList << row[1];
				m_grpList << row[2];
			}
		}
		mysql_free_result(result);
		// Release memories
		mysql_free_result(res);

		// Close a MySQL connection
		//mysql_close(conn);
	} catch (char *e) {
		cerr << "[EXCEPTION] " << e << endl;

		return;
	}

	QStringList::const_iterator constIterator;
	bool checkUser = false;
	int index = 0;
    for (constIterator = m_userList.constBegin(), index = 0; constIterator != m_userList.constEnd(); ++constIterator, index++)
	{
        string dbuser   = (*constIterator).toLocal8Bit().constData();
        string dbpasswd = m_passwdList.at(index).toLocal8Bit().constData();
		if(dbuser == userid.toStdString() && dbpasswd == passwd.toStdString()) {
			checkUser = true;
			m_curUser = dbuser;
			m_curPass = dbpasswd;
			m_curGrp  = m_grpList.at(index).toLocal8Bit().constData();
			break;
		};
	};

	if(checkUser) {
		hide();
		pTester->Enable();
	} else {
		QMessageBox msgBox;
		msgBox.setText("No UserID or Password!!");
		msgBox.exec();
	};
}

void Login::SetPasswd(const QString &text)
{
	ui->passwd->setText(text);
}
