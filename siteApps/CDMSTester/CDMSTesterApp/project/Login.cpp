#include <QMessageBox>
#include <iostream>
#include "Login.h"
#include "ui_Login.h"

// .ui file opened by push buttons and loaded into a dialog
#define DIALOG_UI ":/ui/Login.ui"

using namespace std;
//g++ mariadb_conn.cpp -I/usr/include -L/usr/lib/x86_64-linux-gnu -lmysqlclient

Login::Login(QWidget *parent):QDialog(parent), ui(new Ui::Login)
{
    // Define application wide macro substitutions.
    applicationMacros = "APPL=demo";

    // Initialize constants
    MY_HOSTNAME = "localhost";
    MY_DATABASE = "cdmstester";
    MY_USERNAME = "ctrluser";
    MY_PASSWORD = "qwer1234";
    MY_SOCKET   = NULL;

    // Read the UI
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}


void Login::close()
{
	if(conn!=NULL)
		mysql_close(conn);
	qApp->exit();
}
void Login::accept()
{
#if 0
	QString userid = ui->login->text();
	QString passwd = ui->passwd->text();
	qDebug() << "UserID: " << userid.toUtf8().constData() <<", Passwd: " << passwd.toUtf8().constData();
	qDebug() << "UserID: " << userid <<", Passwd: " << passwd;
#endif
	string userid = ui->login->text().toUtf8().constData();
	string passwd = ui->passwd->text().toUtf8().constData();
	try {
		// Format a MySQL object
		conn = mysql_init(NULL);
		// Establish a MySQL connection
		if (!mysql_real_connect(
					conn,
					MY_HOSTNAME, MY_USERNAME,
					MY_PASSWORD, MY_DATABASE,
					MY_PORT_NO, MY_SOCKET, MY_OPT)) {
			cerr << mysql_error(conn) << endl;
			return;
		}
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
		if(mysql_query(conn, sql.c_str()) == 0)
		{
			result = mysql_store_result(conn);
			while((row = mysql_fetch_row(result)) != NULL){
				//cout << "UserID:" << row[0] << " Passwd: " << row[1] << endl;
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
		if(dbuser == userid && dbpasswd == passwd) {
			checkUser = true;
			m_curUser = dbuser;
			m_curPass = dbpasswd;
			m_curGrp  = m_grpList.at(index).toLocal8Bit().constData();
			break;
		};
	};

	if(checkUser)
	{
		hide();
	}
	else
	{
		QMessageBox msgBox;
		msgBox.setText("No UserID or Password!!");
		msgBox.exec();
	};
}

void Login::SetPasswd(const QString &text)
{
	ui->passwd->setText(text);
}
