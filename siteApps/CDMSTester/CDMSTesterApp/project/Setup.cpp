#include <iostream>
#include "Setup.h"
#include "ui_Setup.h"

using namespace std;

Setup::Setup(QWidget *parent):QTabWidget(parent),ui(new Ui::Setup)
{
	ui->setupUi(this);
}

Setup::~Setup()
{
	delete ui;
}
