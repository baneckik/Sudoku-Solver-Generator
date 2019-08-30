#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QButtonGroup>
#include <QToolButton>
#include <QString>
#include "solvegeneratefunctions.h"
#include <QPrinter>
#include <QPrintDialog>
#include <QtGui>
#include <QtCore>
#include <QComboBox>

void CreateTypeCombo(QComboBox *sudokuType){
    sudokuType->addItem("-None-");
    sudokuType->addItem("Classic");
    sudokuType->addItem("Diagonal");
    sudokuType->addItem("Non-Consecutive");
    sudokuType->addItem("Anti-Knight");
}

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

        QLabel *ChooseLabel = new QLabel("Choose Sudoku type:");
        QComboBox *sudoku1Type = new QComboBox;
        QComboBox *sudoku2Type = new QComboBox;
        QComboBox *sudoku3Type = new QComboBox;
        QComboBox *sudoku4Type = new QComboBox;
        QComboBox *sudoku5Type = new QComboBox;
        QComboBox *sudoku6Type = new QComboBox;
        QGridLayout *SudokuChoose = new QGridLayout;
            SudokuChoose->addWidget(ChooseLabel,0,0,1,1);
            CreateTypeCombo(sudoku1Type);
            SudokuChoose->addWidget(sudoku1Type,1,0,1,1);

            SudokuChoose->addWidget(ChooseLabel,0,1,1,1);
            CreateTypeCombo(sudoku2Type);
            SudokuChoose->addWidget(sudoku2Type,1,1,1,1);

            SudokuChoose->addWidget(ChooseLabel,2,0,1,1);
            CreateTypeCombo(sudoku3Type);
            SudokuChoose->addWidget(sudoku3Type,3,0,1,1);

            SudokuChoose->addWidget(ChooseLabel,2,1,1,1);
            CreateTypeCombo(sudoku4Type);
            SudokuChoose->addWidget(sudoku4Type,3,1,1,1);

            SudokuChoose->addWidget(ChooseLabel,4,0,1,1);
            CreateTypeCombo(sudoku5Type);
            SudokuChoose->addWidget(sudoku5Type,5,0,1,1);

            SudokuChoose->addWidget(ChooseLabel,4,1,1,1);
            CreateTypeCombo(sudoku6Type);
            SudokuChoose->addWidget(sudoku6Type,5,1,1,1);


        mainlayout->addItem(SudokuChoose);

        QLabel *Outro = new QLabel("All right then, let's go!");
        mainlayout->addWidget(Outro);

        QPushButton *GO = new QPushButton("GO!");
        mainlayout->addWidget(GO);

    QWidget *IntroWindow = new QWidget();
    IntroWindow->setWindowTitle("Intro Window");
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

void AddSudokuToPainter(QPainter &painter, int size, int X, int Y, Sudoku9x9 sudoku){
    int startX = X, startY = Y; // top left corner coordinates of sudoku
    int type = sudoku.getType(), seed = sudoku.getSeed();

    QPen MainBoxPen(Qt::black, max(2,size/22), Qt::SolidLine);
    QPen SmallBoxPen(Qt::black, 1, Qt::SolidLine);
    QPen DiagonalPen(Qt::lightGray, max(3,size/33), Qt::DashLine);

    // Diagonals
    if ( sudoku.getType()==2 ){
        painter.setPen(DiagonalPen);
        painter.drawLine(startX,startY,startX+9*size,startY+9*size);
        painter.drawLine(startX,startY+9*size,startX+9*size,startY);
    }

    // Thick lines
    painter.setPen(MainBoxPen);
    painter.drawRect(startX,startY,9*size,9*size);
    if( type == 1 || type == 2 || type == 3 || type == 4 ){
        painter.drawRect(startX+3*size,startY,3*size,9*size);
        painter.drawRect(startX,startY+3*size,9*size,3*size);
    }

    // Narrow lines
    painter.setPen(SmallBoxPen);
    painter.drawRect(startX+size,startY,size,9*size);
    painter.drawRect(startX+4*size,startY,size,9*size);
    painter.drawRect(startX+7*size,startY,size,9*size);
    painter.drawRect(startX,startY+size,9*size,size);
    painter.drawRect(startX,startY+4*size,9*size,size);
    painter.drawRect(startX,startY+7*size,9*size,size);

    // Stars
    for (int i=0;i<sudoku.getDifficulty();i++){
        QPolygon star;
        star << QPoint(startX, startY) << QPoint(startX + 118, startY - 363)
             << QPoint(startX - 191, startY - 588) << QPoint(startX + 191, startY - 588)
             << QPoint(startX + 309, startY - 951)
             << QPoint(startX + 427, startY - 588) << QPoint(startX + 809, startY - 588)
             << QPoint(startX + 500, startY - 363) << QPoint(startX + 618, startY)
             << QPoint(startX + 309, startY - 225);
        double scaleFactor = (double)size/450/4;
        QTransform trans;
        trans.translate(-(scaleFactor-1)*startX + i*scaleFactor*1100,-(scaleFactor-1)*startY -scaleFactor*100);
        trans=trans.scale(scaleFactor,scaleFactor);
        QPolygon star2=trans.map(star);

        painter.drawPolygon(star2);
    }

    // Text

    QString s("Type: Unknown");
    if( type==1 )
        s="Type: Classic";
    else if( type==2 )
        s="Type: Diagonal";
    else if( type==3 )
        s="Type: Non-Con";
    else if( type==4 )
        s="Type: Anti-Knight";

    painter.setFont(QFont(painter.font().defaultFamily(),size/45));

    painter.drawText(startX+size*6,startY-size/4,s);
    painter.drawText(startX+size*3,startY+size*9+size/3,"seed=" + QString::number(seed));

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


void GenerateWindow::paintEvent(QPaintEvent *event)
{
    const QString fileName("/home/baneckik/Documents/Inne/C/YourSudoku.pdf");
    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    QPainter painter(&pdfWriter);
    this->setBaseSize(10000,15000);
    QPainter painter2(this);

    scene = new QGraphicsScene(this);


    srand( time(NULL) );
    //int seed = 1751815565; // generating seed
    int seed = rand(); // generating seed

    int size = 450; // single cell size
    int X = 300, Y = 250, startX, startY; // top left corner coordinates of sudoku

    QFont footerFont = painter.font();
    footerFont.setPointSize(3*footerFont.pointSize()/4);

    //int seeds[6] = {0,1,3,7,13,14};

    for(int iter=0; iter<6; iter++ ){
        Sudoku9x9 sudoku = Generate(seed + iter ,1);
        //sudoku.PermuteRowsCols(seed+iter);
        //sudoku.PermuteDigits(seed+iter);
        //RestrictDigits(sudoku);
        //AddDigits(sudoku,8);
        //sudoku = Solve(sudoku);

        startX = X + (iter%2)*11*size;
        startY = Y + (iter/2)*10*size;
        AddSudokuToPainter(painter, size-iter*50, startX, startY, sudoku);
        startX = X/10 + (iter%2)*11*size;
        startY = Y/10 + (iter/2)*10*size;
        AddSudokuToPainter(painter2, size/50, startX, startY, sudoku);
    }

    painter.setFont(footerFont);
    painter.drawText(7000,13700,"Krzysztof Banecki, all rights reserved" + QString(0x00A9));
}

MainWindow::~MainWindow()
{
    delete ui;
}



