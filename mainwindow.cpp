#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QButtonGroup>
#include <QToolButton>
#include <QString>
#include "solvegeneratefunctions.h"
#include <QPrinter>
#include <QPrintDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setWindowTitle("My sudoku solver & generator");

    QVBoxLayout *mainlayout = new QVBoxLayout;

        QLabel *Intro = new QLabel("Welcome to my application, stranger. What can I do for you?");
        mainlayout->addWidget(Intro);

        QHBoxLayout *OptionsLayout = new QHBoxLayout;
            QSpacerItem *spacer = new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Expanding);
            OptionsLayout->addItem(spacer);
            QRadioButton *SolveOption = new QRadioButton("Solve sth for me");
            QRadioButton *GenerateOption = new QRadioButton("Generate sth for me");
            SolveOption->setChecked(true);
            OptionsLayout->addWidget(SolveOption);
            OptionsLayout->addWidget(GenerateOption);
            OptionsLayout->addItem(spacer);
        mainlayout->addLayout(OptionsLayout);

        QHBoxLayout *Choose1Layout = new QHBoxLayout;
            QLabel *Choose1 = new QLabel("Choose the type of sudoku you want to solve/generate:");
            Choose1Layout->addWidget(Choose1);
            QToolButton *SudokuTypesInfoButton = new QToolButton;
            SudokuTypesInfoButton->setText("?");

            Choose1Layout->addWidget(SudokuTypesInfoButton);
        mainlayout->addItem(Choose1Layout);

        QVBoxLayout *TypeOptions = new QVBoxLayout;
            QRadioButton *type1 = new QRadioButton("Classic");
            QRadioButton *type2 = new QRadioButton("NonConsecutive");
            QRadioButton *type3 = new QRadioButton("Diagonal");
            QRadioButton *type4 = new QRadioButton("Anti-Knight");
            type1->setChecked(true);
            TypeOptions->addWidget(type1);
            TypeOptions->addWidget(type2);
            TypeOptions->addWidget(type3);
            TypeOptions->addWidget(type4);
            TypeOptions->addStretch(200);
            QButtonGroup *TypeOptionsGroup = new QButtonGroup;
            TypeOptionsGroup->addButton(type1);
            TypeOptionsGroup->addButton(type2);
            TypeOptionsGroup->addButton(type3);
            TypeOptionsGroup->addButton(type4);
        mainlayout->addItem(TypeOptions);

        QLabel *Choose2 = new QLabel("Choose the difficulty of sudoku you want to generate:");
        mainlayout->addWidget(Choose2);

        QVBoxLayout *DifficultyOptions = new QVBoxLayout;
            QRadioButton *diff1 = new QRadioButton("Very Easy");
            QRadioButton *diff2 = new QRadioButton("Easy");
            QRadioButton *diff3 = new QRadioButton("Hard");
            QRadioButton *diff4 = new QRadioButton("Very Hard");
            diff1->setChecked(true);
            DifficultyOptions->addWidget(diff1);
            DifficultyOptions->addWidget(diff2);
            DifficultyOptions->addWidget(diff3);
            DifficultyOptions->addWidget(diff4);
            DifficultyOptions->addStretch(1);
            QButtonGroup *DiffOptionsGroup = new QButtonGroup;
            DiffOptionsGroup->addButton(diff1);
            DiffOptionsGroup->addButton(diff2);
            DiffOptionsGroup->addButton(diff3);
            DiffOptionsGroup->addButton(diff4);
        mainlayout->addItem(DifficultyOptions);

        QLabel *Outro = new QLabel("All right then, let's go!");
        mainlayout->addWidget(Outro);

        QPushButton *GO = new QPushButton("GO!");
        mainlayout->addWidget(GO);

    QWidget *IntroWindow = new QWidget();
    IntroWindow->setLayout(mainlayout);
    IntroWindow->show();

    connect(SudokuTypesInfoButton, SIGNAL(clicked()), this, SLOT(openTypesInfoWindow()));


    ui->setupUi(this);
}

void MainWindow::openTypesInfoWindow()
{
    QWidget *TypesInfoWindow = new QWidget;
    TypesInfoWindow->setWindowTitle("Sudoku types info");
    QLabel *InfoClassic = new QLabel("<h3>Classic Sudoku</h3>"
                                     "Fill the grid with numbers from 1 to 9. "
                                     "Each number can only appear once in every row, box and column of the grid.");
    QLabel *InfoNonCon = new QLabel("<h3>Non-consecutive Sudoku</h3>"
                                    "Follow the classic sudoku rules. In addition, none of the neighbour digits can "
                                    "go into the squares with shared edge.");
    QLabel *InfoDiagonal = new QLabel("<h3>Diagonal Sudoku</h3>"
                                      "Follow the classic sudoku rules. In addition, each number can only appear "
                                      "once in both main diagonals of the grid.");
    QLabel *InfoAntiKnight = new QLabel("<h3>Anti-Knight Sudoku</h3>"
                                        "Follow the classic sudoku rules. In addition, none of the numbers can "
                                        "go into the squares placed on the Chess-Knight move away.");

    QVBoxLayout *TypesInfoLayout = new QVBoxLayout;
    TypesInfoLayout->addWidget(InfoClassic);
    TypesInfoLayout->addWidget(InfoNonCon);
    TypesInfoLayout->addWidget(InfoDiagonal);
    TypesInfoLayout->addWidget(InfoAntiKnight);
    TypesInfoWindow->setLayout(TypesInfoLayout);

    TypesInfoWindow->show();
}

void MainWindow::openGenerateSudokuWindow(){
    QWidget *GenerateWindow = new QWidget;
    GenerateWindow->setWindowTitle("Your generated sudoku");

}

