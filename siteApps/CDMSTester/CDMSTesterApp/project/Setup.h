/* Setup.h
* CDMSTester Setup UI
*/
#ifndef  SETUP_DIALOG_H
#define  SETUP_DIALOG_H

#include <QTabWidget> 
#include <mysql.h> 

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
	//config
	void saveuser();
	void userload();
	void usermod();
	void userdel();

	//module
	void addmodlist();
	void delmodlist();
	void modsave();
	void modcancel();
	
	//scenario
	void addscenario();
	void delscenario();
	void scensave();
	void scencancel();
	void modsearch();
	void channelChange(QString strIndex);

private:
	Ui::Setup *ui;
    MYSQL     *m_conn;
    MYSQL_RES *m_res;
    MYSQL_ROW m_row;

public:
	void SetDBConn(MYSQL *pconn) { m_conn = pconn; };

};
#endif  //

