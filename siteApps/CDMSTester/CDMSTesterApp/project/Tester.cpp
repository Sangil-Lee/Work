#include <iostream>
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
}

Tester::~Tester()
{
	delete ui;
}


void Tester::ShowLogin()
{
	plogin->show();
}

void Tester::showlogin()
{
	plogin->SetPasswd("");
	ShowLogin();
}

void Tester::showsetup()
{
	ShowSetup();
}
void Tester::ShowSetup()
{
	psetup->show();
}
