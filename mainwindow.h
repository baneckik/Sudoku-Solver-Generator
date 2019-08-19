#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QtGui>
#include <QWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QGroupBox>
#include <QGridLayout>
#include <QRadioButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
   void openTypesInfoWindow();
   void openGenerateSudokuWindow();

private:
   QWidget *SudokuInfoWindow;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

#ifndef GENERATE_WINDOW
#define GENERATE_WINDOW

class GenerateWindow : public QWidget
{
    Q_OBJECT
public:
 //  MyWidget();

protected:
    void paintEvent(QPaintEvent *event);
signals:

public slots:

};

#endif // GENERATE_WINDOW
