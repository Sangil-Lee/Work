#include <QMessageBox>
#include <iostream>
#include "SignalChecker.h"
#include "ui_SignalCheck.h"

using namespace std;
//g++ mariadb_conn.cpp -I/usr/include -L/usr/lib/x86_64-linux-gnu -lmysqlclient

SignalChecker::SignalChecker(QWidget *parent):QDialog(parent), ui(new Ui::SignalCheck)
{
    // Define application wide macro substitutions.
    applicationMacros = "APPL=demo";

    
    // Read the UI
    ui->setupUi(this);
}


SignalChecker::~SignalChecker()
{
    delete ui;
}


void SignalChecker::close()
{
	hide();
}
