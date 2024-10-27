
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include "QtProcessScreen.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	try {
		QtProcessScreen qtprocesscreen;
		qtprocesscreen.show();
		return a.exec();
	} catch (std::exception &e){
		qDebug() << e.what();
	} catch(...) {
		qDebug() << "What is the excption???";
	}

}
