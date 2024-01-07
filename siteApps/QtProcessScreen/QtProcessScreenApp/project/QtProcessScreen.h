/* QtProcessScreen.h
* Qt Process Application Demo. for TBOG
*/
#ifndef  QTPROCESSSCREEN_WIDGET_H
#define  QTPROCESSSCREEN_WIDGET_H

#include <QWidget> 
#include "ui_QtProcessScreen.h"

namespace Ui {
	class QtProcessScreen{};
};

class QtProcessScreen : public QWidget
{
	Q_OBJECT

public:
	explicit QtProcessScreen(QWidget *widget = 0);
	~QtProcessScreen();
	
public slots:

private:
	Ui::Form *ui;
	int timerId;

protected:
	void timerEvent(QTimerEvent *event);
//	void closeEvent(QCloseEvent *event);
public:

};
#endif  //
