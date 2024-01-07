#include <iostream>
#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QThread>
#include <acai_client_types.h>
#include <acai_client.h>

//#include "QELineEdit.h"
#include "QtProcessScreen.h"
//#include "ui_QtProcessScreen.h"

using namespace std;

QtProcessScreen::QtProcessScreen(QWidget *parent):QWidget(parent),ui(new Ui::Form)
{
	ui->setupUi(this);
	timerId = startTimer(1000);
}

QtProcessScreen::~QtProcessScreen()
{
	killTimer(timerId);
	delete ui;
}


void QtProcessScreen::timerEvent(QTimerEvent *event)
{
	if(ui->label_2==nullptr) return;

	QLabel *pTimeLabel = ui->label_2;
	QString time_format = "yyyy-MM-dd\nHH:mm:ss";
	static int count = 150;

	count = (count>=250)?150:count+10;

	QDateTime datetime = QDateTime::currentDateTime();
	QString strdatetime = datetime.toString(time_format);

	QString bcolor = QString("background-color:rgba(0,%1,0,%1)").arg(count);

	pTimeLabel -> setStyleSheet(bcolor);
	pTimeLabel -> setText(strdatetime);
}
