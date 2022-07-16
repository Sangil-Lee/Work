/* Setup.h
* CDMSTester Setup UI
*/
#ifndef  SETUP_DIALOG_H
#define  SETUP_DIALOG_H

#include <QTabWidget> 
#include <mysql/mysql.h> 

namespace Ui {
class Setup;
}

class Setup : public QTabWidget
{
	Q_OBJECT

public:
	explicit Setup(QWidget *widget = 0);
	~Setup();
	
public slots:

private:
	Ui::Setup *ui;

public:

};
#endif  //

