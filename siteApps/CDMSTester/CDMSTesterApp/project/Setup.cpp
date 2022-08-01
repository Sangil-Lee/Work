#include <QtGui>
#include <QMessageBox>
#include <QTableWidget>
#include <QRegExp>
#include <iostream>
#include "Setup.h"
#include "ui_Setup.h"

using namespace std;

Setup::Setup(QWidget *parent):QTabWidget(parent),ui(new Ui::Setup)
{
	ui->setupUi(this);
	//config tab
	connect(ui->usersave, SIGNAL(pressed()), this, SLOT(saveuser()) );
	connect(ui->userload, SIGNAL(pressed()), this, SLOT(userload()) );
	connect(ui->usermodify, SIGNAL(pressed()), this, SLOT(usermod()) );
	connect(ui->userdelete, SIGNAL(pressed()), this, SLOT(userdel()) );

	//module tab
	connect(ui->addmodlist, SIGNAL(pressed()), this, SLOT(addmodlist()) );
	connect(ui->delmodlist, SIGNAL(pressed()), this, SLOT(delmodlist()) );
	connect(ui->modsave,    SIGNAL(pressed()), this, SLOT(modsave()) );
	connect(ui->modcancel,  SIGNAL(pressed()), this, SLOT(modcancel()) );

	//scenario tab
	connect(ui->addscenario, SIGNAL(pressed()), this, SLOT(addscenario()) );
	connect(ui->delscenario, SIGNAL(pressed()), this, SLOT(delscenario()) );
	connect(ui->scensave,    SIGNAL(pressed()), this, SLOT(scensave()) );
	connect(ui->scencancel,  SIGNAL(pressed()), this, SLOT(scencancel()) );

	QTableWidget *pTable = ui->userinfo;
	pTable->resizeColumnsToContents();
	pTable->horizontalHeader()->setStretchLastSection(true);
	//pTable->horizontalHeader()->setStretchLastSection(true);
}

Setup::~Setup()
{
	delete ui;
}

