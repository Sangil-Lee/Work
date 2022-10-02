#include <iostream>
#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QThread>
#include <acai_client_types.h>
#include <acai_client.h>

#include "QELineEdit.h"
#include "Tester.h"
#include "ui_Tester.h"

using namespace std;

Tester::Tester(QWidget *parent):QWidget(parent),ui(new Ui::Tester),plogin(new Login), psetup(new Setup), pcheck(new SignalChecker)
{
	ui->setupUi(this);

	//plogin = new Login();
	//psetup = new Setup();

	connect(ui->logout, SIGNAL(pressed()), this, SLOT(showlogin()) );
	connect(ui->setup, SIGNAL(pressed()), this, SLOT(showsetup()) );
	connect(ui->load, SIGNAL(pressed()), this, SLOT(load()) );
	connect(ui->check, SIGNAL(pressed()), this, SLOT(check()) );
	connect(ui->save, SIGNAL(pressed()), this, SLOT(save()) );
	connect(ui->startPB, SIGNAL(pressed()), this, SLOT(start()) );
	connect(ui->stopPB, SIGNAL(pressed()), this, SLOT(stop()) );
	connect(ui->pausePB, SIGNAL(pressed()), this, SLOT(pause()) );
	connect(ui->close, SIGNAL(pressed()), this, SLOT(close()) );

	plogin->SetTester(this);
	//plogin->pTester = this;

	ui->scenTable->setSelectionBehavior( QAbstractItemView::SelectRows );
	ui->scenTable->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->resultTable->setSelectionBehavior( QAbstractItemView::SelectRows );
	ui->resultTable->setSelectionMode(QAbstractItemView::SingleSelection);

	//ACAI::Client::initialise ();
}

Tester::~Tester()
{
	delete ui;
}

void Tester::ShowLogin()
{
	plogin->show();
}

void Tester::ShowSetup()
{
	psetup->move(QApplication::desktop()->screen()->rect().center() - psetup->rect().center());
    psetup->show();
	psetup->SetDBConn(plogin->GetDBConn());
}

void Tester::Enable()
{
	string user = plogin->GetCurUser();
	if(!user.empty())
	{
		ui->setup->setEnabled(true);
		ui->load->setEnabled(true);
		ui->check->setEnabled(true);
		ui->save->setEnabled(true);

		ui->setup->update();
		ui->load->update();
		ui->check->update();
		ui->save->update();
	}
}

//SLOT
void Tester::showlogin()
{
	plogin->SetPasswd("");
	ShowLogin();
}

void Tester::showsetup()
{
	string strgrp = plogin->GetCurGrp();
	if(strgrp == string("admin"))
		ShowSetup();
	else
	{
		QMessageBox msgBox;
		msgBox.setText("Current user is not admin group!!");
		msgBox.exec();
	}
}

void Tester::load()
{
	QTableWidget *pTable = ui->scenTable;
    MYSQL  *conn = plogin->GetDBConn();
	try {
		MYSQL_RES *res = mysql_use_result(conn);

		// SELECT 쿼리
		QString sql = QString("select modinfo,scenario from scenario_t");
		MYSQL_RES* result = NULL;
		MYSQL_ROW row;
		if(mysql_query(conn, sql.toUtf8().constData()) == 0)
		{
			result = mysql_store_result(conn);
			while((row = mysql_fetch_row(result)) != NULL){
				pTable->insertRow(pTable->rowCount());
				pTable->setItem(pTable->rowCount()-1, 0, new QTableWidgetItem(QString(row[0])) );
				pTable->setItem(pTable->rowCount()-1, 1, new QTableWidgetItem(QString(row[1])) );
				QString scenario = QString(row[1]);
				QStringList slist = scenario.split(QRegExp("[(,)]"));
				slist.removeAll("");
				qDebug() << slist;

			}
		}
		mysql_free_result(result);
		// Release memories
		mysql_free_result(res);
	} catch (char *e) {
		cerr << "[EXCEPTION] " << e << endl;
		return;
	}
}

void Tester::check()
{
	qDebug() << "Check Pressed ";
	pcheck->move(QApplication::desktop()->screen()->rect().center() - pcheck->rect().center());
    pcheck->show();
}

void Tester::save()
{
	qDebug() << "Save Pressed";
}

void Tester::start()
{
	QELineEdit *pLine = ui->qelinestart;
	pLine->setText("1 second");
	pLine->update();
	emit pLine->returnPressed();

#if 0
	ACAI::Client pv ("CDMS-Tester:PXI:Progress");
	pv.openChannel();

 	if (pv.dataIsAvailable ()) {
        double x = pv.getFloating();

		QMessageBox msgBox;
		msgBox.setText(QString::number(x));
		msgBox.exec();
     }

	pv.closeChannel ();
   	ACAI::Client::finalise ();
#endif
}

void Tester::stop()
{
	pause();
	QELineEdit *pLine = ui->qelinestop;
	pLine->setText("0");
	pLine->update();
	emit pLine->returnPressed();

}

void Tester::pause()
{
	QELineEdit *pLine = ui->qelinestart;
	pLine->setText("Passive");
	pLine->update();
	emit pLine->returnPressed();
}

void Tester::close()
{
	if(plogin->GetDBConn())
		mysql_close(plogin->GetDBConn());

	while(QWidget *w = findChild<QWidget*>())
		delete w;

	qApp->quit();
}

void Tester::closeEvent(QCloseEvent *event)
{
	 QMessageBox::StandardButton resBtn = QMessageBox::question( this, "CDMSTester",
                                                                tr("Are you sure?\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
		close();
    }
}
