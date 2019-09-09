#include "sudoku9x9.h"

#ifndef SUDOKU9x9IRREGULAR
#define SUDOKU9x9IRREGULAR

class Sudoku9x9Irregular : public Sudoku9x9 {
    private:

    public:
        GridOfRegions *GoR;

        Sudoku9x9Irregular();
        //Sudoku9x9Irregular(int Given[9][9], int type = 5);
        ~Sudoku9x9Irregular();

        void operator=(Sudoku9x9Irregular &sudoku);

        void UpdatePossGrid();

        void PrintToConsole();
};

#endif

Sudoku9x9Irregular::Sudoku9x9Irregular(){
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++) {
            GivenGrid[i][j] = 0;
            CurrentGrid[i][j] = 0;
        }
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            for (int k = 0; k < 9; k++)
                PossibilitiesGrid[i][j][k] = true;
    setType(5);
    setStatus(0);
    setDifficulty(0);
    setSeed(0);    

    GoR = new GridOfRegions;
}

Sudoku9x9Irregular::~Sudoku9x9Irregular(){
    (*GoR).~GridOfRegions();    
}

void Sudoku9x9Irregular::operator=(Sudoku9x9Irregular &sudoku){
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++) {
            GivenGrid[i][j] = sudoku.GivenGrid[i][j];
            CurrentGrid[i][j] = sudoku.CurrentGrid[i][j];
        }
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            for (int k = 0; k < 9; k++)
                PossibilitiesGrid[i][j][k] = sudoku.PossibilitiesGrid[i][j][k];
    setType(sudoku.getType());
    setStatus(sudoku.getStatus());
    setDifficulty(sudoku.getDifficulty());
    setSeed(sudoku.getSeed());

    GoR = sudoku.GoR;   
}



void Sudoku9x9Irregular::PrintToConsole(){
    int Width = (*GoR).getGridWidth();
    int Hight = (*GoR).getGridHight();

    for (int r = 0; r < Hight; r++) {
        for (int c = 0; c < Width; c++) {

            if ( CurrentGrid[r][c] == 0 )
                std::cout << "\033[1;31m";
            else if( CurrentGrid[r][c] == GivenGrid[r][c])
                 std::cout << "\033[1;37m";
            else
                std::cout << "\033[1;32m";

            std::cout << CurrentGrid[r][c];
        }

        std::cout<<" ";

        for( int c=0; c<Width; c++ ){
            if( (*GoR).Grid[r*Width+c] == -1 ){
                std::cout <<"\033[0m";
                std::cout<<"-";
            }
            else{
                int d = (*GoR).Grid[r*Width+c];
                if( d == 0 )
                    std::cout << "\033[1;30m";
                else if( d == 1 )
                    std::cout << "\033[1;31m";
                else if( d == 2 )
                    std::cout << "\033[1;32m";
                else if( d == 3 )
                    std::cout << "\033[1;33m";
                else if( d == 4 )
                    std::cout << "\033[1;34m";
                else if( d == 5 )
                    std::cout << "\033[1;35m";
                else if( d == 6 )
                    std::cout << "\033[1;36m";
                else if( d == 7 )
                    std::cout << "\033[1;37m";
                else if( d == 8 )
                    std::cout << "\033[0m";
                
                
                std::cout<<d<<" ";
            }
        }
        std::cout << std::endl;
    }
    std::cout<<"\033[0m"<<"\n";
    return;
}

void Sudoku9x9Irregular::UpdatePossGrid(){
    // Standard possibilities grid updating
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            if (CurrentGrid[r][c] != 0) {
                int D = CurrentGrid[r][c];
                // eliminating possibilities in the exact place
                for (int d = 0; d < 9; d++) if (d != D-1) {
                    PossibilitiesGrid[r][c][d] = false;
                }
                // eliminating possibilities in the row
                for (int c2 = 0; c2 < 9; c2++) if (c2 != c) {
                    PossibilitiesGrid[r][c2][D-1] = false;
                }
                // eliminating possibilities in the column
                for (int r2 = 0; r2 < 9; r2++) if (r2 != r) {
                    PossibilitiesGrid[r2][c][D-1] = false;
                }
                int reg = (*GoR).Grid[r*9+c];
                // eliminating possibilities in the region
                for( int Cell=0; Cell<81; Cell++ ){
                    if ( (*GoR).Grid[Cell] == reg && Cell != r*9+c ) {
                        PossibilitiesGrid[Cell/9][Cell%9][D-1] = false;
                    }
                }
            }
        }
    }
}