void Setup::saveuser()
{
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
void Setup::userload() 
{
	QTableWidget *pTable = ui->userinfo;
	pTable->setRowCount(0);
	try {
		// Format a MySQL object
		// Get a result set
		m_res = mysql_use_result(m_conn);

		// SELECT 쿼리
		string sql = QString("select user_id, grp from user_t").toUtf8().constData();
		MYSQL_RES* result = NULL;
		MYSQL_ROW row;
		if(mysql_query(m_conn, sql.c_str()) == 0)
		{
			result = mysql_store_result(m_conn);
			while((row = mysql_fetch_row(result)) != NULL){
				cout << "UserID:" << row[0] << " Group: " << row[1] << endl;
				pTable->insertRow(pTable->rowCount());
				pTable->setItem(pTable->rowCount()-1, 0, new QTableWidgetItem(QString(row[0])) );
				pTable->setItem(pTable->rowCount()-1, 1, new QTableWidgetItem(QString(row[1])) );
			}
		}
		mysql_free_result(result);
		// Release memories
		mysql_free_result(m_res);
	} catch (char *e) {
		cerr << "[EXCEPTION] " << e << endl;
		return;
	}
}

void Setup::usermod() 
{
}

void Setup::userdel() 
{
	QTableWidget *pTable = ui->userinfo;
	QString userid = pTable->item(pTable->currentRow(), 0)->text();
	cout << userid.toUtf8().constData() << endl;
#if 1
	try {
		// Get a result set
		m_res = mysql_use_result(m_conn);

		// SELECT 쿼리
		string sql = QString("delete from user_t where user_id = '%1'").arg(userid).toUtf8().constData();
		MYSQL_RES* result = NULL;
		if(mysql_query(m_conn, sql.c_str()) != 0) {
			cout << "Delete query error!!" << endl;
		}
		mysql_free_result(result);
		// Release memories
		mysql_free_result(m_res);
	} catch (char *e) {
		cerr << "[EXCEPTION] " << e << endl;
		return;
	}
#endif
	userload();
}

void Setup::addmodlist()
{
	//modtype/modloc/modkind(QComboBox), modsernum/modname(QLineEdit), moddesc(QTextEdit)
	QComboBox *pType = ui->modtype;
	QComboBox *pLoc  = ui->modloc;
	QComboBox *pKind = ui->modkind;

	QLineEdit *pModser  = ui->modsernum;
	QLineEdit *pModname = ui->modname;
	QTextEdit *pModdesc = ui->moddesc;

	if(pModser->text().isEmpty()) {
		QMessageBox msgBox;
		msgBox.setText("Serial number should be not empty!!");
		msgBox.exec();

		return;
	}


	QString strmodlist = QString("Serial_%1,Type_%2,Loc_%3,Name_%4-%5,Desc_%6").arg(pModser->text()).
		arg(QString::number(pType->currentIndex())).
		arg(QString::number(pLoc->currentIndex())).
		arg(pKind->currentText()).
		arg(pModname->text()).
		arg(pModdesc->toPlainText());

	cout << strmodlist.toUtf8().constData() << endl;

	//modlist(QListWidget)
	QListWidget *pModlist = ui->modlist;
	pModlist->addItem(strmodlist);
}

void Setup::delmodlist()
{
	QListWidget *pModlist = ui->modlist;
	//pModlist->currentItem()->takeItem();
	pModlist->model()->removeRow(pModlist->currentRow());
}

void Setup::modsave()
{
	QListWidget *pModlist = ui->modlist;

#if 0
	if(pModlist->currentRow() < 0) return;
	QString strlist = pModlist->item(pModlist->currentRow())->text();
	QStringList slist = strlist.split(QRegExp("[_,]"));
	qDebug() << slist;
	serial:slist[1], type:slist[3], location:slist[5], modname:slist[7], desc:slist[9]
	for(int i = 0; i < slist.size();++i)
	{
		if(i%2 == 0) continue;
		cout << slist.at(i).toUtf8().constData() << endl;
	}
#endif
	try {
		// Format a MySQL object
		// Get a result set
		m_res = mysql_use_result(m_conn);
		MYSQL_RES* result = NULL;

		for(int i = 0; i < pModlist->count(); ++i)
		{
			QString strlist = pModlist->item(i)->text();
			QStringList slist = strlist.split(QRegExp("[_,]"));
			qDebug() << slist[1] << "," << slist[3] << "," << slist[5] << "," << slist[7] <<"," <<slist[9];

			// SELECT 쿼리
			QString inssql = QString("INSERT INTO module_t(sernum, modname, modtype, location, desciption) VALUES('%1','%2',%3,%4,'%5')").arg(slist[1]).arg(slist[7]).arg(slist[3].toInt()).arg(slist[5].toInt()).arg(slist[9]);
			if(mysql_query(m_conn, inssql.toUtf8().constData())) {
				cout << "Insert Error" << endl;
			}
		};

		mysql_free_result(result);
		// Release memories
		mysql_free_result(m_res);
	} catch (char *e) {
		cerr << "[EXCEPTION] " << e << endl;
		return;
	}
	
#if 0
	for(int i = 0; i < pModlist->count(); ++i)
	{
		QString strlist = pModlist->item(i)->text();
		QStringList slist = strlist.split(QRegExp("[:,]"));
		qDebug() << slist[1] << "," << slist[3] << "," << slist[5] << "," << slist[7] <<"," <<slist[9];
	}
#endif
	
}

void Setup::modcancel()
{
	QListWidget *pModlist = ui->modlist;
	pModlist->clear();

	ui->modtype->setCurrentIndex(0);
	ui->modloc->setCurrentIndex(0);
	ui->modkind->setCurrentIndex(0);

	ui->modsernum->setText("");
	ui->modname->setText("");
	ui->moddesc->setText("");

	hide();
}

void Setup::addscenario()
{
	QComboBox *pfromDev  = ui->fromdevCombo;
	QComboBox *pfromMod  = ui->frommodCombo;
	QComboBox *pfromType = ui->fromtypeCombo;
	QComboBox *pfromCh   = ui->fromchCombo;
	QComboBox *pfromKind = ui->fromkindCombo;

	//PXI-Mod00:AI-CHxx:Volt
	QString strFrom = QString("TEST(%1-%2:%3-%4:%5)").
		arg(pfromDev->currentText()).
		arg(pfromMod->currentText()).
		arg(pfromType->currentText()).
		arg(pfromCh->currentText()).
		arg(pfromKind->currentText());

	qDebug() << strFrom;

#if 0
	QComboBox *ptoDev  = ui->todevCombo;
	QComboBox *ptoMod  = ui->tomodCombo;
	QComboBox *ptoType = ui->totypeCombo;
	QComboBox *ptoCh   = ui->tochCombo;
	QComboBox *ptoKind = ui->tokindCombo;

	QString strTo = QString("To(%1-%2:%3-%4:%5)").
		arg(ptoDev->currentText()).
		arg(ptoMod->currentText()).
		arg(ptoType->currentText()).
		arg(ptoCh->currentText()).
		arg(ptoKind->currentText());
	qDebug() << strTo;
#endif
	
	QComboBox *pScan  = ui->scanCombo;

	QString strScan;
	strScan = QString("SCAN(%1)").arg(pScan->currentText());

	qDebug() << strScan;

	QRadioButton *pAuto  = ui->autoRadio;
	QComboBox    *pfuncT = ui->functypeCombo;
	QLineEdit    *puserT = ui->usertarget;
	QDoubleSpinBox *pminVal = ui->minVal;
	QDoubleSpinBox *pmaxVal = ui->maxVal;

	QString strFunc;
	if(pAuto->isChecked())
		strFunc = QString("FUNC(Auto,%1,Min=%2,Max=%3)").arg(pfuncT->currentText()).arg(pminVal->value()).arg(pmaxVal->value());
	else
		strFunc = QString("FUNC(Manu,%1)").arg(puserT->text());

	qDebug() << strFunc;

	QString strScenario = strFrom+","+strScan+","+strFunc;
	QListWidget *pScenlist = ui->scenList;
	pScenlist->addItem(strScenario);

}
void Setup::delscenario()
{
	QListWidget *pScenlist = ui->scenList;
	pScenlist->model()->removeRow(pScenlist->currentRow());
}
void Setup::scensave()
{
	QListWidget *pScenlist = ui->scenList;

#if 0
	for(int i = 0; i < pScenlist->count(); ++i)
	{
		QString strlist = pScenlist->item(i)->text();
		QStringList slist = strlist.split(QRegExp("[(,)]"));
		slist.removeAll(QString(""));
		qDebug() << slist;

		//scenario_t Save
	}
#endif
#if 1
	try {
		// Format a MySQL object
		m_res = mysql_use_result(m_conn);
		MYSQL_RES* result = NULL;

		for(int i = 0; i < pScenlist->count(); ++i)
		{
			QString strlist = pScenlist->item(i)->text();
			//QStringList slist = strlist.split(QRegExp("[(,)]"));
			//slist.removeAll(QString(""));

			// SELECT 쿼리
			QString inssql = QString("INSERT INTO scenario_t(scenario, modinfo) VALUES('%1','%2')").arg(strlist).arg("");
			if(mysql_query(m_conn, inssql.toUtf8().constData())) {
				cout << "Insert Error" << endl;
			}
		};

		mysql_free_result(result);
		// Release memories
		mysql_free_result(m_res);
	} catch (char *e) {
		cerr << "[EXCEPTION] " << e << endl;
		return;
	}
#endif

}
void Setup::scencancel()
{
	QListWidget *pScenlist = ui->scenList;
	pScenlist->clear();

	hide();
}
