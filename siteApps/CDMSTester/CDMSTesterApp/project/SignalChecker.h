/*  SignalCheck.h
 *
 */

#ifndef SIGNAL_CHECKER_DIALOG_H
#define SIGNAL_CHECKER_DIALOG_H

#include <QDialog>
#include <QStringList>
#include <QDebug>

namespace Ui {
class SignalCheck;
}

class SignalChecker;

class SignalChecker : public QDialog
{
    Q_OBJECT

public:
    explicit SignalChecker(QWidget *parent = 0);
    ~SignalChecker();

private:

public slots:
	//void accept();
	void close();

private:
    QString applicationMacros; // Macro substitutions to be applied across the application
    Ui::SignalCheck   *ui;

};

#endif // LOGIN_DIALOG_H