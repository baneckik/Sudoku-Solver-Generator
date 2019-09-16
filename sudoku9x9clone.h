#include "sudoku9x9.h"

#ifndef SUDOKU9x9CLONE
#define SUDOKU9x9CLONE

class Sudoku9x9Clone : public Sudoku9x9 {
    private:
        int shift_r;
        int shift_c;
        int reg_Size;
    public:
        Region reg;
        Region reg_shifted;

        Sudoku9x9Clone();
        Sudoku9x9Clone(int region_size);

        void operator=(Sudoku9x9Clone &sudoku);

        void setShiftR(int sr){ shift_r = sr; };
        void setShiftC(int sc){ shift_c = sc; };
        int getShiftR(){ return shift_r; };
        int getShiftC(){ return shift_c; };

        void InsertRegionCell(int r, int c);
        void UpdateRegShifted();
        bool IsRegShiftedInGrid();

        //void UpdatePossGrid();
        //void InsertRandomDigit();

        void PrintToConsole();
};

#endif  //SUDOKU9x9CLONE

#ifndef SUDOKU9X9CLONE
#define SUDOKU9X9CLONE

Sudoku9x9Clone::Sudoku9x9Clone(){
    shift_r = 0;
    shift_c = 0;
    reg_Size = 1;
    reg.setWidth(9);
    reg.setHight(9);
    reg.Grid = new bool[81];
    for( int i=0;i<81; i++ ) reg.Grid[i] = false;
    reg_shifted.setWidth(9);
    reg_shifted.setHight(9);
    reg_shifted.Grid = new bool[81];
    for( int i=0;i<81; i++ ) reg_shifted.Grid[i] = false;
}

Sudoku9x9Clone::Sudoku9x9Clone(int region_size){
    shift_r = 0;
    shift_c = 0;
    reg_Size = region_size;
    reg.setWidth(9);
    reg.setHight(9);
    reg.Grid = new bool[81];
    for( int i=0;i<81; i++ ) reg.Grid[i] = false;
    reg_shifted.setWidth(9);
    reg_shifted.setHight(9);
    reg_shifted.Grid = new bool[81];
    for( int i=0;i<81; i++ ) reg_shifted.Grid[i] = false;
}

void Sudoku9x9Clone::InsertRegionCell(int r, int c){
    if( r<0 || c<0 || r>8 || c>8 || r+shift_r<0 || c+shift_c<0 || r+shift_r>8 || c+shift_c>8){
        return;
    }
    reg.Grid[r*9+c] = true;
    reg_shifted.Grid[(r+shift_r)*9+c-shift_c] = true;
}

void Sudoku9x9Clone::UpdateRegShifted(){
    for( int r=0; r<9; r++ ){
        for( int c=0; c<9; c++ ){
            if( r-shift_r>=0 && r-shift_r<9 && c-shift_c>=0 && c-shift_c<9 &&
            reg.Grid[(r-shift_r)*9+c-shift_c] ){
                reg_shifted.Grid[r*9+c] = true;
            }
            else{
                reg_shifted.Grid[r*9+c] = false;
            }
        }
    }
}

bool Sudoku9x9Clone::IsRegShiftedInGrid(){
    for( int r=0; r<9; r++ ){
        for( int c=0; c<9; c++ ){
            if( reg.Grid[r*9+c] && (r+shift_r<0 || r+shift_r>8 || c+shift_c<0 || c+shift_c>8) ){
                return false;
            }
        }
    }
    return true;
}

void Sudoku9x9Clone::PrintToConsole(){
    int Width = 9;
    int Hight = 9;

    for (int r = 0; r < Hight; r++) {
        for (int c = 0; c < Width; c++) {

            if ( CurrentGrid[r][c] == 0 )
                std::cout << "\033[1;31m";
            else if( CurrentGrid[r][c] == GivenGrid[r][c])
                 std::cout << "\033[1;37m";
            else
                std::cout << "\033[1;32m";

            std::cout << CurrentGrid[r][c] << " ";
        }

        std::cout<<" \033[0m";

        for( int c=0; c<Width; c++ ){
            if( !reg.Grid[r*Width+c] && !reg_shifted.Grid[r*9+c] ){
                std::cout<<"- ";
            }
            else if ( reg.Grid[r*Width+c] && reg_shifted.Grid[r*9+c] ){
                std::cout<<"x ";
            }
            else if ( !reg.Grid[r*Width+c] ){
                std::cout<<"1 ";
            }
            else{
                std::cout<<"0 ";
            }
        }
        std::cout << std::endl;
    }
    std::cout<<"\033[0m"<<"\n";
}

#endif
