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

private:
    Ui::Login *ui;
    QString applicationMacros; // Macro substitutions to be applied across the application

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

public:
};

#endif // LOGIN_DIALOG_H
