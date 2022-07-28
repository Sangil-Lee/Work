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
	void saveuser();

private:
	Ui::Setup *ui;
    MYSQL     *m_conn;
    MYSQL_RES *m_res;
    MYSQL_ROW m_row;

public:
	void SetDBConn(MYSQL *pconn) { m_conn = pconn; };

};
#endif  //

