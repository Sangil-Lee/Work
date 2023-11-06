/********************************************************************************
** Form generated from reading UI file 'object.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OBJECT_H
#define UI_OBJECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QWidget>
#include "QESimpleShape.h"
#include "QSimpleShape.h"

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QESimpleShape *qesimpleshape;
    QSimpleShape *qsimpleshape;
    QFrame *line;
    QFrame *line_2;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(400, 300);
        qesimpleshape = new QESimpleShape(Form);
        qesimpleshape->setObjectName(QString::fromUtf8("qesimpleshape"));
        qesimpleshape->setGeometry(QRect(160, 100, 81, 41));
        qesimpleshape->setShape(QSimpleShape::valve_h);
        qsimpleshape = new QSimpleShape(Form);
        qsimpleshape->setObjectName(QString::fromUtf8("qsimpleshape"));
        qsimpleshape->setGeometry(QRect(191, 112, 16, 16));
        qsimpleshape->setShape(QSimpleShape::circle);
        qsimpleshape->setSemiCycles(5);
        qsimpleshape->setPercentSize(1);
        qsimpleshape->setModulus(8);
        qsimpleshape->setIndent(3);
        qsimpleshape->setIsActive(true);
        qsimpleshape->setFlashOffColour(QColor(0, 0, 0));
        qsimpleshape->setProperty("colour0", QVariant(QColor(0, 0, 0)));
        line = new QFrame(Form);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(146, 100, 20, 40));
        line->setFrameShadow(QFrame::Plain);
        line->setLineWidth(3);
        line->setFrameShape(QFrame::VLine);
        line_2 = new QFrame(Form);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setGeometry(QRect(234, 100, 20, 40));
        line_2->setFrameShadow(QFrame::Plain);
        line_2->setLineWidth(3);
        line_2->setFrameShape(QFrame::VLine);

        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", nullptr));
        qesimpleshape->setProperty("variable", QVariant(QApplication::translate("Form", "ctrluser:ai1", nullptr)));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OBJECT_H
