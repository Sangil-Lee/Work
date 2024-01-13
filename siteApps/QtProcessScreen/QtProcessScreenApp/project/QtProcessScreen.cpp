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
	this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
}

QtProcessScreen::~QtProcessScreen()
{
	killTimer(timerId);
	delete ui;
}


void QtProcessScreen::timerEvent(QTimerEvent *event)
{
	QLabel *pBeat = ui->qelabel;
	if(pBeat==nullptr) return;
	if(ui->label_2==nullptr) return;

	int beat = pBeat->text().toInt();

	QLabel *pTimeLabel = ui->label_2;
	QString time_format = "yyyy-MM-dd\nHH:mm:ss";
	static int count = 150;
	count = (count>=250)?150:count+10;

	QDateTime datetime;
	QString strdatetime;
	QString bcolor;
	if(gbeat == beat) {
		strdatetime = "Distconnected...";
		bcolor = QString("background-color:darkgray");
	} else {
		datetime = QDateTime::currentDateTime();
		strdatetime = datetime.toString(time_format);
		bcolor = QString("background-color:rgba(0,%1,0,%1)").arg(count);
	}

	pTimeLabel -> setStyleSheet(bcolor);
	pTimeLabel -> setText(strdatetime);
	gbeat = beat;
}

void QtProcessScreen::close()
{
	 QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Process Screen",
                                                                tr("Are you sure?\n"),
                                                                QMessageBox::Cancel | QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
		return;
    } else {
		while(QWidget *w = findChild<QWidget*>())
			delete w;
		qApp->quit();
    }
}

void QtProcessScreen::closeEvent(QCloseEvent *event)
{
	close();
}
