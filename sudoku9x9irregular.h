#include "sudoku9x9.h"

#ifndef SUDOKU9x9IRREGULAR
#define SUDOKU9x9IRREGULAR

class Sudoku9x9Irregular : public Sudoku9x9 {
    private:

    public:
        GridOfRegions GoR;

        Sudoku9x9Irregular();

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
    setStatus(0);
    setDifficulty(0);
    setSeed(0);    
}

void Sudoku9x9Irregular::PrintToConsole(){
    int Width = GoR.getGridWidth();
    int Hight = GoR.getGridHight();
    int Number = GoR.getNumberOfRegions();

    for (int r = 0; r < Hight; r++) {
        for (int c = 0; c < Width; c++) {

            if ( CurrentGrid[r][c] == 0 )
                std::cout << "\033[1;31m";
            else if( CurrentGrid[r][c] == GivenGrid[r][c])
                 std::cout << "\033[1;37m";
            else
                std::cout << "\033[1;32m";

            std::cout << CurrentGrid[r][c];
            std::cout <<"\033[0m";
        }

        std::cout<<" ";

        for( int c=0; c<Width; c++ ){
            if( GoR.Grid[r*Width+c] == -1 ){
                std::cout <<"\033[0m";
                std::cout<<"-";
            }
            else{
                int d = GoR.Grid[r*Width+c];
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
                
                
                std::cout<<d;
            }
        }
        std::cout << std::endl;
    }
}