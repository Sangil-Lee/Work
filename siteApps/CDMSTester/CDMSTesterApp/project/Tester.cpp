#include <iostream>
#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QThread>
#include "QELineEdit.h"
#include "Tester.h"
#include "ui_Tester.h"

using namespace std;

Tester::Tester(QWidget *parent):QWidget(parent),ui(new Ui::Tester)
{
	ui->setupUi(this);
	plogin = new Login();
	psetup = new Setup();

	connect(ui->logout, SIGNAL(pressed()), this, SLOT(showlogin()) );
	connect(ui->setup, SIGNAL(pressed()), this, SLOT(showsetup()) );
	connect(ui->load, SIGNAL(pressed()), this, SLOT(load()) );
	connect(ui->check, SIGNAL(pressed()), this, SLOT(check()) );
	connect(ui->save, SIGNAL(pressed()), this, SLOT(save()) );
	connect(ui->startPB, SIGNAL(pressed()), this, SLOT(start()) );
	connect(ui->stopPB, SIGNAL(pressed()), this, SLOT(stop()) );
	connect(ui->pausePB, SIGNAL(pressed()), this, SLOT(pause()) );

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
	qDebug() << "Load Pressed";
}

void Tester::check()
{
	qDebug() << "Check Pressed ";
	qDebug() << "Angle Value: " << ui->angle->text();
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
