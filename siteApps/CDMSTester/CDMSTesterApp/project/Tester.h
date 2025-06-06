/* Tester.h
* CDMSTester Tester UI
*/
#ifndef  TESTER_DIALOG_H
#define  TESTER_DIALOG_H

#include <QWidget> 
#include <mysql.h> 
#include "Login.h"
#include "Setup.h"
#include "SignalChecker.h"

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
	void start();
	void stop();
	void pause();
	void close();

	void scenDelete();

private:
	Ui::Tester *ui;
	Login	*plogin;
	Setup	*psetup;
	SignalChecker	*pcheck;
	void closeEvent(QCloseEvent *event);
public:
	void ShowLogin();
	void ShowSetup();
	void Enable();

};
#endif  //
