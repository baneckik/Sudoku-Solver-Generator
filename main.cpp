#include "mainwindow.h"
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QtGui>
#include <QtCore>
#include <QToolButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QButtonGroup>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("My sudoku solver & generator");

    //QWidget *IntroWindow = new QWidget;

    //IntroWindow->setLayout(mainlayout);

    //IntroWindow->show();
    //w.show();



    GenerateWindow g;
    g.show();

    return a.exec();
}
