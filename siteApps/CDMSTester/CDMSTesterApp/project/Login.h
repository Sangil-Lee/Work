/*  Login.h
 *
 */

#ifndef LOGIN_DIALOG_H
#define LOGIN_DIALOG_H

#include <QDialog>
#include <QStringList>
#include <QDebug>
#include <mysql/mysql.h>  // require libmysqlclient-dev

namespace Ui {
class Login;
}

class Tester;

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

	void SetPasswd(const QString &text);

private:

public slots:
	void accept();
	void close();

private:
    QString applicationMacros; // Macro substitutions to be applied across the application

    Ui::Login   *ui;
    const char* MY_HOSTNAME;
    const char* MY_DATABASE;
    const char* MY_USERNAME;
    const char* MY_PASSWORD;
    const char* MY_SOCKET;
    enum {
        MY_PORT_NO = 3306,
        MY_OPT     = 0
    };
    MYSQL     *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

	QStringList m_userList;
	QStringList m_passwdList;
	QStringList m_grpList;

	std::string m_curUser;
	std::string m_curPass;
	std::string m_curGrp;
	Tester	*pTester;
	
public:
	std::string GetCurUser() { return m_curUser; }
	std::string GetCurPass() { return m_curPass; }
	std::string GetCurGrp() { return  m_curGrp; }
	void SetTester(Tester *ptester) {pTester = ptester;}
	MYSQL *GetDBConn() { return conn; };


};

#endif // LOGIN_DIALOG_H
