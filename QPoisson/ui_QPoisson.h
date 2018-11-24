/********************************************************************************
** Form generated from reading UI file 'QPoisson.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QPOISSON_H
#define UI_QPOISSON_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QPoissonClass
{
public:

    void setupUi(QWidget *QPoissonClass)
    {
        if (QPoissonClass->objectName().isEmpty())
            QPoissonClass->setObjectName(QStringLiteral("QPoissonClass"));
        QPoissonClass->resize(600, 400);

        retranslateUi(QPoissonClass);

        QMetaObject::connectSlotsByName(QPoissonClass);
    } // setupUi

    void retranslateUi(QWidget *QPoissonClass)
    {
        QPoissonClass->setWindowTitle(QApplication::translate("QPoissonClass", "QPoisson", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class QPoissonClass: public Ui_QPoissonClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QPOISSON_H
