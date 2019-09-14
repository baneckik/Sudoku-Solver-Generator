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

    /*
    int sudokuTypes[6] = {1,2,1,2,2,2};
    int sudokuSeeds[6] = {1,2,3,4,5,6};
    bool IsRestricted[6] = {0,0,1,1,1,1};
    int DigitsToAdd[6] = {19,2,0,19,2,0};
    bool IsSolved[6] = {0,1,0,0,0,1};

    w.openGenerateSudokuWindow(sudokuTypes,sudokuSeeds,IsRestricted,DigitsToAdd,IsSolved);
    */

    srand(time(NULL));
    int r = rand();
    int sudokuTypes[6] = {5,5,5,5,5,5};
    int sudokuSeeds[6] = {r,r+1,r+2,r+3,r+4,r+5};
    bool IsRestricted[6] = {0,0,0,0,0,0};
    int DigitsToAdd[6] = {10,10,10,10,10,10};
    bool IsSolved[6] = {0,0,0,0,0,0};

    w.openGenerateSudokuWindow(sudokuTypes,sudokuSeeds,IsRestricted,DigitsToAdd,IsSolved);

    // do Classica 0, co Diag 3
    // do noncon dodalem , do ak 5
    // Non-Con: 1,3,7,13,14,17
    // Anti-Knight: 2,4,5,6,10,11
    //GenerateWindow g;
    //g.show();

    return a.exec();
}
