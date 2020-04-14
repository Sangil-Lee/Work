/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QLabel *label;
    QComboBox *cmbCameraList;
    QPushButton *btnRefresh;
    QLabel *label_2;
    QComboBox *cmbPixelFormat;
    QPushButton *btnStart;
    QPushButton *btnStop;
    QLabel *laPic;
    QFrame *line;
    QLabel *label_3;
    QLineEdit *editGain;
    QCheckBox *chkGainAuto;
    QPushButton *btnGain;
    QCheckBox *chkShutterAuto;
    QLabel *label_4;
    QPushButton *btnShutter;
    QLineEdit *editShutter;
    QLabel *label_5;
    QLabel *label_6;
    QCheckBox *chkTriggerEnable;
    QLabel *label_7;
    QLabel *label_8;
    QFrame *line_2;
    QPushButton *btnRunSW;
    QComboBox *cmbTriggerSource;
    QLineEdit *editTriggerParameter;
    QPushButton *btnTriggerPara;
    QLabel *laRecvCount;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(1178, 684);
        label = new QLabel(Widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 20, 91, 17));
        cmbCameraList = new QComboBox(Widget);
        cmbCameraList->setObjectName(QString::fromUtf8("cmbCameraList"));
        cmbCameraList->setGeometry(QRect(113, 15, 251, 27));
        btnRefresh = new QPushButton(Widget);
        btnRefresh->setObjectName(QString::fromUtf8("btnRefresh"));
        btnRefresh->setGeometry(QRect(370, 16, 99, 27));
        label_2 = new QLabel(Widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(490, 21, 91, 17));
        cmbPixelFormat = new QComboBox(Widget);
        cmbPixelFormat->setObjectName(QString::fromUtf8("cmbPixelFormat"));
        cmbPixelFormat->setGeometry(QRect(584, 16, 141, 27));
        btnStart = new QPushButton(Widget);
        btnStart->setObjectName(QString::fromUtf8("btnStart"));
        btnStart->setGeometry(QRect(760, 17, 81, 27));
        btnStop = new QPushButton(Widget);
        btnStop->setObjectName(QString::fromUtf8("btnStop"));
        btnStop->setGeometry(QRect(850, 17, 81, 27));
        laPic = new QLabel(Widget);
        laPic->setObjectName(QString::fromUtf8("laPic"));
        laPic->setGeometry(QRect(20, 70, 800, 600));
        laPic->setFrameShape(QFrame::Panel);
        laPic->setAlignment(Qt::AlignCenter);
        line = new QFrame(Widget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(20, 47, 1141, 16));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        label_3 = new QLabel(Widget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(836, 77, 321, 17));
        editGain = new QLineEdit(Widget);
        editGain->setObjectName(QString::fromUtf8("editGain"));
        editGain->setGeometry(QRect(853, 107, 151, 27));
        chkGainAuto = new QCheckBox(Widget);
        chkGainAuto->setObjectName(QString::fromUtf8("chkGainAuto"));
        chkGainAuto->setGeometry(QRect(1009, 109, 71, 22));
        btnGain = new QPushButton(Widget);
        btnGain->setObjectName(QString::fromUtf8("btnGain"));
        btnGain->setGeometry(QRect(1090, 107, 71, 27));
        chkShutterAuto = new QCheckBox(Widget);
        chkShutterAuto->setObjectName(QString::fromUtf8("chkShutterAuto"));
        chkShutterAuto->setGeometry(QRect(1009, 189, 71, 22));
        label_4 = new QLabel(Widget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(836, 157, 331, 17));
        btnShutter = new QPushButton(Widget);
        btnShutter->setObjectName(QString::fromUtf8("btnShutter"));
        btnShutter->setGeometry(QRect(1090, 187, 71, 27));
        editShutter = new QLineEdit(Widget);
        editShutter->setObjectName(QString::fromUtf8("editShutter"));
        editShutter->setGeometry(QRect(853, 187, 151, 27));
        label_5 = new QLabel(Widget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(836, 240, 331, 17));
        label_6 = new QLabel(Widget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(855, 302, 61, 17));
        chkTriggerEnable = new QCheckBox(Widget);
        chkTriggerEnable->setObjectName(QString::fromUtf8("chkTriggerEnable"));
        chkTriggerEnable->setGeometry(QRect(850, 270, 101, 22));
        label_7 = new QLabel(Widget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(855, 333, 81, 17));
        label_8 = new QLabel(Widget);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(855, 370, 131, 17));
        line_2 = new QFrame(Widget);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setGeometry(QRect(857, 400, 301, 20));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        btnRunSW = new QPushButton(Widget);
        btnRunSW->setObjectName(QString::fromUtf8("btnRunSW"));
        btnRunSW->setGeometry(QRect(982, 364, 181, 27));
        cmbTriggerSource = new QComboBox(Widget);
        cmbTriggerSource->setObjectName(QString::fromUtf8("cmbTriggerSource"));
        cmbTriggerSource->setGeometry(QRect(950, 299, 141, 27));
        editTriggerParameter = new QLineEdit(Widget);
        editTriggerParameter->setObjectName(QString::fromUtf8("editTriggerParameter"));
        editTriggerParameter->setGeometry(QRect(950, 330, 141, 27));
        btnTriggerPara = new QPushButton(Widget);
        btnTriggerPara->setObjectName(QString::fromUtf8("btnTriggerPara"));
        btnTriggerPara->setGeometry(QRect(1100, 330, 71, 27));
        laRecvCount = new QLabel(Widget);
        laRecvCount->setObjectName(QString::fromUtf8("laRecvCount"));
        laRecvCount->setGeometry(QRect(840, 440, 331, 17));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", nullptr));
        label->setText(QApplication::translate("Widget", "Camera List : ", nullptr));
        btnRefresh->setText(QApplication::translate("Widget", "Refresh", nullptr));
        label_2->setText(QApplication::translate("Widget", "PixelFormat :", nullptr));
        btnStart->setText(QApplication::translate("Widget", "Start", nullptr));
        btnStop->setText(QApplication::translate("Widget", "Stop", nullptr));
        laPic->setText(QString());
        label_3->setText(QApplication::translate("Widget", "[  Gain ( 0 ~ 511)  ]", nullptr));
        chkGainAuto->setText(QApplication::translate("Widget", "Auto", nullptr));
        btnGain->setText(QApplication::translate("Widget", "Set", nullptr));
        chkShutterAuto->setText(QApplication::translate("Widget", "Auto", nullptr));
        label_4->setText(QApplication::translate("Widget", "[  Shutter ( 1 ~ 3600000000)  ]", nullptr));
        btnShutter->setText(QApplication::translate("Widget", "Set", nullptr));
        label_5->setText(QApplication::translate("Widget", "[  Trigger  ]", nullptr));
        label_6->setText(QApplication::translate("Widget", "Source : ", nullptr));
        chkTriggerEnable->setText(QApplication::translate("Widget", "Enabled", nullptr));
        label_7->setText(QApplication::translate("Widget", "Parameter : ", nullptr));
        label_8->setText(QApplication::translate("Widget", "Fire S/W Trigger :", nullptr));
        btnRunSW->setText(QApplication::translate("Widget", "Run S/W Trigger", nullptr));
        btnTriggerPara->setText(QApplication::translate("Widget", "Set", nullptr));
        laRecvCount->setText(QApplication::translate("Widget", "Recieve Frame Count : ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
