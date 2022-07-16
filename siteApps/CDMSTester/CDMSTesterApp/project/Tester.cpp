#include <iostream>
#include "Tester.h"
#include "ui_Tester.h"

using namespace std;

Tester::Tester(QWidget *parent):QWidget(parent),ui(new Ui::Tester)
{
	ui->setupUi(this);
}

Tester::~Tester()
{
	delete ui;
}
