#include <QMessageBox>
#include <iostream>
#include "Setup.h"
#include "ui_Setup.h"

using namespace std;

Setup::Setup(QWidget *parent):QTabWidget(parent),ui(new Ui::Setup)
{
	ui->setupUi(this);
	connect(ui->usersave, SIGNAL(pressed()), this, SLOT(saveuser()) );
}

Setup::~Setup()
{
	delete ui;
}

void Setup::saveuser()
{
	cout << "Save User" << endl;
	//select * from user_t group by user_id having count(*) > 1;
	QString userid = ui->userid->text();
	QString passwd = ui->passwd->text();
	QString passwdconfirm = ui->passwdconfirm->text();
	QString grp = ui->grpcombo->currentText();
	try {
		// Format a MySQL object
		// Get a result set
		m_res = mysql_use_result(m_conn);

		// SELECT 쿼리
		string sql = QString("select user_id from user_t where user_id = '%1'").arg(userid).toUtf8().constData();
		MYSQL_RES* result = NULL;
		MYSQL_ROW row;
		if(mysql_query(m_conn, sql.c_str()) == 0)
		{
			result = mysql_store_result(m_conn);
			while((row = mysql_fetch_row(result)) != NULL){
				QMessageBox msgBox;
				msgBox.setText("Already UserID existed!!");
				msgBox.exec();
			}
		}
		mysql_free_result(result);
		// Release memories
		mysql_free_result(m_res);
	} catch (char *e) {
		cerr << "[EXCEPTION] " << e << endl;
		return;
	}

	if(passwd != passwdconfirm)
	{
		QMessageBox msgBox;
		msgBox.setText("Confirm Password!!");
		msgBox.exec();
		return;
	}


	try {
		// Format a MySQL object
		// Get a result set
		m_res = mysql_use_result(m_conn);

		// SELECT 쿼리
		QString inssql = QString("INSERT INTO user_t(user_id, passwd, grp) VALUES('%1','%2','%3')").arg(userid).arg(passwd).arg(grp);
		MYSQL_RES* result = NULL;
		if(mysql_query(m_conn, inssql.toUtf8().constData()))
		{
			cout << "Insert Error" << endl;
		}
		mysql_free_result(result);
		// Release memories
		mysql_free_result(m_res);
	} catch (char *e) {
		cerr << "[EXCEPTION] " << e << endl;
		return;
	}
	//cout << inssql.toUtf8().constData() << endl;
	

}