void GenerateWindow::paintEvent(QPaintEvent *event)
{
    const QString fileName("/home/baneckik/Documents/Inne/C/YourSudoku.pdf");
    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));


    srand( time(NULL) );
    //int seed = 1751815565; // generating seed
    int seed = rand(); // generating seed
    QPainter painter(&pdfWriter);
    int size = 450; // single cell size
    int X = 300, Y = 250, startX, startY; // top left corner coordinates of sudoku

    QPen MainBoxPen(Qt::black, 20, Qt::SolidLine);
    QPen SmallBoxPen(Qt::black, 1, Qt::SolidLine);
    QPen DiagonalPen(Qt::lightGray, 30, Qt::DashLine);


    for(int iter=0; iter<6; iter++ ){

        Sudoku9x9 sudoku = Generate(seed+iter,2);
        RestrictDigits(sudoku);

        startX = X + (iter%2)*11*size;
        startY = Y + (iter/2)*10*size;

        if ( sudoku.getType()==2 ){
            painter.setPen(DiagonalPen);
            painter.drawLine(startX,startY,startX+9*size,startY+9*size);
            painter.drawLine(startX,startY+9*size,startX+9*size,startY);
        }

        painter.setPen(MainBoxPen);
        painter.drawRect(startX,startY,9*size,9*size);
        painter.drawRect(startX+3*size,startY,3*size,9*size);
        painter.drawRect(startX,startY+3*size,9*size,3*size);

        painter.setPen(SmallBoxPen);
        painter.drawRect(startX+size,startY,size,9*size);
        painter.drawRect(startX+4*size,startY,size,9*size);
        painter.drawRect(startX+7*size,startY,size,9*size);
        painter.drawRect(startX,startY+size,9*size,size);
        painter.drawRect(startX,startY+4*size,9*size,size);
        painter.drawRect(startX,startY+7*size,9*size,size);

        QString s("Type: Unknown");
        if( sudoku.getType()==1 )
            s="Type: Classic";
        else if( sudoku.getType()==2 )
            s="Type: Diagonal";
        else if( sudoku.getType()==3 )
            s="Type: Non-Con";
        else if( sudoku.getType()==4 )
            s="Type: Anti-Knight";

        painter.drawText(startX+size*6,startY-110,s);
        //painter.drawText(startX+size*3,startY+size*9+150,"seed=" + QString::number(sudoku.getSeed()));
        painter.drawText(startX+size*3,startY+size*9+150,"seed=" + QString::number(seed+iter));

        for (int i=0;i<sudoku.getDifficulty();i++){
            QPolygon star;
            star << QPoint(startX, startY) << QPoint(startX + 118, startY - 363)
                 << QPoint(startX - 191, startY - 588) << QPoint(startX + 191, startY - 588)
                 << QPoint(startX + 309, startY - 951)
                 << QPoint(startX + 427, startY - 588) << QPoint(startX + 809, startY - 588)
                 << QPoint(startX + 500, startY - 363) << QPoint(startX + 618, startY)
                 << QPoint(startX + 309, startY - 225);
            double scaleFactor = 0.25;
            QTransform trans;
            trans.translate(-(scaleFactor-1)*startX + i*scaleFactor*1100,-(scaleFactor-1)*startY -scaleFactor*100);
            trans=trans.scale(scaleFactor,scaleFactor);
            QPolygon star2=trans.map(star);

            painter.drawPolygon(star2);
        }
        /*
        for (int i=0;i<sudoku.getDifficulty();i++){
            QPolygon star;
            star << QPoint(26*i+startX+4, startY-4) << QPoint(26*i+startX+7, startY-12)
                 << QPoint(26*i+startX+2, startY-18) << QPoint(26*i+startX+8, startY-18)
                 << QPoint(26*i+startX+12, startY-26)
                 << QPoint(26*i+startX+16, startY-18) << QPoint(26*i+startX+22, startY-18)
                 << QPoint(26*i+startX+17, startY-12) << QPoint(26*i+startX+20, startY-4)
                 << QPoint(26*i+startX+12, startY-10);
            int scaleFactor = 10;
            QTransform trans;
            trans.translate(-(scaleFactor-1)*(startX+2),-(scaleFactor-1)*(startY+2));
            trans=trans.scale(scaleFactor,scaleFactor);
            QPolygon star2=trans.map(star);

            painter.drawPolygon(star2);
        }
        */

        //sudoku = Solve(sudoku);

        QFont digitFont = painter.font();
        digitFont.setPointSize(digitFont.pointSize()*2);
        painter.setFont(digitFont);
        QString * digit = new QString;

        for( int r=0;r<9;r++){
            for( int c=0;c<9;c++ ){
                if( sudoku.CurrentGrid[r][c] != 0 ){
                    digit->setNum(sudoku.CurrentGrid[r][c]);
                    if ( sudoku.CurrentGrid[r][c] == sudoku.GivenGrid[r][c] ){
                        painter.setOpacity(1);
                    }else {
                        painter.setOpacity(0.4);
                    }
                    painter.drawText(startX+size*r+size*30/100,startY+size*c+78*size/100,*digit);
                }
            }
        }
        painter.setOpacity(1);


        digitFont.setPointSize(digitFont.pointSize()/2);
        painter.setFont(digitFont);

    }

    QFont footerFont = painter.font();
    footerFont.setPointSize(3*footerFont.pointSize()/4);
    painter.setFont(footerFont);
    painter.drawText(7000,13700,"Krzysztof Banecki, all rights reserved" + QString(0x00A9));

}

MainWindow::~MainWindow()
{
    delete ui;
}



