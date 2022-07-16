/* Tester.h
* CDMSTester Tester UI
*/
#ifndef  TESTER_DIALOG_H
#define  TESTER_DIALOG_H

#include <QWidget> 
#include <mysql/mysql.h> 

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

private:
	Ui::Tester *ui;

public:

};
#endif  //
