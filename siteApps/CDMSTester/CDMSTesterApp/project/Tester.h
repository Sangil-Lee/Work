/* Tester.h
* CDMSTester Tester UI
*/
#ifndef  TESTER_DIALOG_H
#define  TESTER_DIALOG_H

#include <QWidget> 
#include <mysql/mysql.h> 
#include "Login.h"
#include "Setup.h"

namespace Ui {
class Tester;
}

class Tester : public QWidget
{
	Q_OBJECT

public:
	explicit Tester(QWidget *widget = 0);
	~Tester();
	
public slots:
	void showlogin();
	void showsetup();
	void check();
	void load();
	void save();

private:
	Ui::Tester *ui;
	Login		*plogin;
	Setup		*psetup;

public:
	void ShowLogin();
	void ShowSetup();

};
#endif  //
