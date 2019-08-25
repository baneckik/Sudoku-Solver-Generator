#include <iostream>
#include <algorithm>

#ifndef SUDOKU9x9
#define SUDOKU9x9

class Sudoku9x9{
    private:
        int Type;       // 1 - Classic, 2 - Diagonal, 3 - Nonconsecutive, 4 - AntiKnight
        int Status;     // 0 - Unknown, 1 - Solved, 2 - Unsolvable(by this solver), 3 - Ambiguous, 4 - Contradictionary
        int Difficulty; // 0 - Unknown, 1 - Very Easy, 2 - Easy, 3 - Hard, 4 - Very Hard
        int Seed;       // only if sudoku is generated, otherwise equals 0
    public:
        int GivenGrid[9][9];        // [row][column]
        int CurrentGrid[9][9];      // [row][column]
        bool PossibilitiesGrid[9][9][9];     // [row][column][digit]

        Sudoku9x9(int Given[9][9], int type = 1);
        //Sudoku9x9(Sudoku9x9 &sudoku);

        int getType(){return Type;}
        int getStatus(){return Status;}
        int getDifficulty(){return Difficulty;}
        int getSeed(){return Seed;}

        void setStatus(int s){Status = s;}
        void setDifficulty(int d){Difficulty = d;}
        void setSeed(int s){Seed = s;}

        int N_Given();     // number of digits given
        int N_Current();   // number of digits currently placed on the grid
        int N_Possibilities(int r, int c); // number of digits possible at row r and column c
        int N_Possibilities(int d, int rc, bool type);
            // number of digits possible for a digit d in a row(type=0) or column(type=1) rc

        void UpdatePossGrid();
        bool InsertRandomDigit();

        void PrintToConsole();
};

#endif

Sudoku9x9::Sudoku9x9(int Given[9][9], int type) : Type(type) {
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++) {
            GivenGrid[i][j] = Given[i][j];
            CurrentGrid[i][j] = Given[i][j];
        }
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            for (int k = 0; k < 9; k++)
                PossibilitiesGrid[i][j][k] = 1;
    Status = 0;
    Difficulty = 0;
    Seed = 0;
}

/*
Sudoku9x9::Sudoku9x9(Sudoku9x9 &sudoku) : Type(sudoku.getType()),
                                        Difficulty(sudoku.getDifficulty()),
                                        Seed(sudoku.getSeed())
{
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++) {
            GivenGrid[i][j] = sudoku.GivenGrid[i][j];
            CurrentGrid[i][j] = sudoku.GivenGrid[i][j];
        }
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            for (int k = 0; k < 9; k++)
                PossibilitiesGrid[i][j][k] = 1;
    UpdatePossGrid();
    Status = 0;
}
*/

int TotalPoss(bool Possibilities[9][9][9], int d, int r1, int c1, int r2, int c2) {
    int suma = 0;
    for (int r = r1; r <= r2; r++)
        for (int c = c1; c <= c2; c++)
            suma += Possibilities[r][c][d];
    return suma;
}


int Sudoku9x9::N_Given(){
    int sum = 0;
    for(int i=0; i<9; i++)
        for(int j=0;j<9;j++)
            if(GivenGrid[i][j] != 0) sum++;
    return sum;
}

int Sudoku9x9::N_Current(){
    int sum = 0;
    for(int i=0; i<9; i++)
        for(int j=0;j<9;j++)
            if(CurrentGrid[i][j] != 0) sum++;
    return sum;
}

int Sudoku9x9::N_Possibilities(int r, int c){
    int sum = 0;
    for(int i=0; i<9; i++)
        if(PossibilitiesGrid[r][c][i] != 0) sum++;
    return sum;
}

int Sudoku9x9::N_Possibilities(int d, int rc, bool type){
    int sum = 0;
    if( !type ){
        for(int c=0; c<9; c++){
            sum += PossibilitiesGrid[rc][c][d];
        }
    }else{
        for(int r=0; r<9; r++){
            sum += PossibilitiesGrid[r][rc][d];
        }
    }
    return sum;
}

void Sudoku9x9::PrintToConsole(){
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {

            if ( CurrentGrid[r][c] == 0 )
                std::cout << "\033[1;31m";
            else if( CurrentGrid[r][c] == GivenGrid[r][c])
                 std::cout << "\033[1;37m";
            else
                std::cout << "\033[1;32m";

            std::cout << CurrentGrid[r][c];
            std::cout <<"\033[0m";
            if ((c + 1) % 3 == 0)std::cout << " ";
        }
        if ((r + 1) % 3 == 0)std::cout << std::endl;
        std::cout << std::endl;
    }
}

void Sudoku9x9::UpdatePossGrid(){
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
                // eliminating possibilities in the box
                int boxr = r/3, boxc = c/3;
                for (int r2 = 0; r2 < 3; r2++)
                    for (int c2 = 0; c2 < 3; c2++)
                        if ( boxr*3 + r2 != r || boxc*3 + c2 != c ) {
                            PossibilitiesGrid[boxr*3 + r2][boxc*3 + c2][D-1] = false;
                        }


            }
        }
    }

    // only for Diagonal sudoku
    if( getType() == 2 ){
        for(int c=0;c<9;c++){
            //first main diagonal
            if (CurrentGrid[c][c] != 0){
                int D = CurrentGrid[c][c];
                for (int c2 = 0; c2 < 9; c2++) if (c2 != c) {
                    PossibilitiesGrid[c2][c2][D-1] = false;
                }
            }
            //second main diagonal
            if (CurrentGrid[8-c][c] != 0){
                int D = CurrentGrid[8-c][c];
                for (int c2 = 0; c2 < 9; c2++) if (c2 != c) {
                    PossibilitiesGrid[8-c2][c2][D-1] = false;
                }
            }

        }
    }

    // Only for Non-Consecutive sudoku
    if( getType() == 3 ){
        // standard updating
        for(int r=0; r<9; r++){
            for(int c=0; c<9; c++){
                if( CurrentGrid[r][c] != 0 ){
                    int D = CurrentGrid[r][c];
                    // eliminating possibilities from the place to the left:
                    if( r != 0 ){
                        if( D != 1 ){
                                PossibilitiesGrid[r-1][c][D-2] = false;
                        }
                        if( D != 9 ){
                                PossibilitiesGrid[r-1][c][D] = false;
                        }
                    }
                    // eliminating possibilities from the place to the right:
                    if( r != 8 ){
                        if( D != 1 ){
                                PossibilitiesGrid[r+1][c][D-2] = false;
                        }
                        if( D != 9 ){
                                PossibilitiesGrid[r+1][c][D] = false;
                        }
                    }
                    // eliminating possibilities from the place to the top:
                    if( c != 0 ){
                        if( D != 1 ){
                                PossibilitiesGrid[r][c-1][D-2] = false;
                        }
                        if( D != 9 ){
                                PossibilitiesGrid[r][c-1][D] = false;
                        }
                    }
                    // eliminating possibilities from the place to the top:
                    if( c != 8 ){
                        if( D != 1 ){
                                PossibilitiesGrid[r][c+1][D-2] = false;
                        }
                        if( D != 9 ){
                                PossibilitiesGrid[r][c+1][D] = false;
                        }
                    }

                }
            }
        }

        // Elimination technique nr 1:
        for( int r=0; r<8; r++ ){
            for( int c=0; c<9; c++ ){
                if( CurrentGrid[r][c] == 0 ){
                    for( int d=1; d<8; d++ ){
                        // d - central digit (out of three)
                        int sum = 0;
                        for( int d1=0; d1<d-1; d1++ ){
                            if( PossibilitiesGrid[r][c][d1] == true ){ sum++; break; }
                        }
                        for( int d1=d+2; d1<9; d1++ ){
                            if( PossibilitiesGrid[r][c][d1] == true ) { sum++; break; }
                        }
                        if( sum == 0 ){
                            if( r != 0 )
                                PossibilitiesGrid[r-1][c][d] = false;
                            if( r != 8 )
                                PossibilitiesGrid[r+1][c][d] = false;
                            if( c != 0 )
                                PossibilitiesGrid[r][c-1][d] = false;
                            if( c != 8 )
                                PossibilitiesGrid[r][c+1][d] = false;
                            break;
                        }
                    }
                }
            }
        }
        // Elimination technique nr 2:
        for( int r=0; r<8; r++ ){
            for( int c=0; c<9; c++ ){
                if( CurrentGrid[r][c] == 0 ){
                    for( int d=0; d<8; d++ ){
                        // d - lower digit (out of two)
                        int sum = 0;
                        for( int d1=0; d1<d; d1++ ){
                            if( PossibilitiesGrid[r][c][d1] == true ){ sum++; break; }
                        }
                        for( int d1=d+2; d1<9; d1++ ){
                            if( PossibilitiesGrid[r][c][d1] == true ) { sum++; break; }
                        }
                        if( sum == 0 ){
                            if( r != 0 ){
                                PossibilitiesGrid[r-1][c][d] = false;
                                PossibilitiesGrid[r-1][c][d+1] = false;
                            }
                            if( r != 8 ){
                                PossibilitiesGrid[r+1][c][d] = false;
                                PossibilitiesGrid[r+1][c][d+1] = false;
                            }
                            if( c != 0 ){
                                PossibilitiesGrid[r][c-1][d] = false;
                                PossibilitiesGrid[r][c-1][d+1] = false;
                            }
                            if( c != 8 ){
                                PossibilitiesGrid[r][c+1][d] = false;
                                PossibilitiesGrid[r][c+1][d+1] = false;
                            }
                            break;
                        }
                    }
                }
            }
        }
        // Elimination technique nr 3:
        for( int r=0; r<9; r++ ){
            for( int d=0; d<9; d++){
                for( int c=1; c<8; c++ ){
                    // c - central column (out of three)
                    if( CurrentGrid[r][c] == 0 ){
                        int sum = 0;
                        for( int c1=0; c1<c-1; c1++ ){
                            if( PossibilitiesGrid[r][c1][d] == true ){ sum++; break; }
                        }
                        for( int c1=c+2; c1<9; c1++ ){
                            if( PossibilitiesGrid[r][c1][d] == true ) { sum++; break; }
                        }
                        if( sum == 0 ){
                            if( d != 0 )
                                PossibilitiesGrid[r][c][d-1] = false;
                            if( d != 8 )
                                PossibilitiesGrid[r][c][d+1] = false;
                            break;
                        }
                    }
                }
            }
        }
        for( int c=0; c<9; c++ ){
            for( int d=0; d<9; d++){
                for( int r=1; r<8; r++ ){
                    // r - central row (out of three)
                    if( CurrentGrid[r][c] == 0 ){
                        int sum = 0;
                        for( int r1=0; r1<r-1; r1++ ){
                            if( PossibilitiesGrid[r1][c][d] == true ){ sum++; break; }
                        }
                        for( int r1=r+2; r1<9; r1++ ){
                            if( PossibilitiesGrid[r1][c][d] == true ) { sum++; break; }
                        }
                        if( sum == 0 ){
                            if( d != 0 )
                                PossibilitiesGrid[r][c][d-1] = false;
                            if( d != 8 )
                                PossibilitiesGrid[r][c][d+1] = false;
                            break;
                        }
                    }
                }
            }
        }
        // Elimination technique nr 4:
        for( int r=0; r<9; r++ ){
            for( int d=0; d<9; d++){
                for( int c=0; c<8; c++ ){
                    // c - left column (out of two)
                    if( CurrentGrid[r][c] == 0 ){
                        int sum = 0;
                        for( int c1=0; c1<c; c1++ ){
                            if( PossibilitiesGrid[r][c1][d] == true ){ sum++; break; }
                        }
                        for( int c1=c+2; c1<9; c1++ ){
                            if( PossibilitiesGrid[r][c1][d] == true ) { sum++; break; }
                        }
                        if( sum == 0 ){
                            if( d != 0 ){
                                PossibilitiesGrid[r][c][d-1] = false;
                                PossibilitiesGrid[r][c+1][d-1] = false;
                            }
                            if( d != 8 ){
                                PossibilitiesGrid[r][c][d+1] = false;
                                PossibilitiesGrid[r][c+1][d+1] = false;
                            }
                            break;
                        }
                    }
                }
            }
        }
        for( int c=0; c<9; c++ ){
            for( int d=0; d<9; d++){
                for( int r=0; r<8; r++ ){
                    // r - left row (out of two)
                    if( CurrentGrid[r][c] == 0 ){
                        int sum = 0;
                        for( int r1=0; r1<r; r1++ ){
                            if( PossibilitiesGrid[r1][c][d] == true ){ sum++; break; }
                        }
                        for( int r1=r+2; r1<9; r1++ ){
                            if( PossibilitiesGrid[r1][c][d] == true ) { sum++; break; }
                        }
                        if( sum == 0 ){
                            if( d != 0 ){
                                PossibilitiesGrid[r][c][d-1] = false;
                                PossibilitiesGrid[r+1][c][d-1] = false;
                            }
                            if( d != 8 ){
                                PossibilitiesGrid[r][c][d+1] = false;
                                PossibilitiesGrid[r+1][c][d+1] = false;
                            }
                            break;
                        }
                    }
                }
            }
        }
        // Elimination technique nr 5
        for( int boxr=0; boxr<3; boxr++ ){
            for( int boxc=0; boxc<3; boxc++ ){
                for( int d=0; d<9; d++ ){
                    // center cross shape
                    if( PossibilitiesGrid[boxr*3][boxc*3][d] == false &&
                        PossibilitiesGrid[boxr*3+2][boxc*3][d] == false &&
                        PossibilitiesGrid[boxr*3][boxc*3+2][d] == false &&
                        PossibilitiesGrid[boxr*3+2][boxc*3+2][d] == false){
                        if( d != 0 )
                            PossibilitiesGrid[boxr*3+1][boxc*3+1][d-1] = false;
                        if( d != 8 )
                            PossibilitiesGrid[boxr*3+1][boxc*3+1][d+1] = false;
                    }
                    // left T-shape
                    if( PossibilitiesGrid[boxr*3][boxc*3+1][d] == false &&
                        PossibilitiesGrid[boxr*3][boxc*3+2][d] == false &&
                        PossibilitiesGrid[boxr*3+1][boxc*3+2][d] == false &&
                        PossibilitiesGrid[boxr*3+2][boxc*3+1][d] == false &&
                        PossibilitiesGrid[boxr*3+2][boxc*3+2][d] == false){
                        if( d != 0 )
                            PossibilitiesGrid[boxr*3+1][boxc*3][d-1] = false;
                        if( d != 8 )
                            PossibilitiesGrid[boxr*3+1][boxc*3][d+1] = false;
                    }
                    // right T-shape
                    if( PossibilitiesGrid[boxr*3][boxc*3][d] == false &&
                        PossibilitiesGrid[boxr*3][boxc*3+1][d] == false &&
                        PossibilitiesGrid[boxr*3+1][boxc*3][d] == false &&
                        PossibilitiesGrid[boxr*3+2][boxc*3][d] == false &&
                        PossibilitiesGrid[boxr*3+2][boxc*3+1][d] == false){
                        if( d != 0 )
                            PossibilitiesGrid[boxr*3+1][boxc*3+2][d-1] = false;
                        if( d != 8 )
                            PossibilitiesGrid[boxr*3+1][boxc*3+2][d+1] = false;
                    }
                    // upper T-shape
                    if( PossibilitiesGrid[boxr*3+1][boxc*3][d] == false &&
                        PossibilitiesGrid[boxr*3+1][boxc*3+2][d] == false &&
                        PossibilitiesGrid[boxr*3+2][boxc*3][d] == false &&
                        PossibilitiesGrid[boxr*3+2][boxc*3+1][d] == false &&
                        PossibilitiesGrid[boxr*3+2][boxc*3+2][d] == false){
                        if( d != 0 )
                            PossibilitiesGrid[boxr*3][boxc*3+1][d-1] = false;
                        if( d != 8 )
                            PossibilitiesGrid[boxr*3][boxc*3+1][d+1] = false;
                    }
                    // lower T-shape
                    if( PossibilitiesGrid[boxr*3][boxc*3][d] == false &&
                        PossibilitiesGrid[boxr*3][boxc*3+1][d] == false &&
                        PossibilitiesGrid[boxr*3][boxc*3+2][d] == false &&
                        PossibilitiesGrid[boxr*3+1][boxc*3][d] == false &&
                        PossibilitiesGrid[boxr*3+1][boxc*3+2][d] == false){
                        if( d != 0 )
                            PossibilitiesGrid[boxr*3+2][boxc*3+1][d-1] = false;
                        if( d != 8 )
                            PossibilitiesGrid[boxr*3+2][boxc*3+1][d+1] = false;
                    }
                }
            }
        }
        // Elimination technique nr 6 
        for( int boxr=0; boxr<3; boxr++ ){
            for( int boxc=0; boxc<3; boxc++ ){
                for( int d=0; d<9; d++ ){
                    // oxx
                    // xox
                    // xxx
                    if( PossibilitiesGrid[boxr*3][boxc*3][d] == true &&
                        PossibilitiesGrid[boxr*3][boxc*3+1][d] == false &&
                        PossibilitiesGrid[boxr*3][boxc*3+2][d] == false &&
                        PossibilitiesGrid[boxr*3+1][boxc*3][d] == false &&
                        PossibilitiesGrid[boxr*3+1][boxc*3+1][d] == true &&
                        PossibilitiesGrid[boxr*3+1][boxc*3+2][d] == false &&
                        PossibilitiesGrid[boxr*3+2][boxc*3][d] == false &&
                        PossibilitiesGrid[boxr*3+2][boxc*3+1][d] == false &&
                        PossibilitiesGrid[boxr*3+2][boxc*3+2][d] == false
                    ){
                        if( d != 0 )
                            PossibilitiesGrid[boxr*3][boxc*3+1][d-1] = false;
                        if( d != 8 )
                            PossibilitiesGrid[boxr*3][boxc*3+1][d+1] = false;
                        if( d != 0 )
                            PossibilitiesGrid[boxr*3+1][boxc*3][d-1] = false;
                        if( d != 8 )
                            PossibilitiesGrid[boxr*3+1][boxc*3][d+1] = false;
                    }
                    // xox
                    // oxx
                    // xxx
                    if( PossibilitiesGrid[boxr*3][boxc*3][d] == false &&
                        PossibilitiesGrid[boxr*3][boxc*3+1][d] == true &&
                        PossibilitiesGrid[boxr*3][boxc*3+2][d] == false &&
                        PossibilitiesGrid[boxr*3+1][boxc*3][d] == true &&
                        PossibilitiesGrid[boxr*3+1][boxc*3+1][d] == false &&
                        PossibilitiesGrid[boxr*3+1][boxc*3+2][d] == false &&
                        PossibilitiesGrid[boxr*3+2][boxc*3][d] == false &&
                        PossibilitiesGrid[boxr*3+2][boxc*3+1][d] == false &&
                        PossibilitiesGrid[boxr*3+2][boxc*3+2][d] == false
                    ){
                        if( d != 0 )
                            PossibilitiesGrid[boxr*3][boxc*3][d-1] = false;
                        if( d != 8 )
                            PossibilitiesGrid[boxr*3][boxc*3][d+1] = false;
                        if( d != 0 )
                            PossibilitiesGrid[boxr*3+1][boxc*3+1][d-1] = false;
                        if( d != 8 )
                            PossibilitiesGrid[boxr*3+1][boxc*3+1][d+1] = false;
                    }
                    // xox
                    // xxo
                    // xxx
                    if( PossibilitiesGrid[boxr*3][boxc*3][d] == false &&
                        PossibilitiesGrid[boxr*3][boxc*3+1][d] == true &&
                        PossibilitiesGrid[boxr*3][boxc*3+2][d] == false &&
                        PossibilitiesGrid[boxr*3+1][boxc*3][d] == false &&
                        PossibilitiesGrid[boxr*3+1][boxc*3+1][d] == false &&
                        PossibilitiesGrid[boxr*3+1][boxc*3+2][d] == true &&
                        PossibilitiesGrid[boxr*3+2][boxc*3][d] == false &&
                        PossibilitiesGrid[boxr*3+2][boxc*3+1][d] == false &&
                        PossibilitiesGrid[boxr*3+2][boxc*3+2][d] == false
                    ){
                        if( d != 0 )
                            PossibilitiesGrid[boxr*3][boxc*3+2][d-1] = false;
                        if( d != 8 )
                            PossibilitiesGrid[boxr*3][boxc*3+2][d+1] = false;
                        if( d != 0)
                            PossibilitiesGrid[boxr*3+1][boxc*3+1][d-1] = false;
                        if( d != 8 )
                            PossibilitiesGrid[boxr*3+1][boxc*3+1][d+1] = false;
                    }
                    // xxo
                    // xox
                    // xxx
                    if( PossibilitiesGrid[boxr*3][boxc*3][d] == false &&
                        PossibilitiesGrid[boxr*3][boxc*3+1][d] == false &&
                        PossibilitiesGrid[boxr*3][boxc*3+2][d] == true &&
                        PossibilitiesGrid[boxr*3+1][boxc*3][d] == false &&
                        PossibilitiesGrid[boxr*3+1][boxc*3+1][d] == true &&
                        PossibilitiesGrid[boxr*3+1][boxc*3+2][d] == false &&
                        PossibilitiesGrid[boxr*3+2][boxc*3][d] == false &&
                        PossibilitiesGrid[boxr*3+2][boxc*3+1][d] == false &&
                        PossibilitiesGrid[boxr*3+2][boxc*3+2][d] == false
                    ){
                        if( d != 0 )
                            PossibilitiesGrid[boxr*3][boxc*3+1][d-1] = false;
                        if( d != 8 )
                            PossibilitiesGrid[boxr*3][boxc*3+1][d+1] = false;
                        if( d != 0 )
                            PossibilitiesGrid[boxr*3+1][boxc*3+2][d-1] = false;
                        if( d != 8 )
                            PossibilitiesGrid[boxr*3+1][boxc*3+2][d+1] = false;
                    }
                    // xxx
                    // oxx
                    // xox
                    if( PossibilitiesGrid[boxr*3][boxc*3][d] == false &&
                        PossibilitiesGrid[boxr*3][boxc*3+1][d] == false &&
                        PossibilitiesGrid[boxr*3][boxc*3+2][d] == false &&
                        PossibilitiesGrid[boxr*3+1][boxc*3][d] == true &&
                        PossibilitiesGrid[boxr*3+1][boxc*3+1][d] == false &&
                        PossibilitiesGrid[boxr*3+1][boxc*3+2][d] == false &&
                        PossibilitiesGrid[boxr*3+2][boxc*3][d] == false &&
                        PossibilitiesGrid[boxr*3+2][boxc*3+1][d] == true &&
                        PossibilitiesGrid[boxr*3+2][boxc*3+2][d] == false
                    ){
                        if( d != 0 )
                            PossibilitiesGrid[boxr*3+1][boxc*3+1][d-1] = false;
                        if( d != 8 )
                            PossibilitiesGrid[boxr*3+1][boxc*3+1][d+1] = false;
                        if( d != 0 )
                            PossibilitiesGrid[boxr*3+2][boxc*3][d-1] = false;
                        if( d != 8 )
                            PossibilitiesGrid[boxr*3+2][boxc*3][d+1] = false;
                    }
                    // xxx
                    // xox
                    // oxx
                    if( PossibilitiesGrid[boxr*3][boxc*3][d] == false &&
                        PossibilitiesGrid[boxr*3][boxc*3+1][d] == false &&
                        PossibilitiesGrid[boxr*3][boxc*3+2][d] == false &&
                        PossibilitiesGrid[boxr*3+1][boxc*3][d] == false &&
                        PossibilitiesGrid[boxr*3+1][boxc*3+1][d] == true &&
                        PossibilitiesGrid[boxr*3+1][boxc*3+2][d] == false &&
                        PossibilitiesGrid[boxr*3+2][boxc*3][d] == true &&
                        PossibilitiesGrid[boxr*3+2][boxc*3+1][d] == false &&
                        PossibilitiesGrid[boxr*3+2][boxc*3+2][d] == false
                    ){
                        if( d != 0 )
                            PossibilitiesGrid[boxr*3+1][boxc*3][d-1] = false;
                        if( d != 8 )
                            PossibilitiesGrid[boxr*3+1][boxc*3][d+1] = false;
                        if( d != 0 )
                            PossibilitiesGrid[boxr*3+2][boxc*3+1][d-1] = false;
                        if( d != 8 )
                            PossibilitiesGrid[boxr*3+2][boxc*3+1][d+1] = false;
                    }
                    // xxx
                    // xox
                    // xxo
                    if( PossibilitiesGrid[boxr*3][boxc*3][d] == false &&
                        PossibilitiesGrid[boxr*3][boxc*3+1][d] == false &&
                        PossibilitiesGrid[boxr*3][boxc*3+2][d] == false &&
                        PossibilitiesGrid[boxr*3+1][boxc*3][d] == false &&
                        PossibilitiesGrid[boxr*3+1][boxc*3+1][d] == true &&
                        PossibilitiesGrid[boxr*3+1][boxc*3+2][d] == false &&
                        PossibilitiesGrid[boxr*3+2][boxc*3][d] == false &&
                        PossibilitiesGrid[boxr*3+2][boxc*3+1][d] == false &&
                        PossibilitiesGrid[boxr*3+2][boxc*3+2][d] == true
                    ){
                        if( d != 0 )
                            PossibilitiesGrid[boxr*3+1][boxc*3+2][d-1] = false;
                        if( d != 8 )
                            PossibilitiesGrid[boxr*3+1][boxc*3+2][d+1] = false;
                        if( d != 0 )
                            PossibilitiesGrid[boxr*3+2][boxc*3+1][d-1] = false;
                        if( d != 8 )
                            PossibilitiesGrid[boxr*3+2][boxc*3+1][d+1] = false;
                    }
                    // xxx
                    // xxo
                    // xox
                    if( PossibilitiesGrid[boxr*3][boxc*3][d] == false &&
                        PossibilitiesGrid[boxr*3][boxc*3+1][d] == false &&
                        PossibilitiesGrid[boxr*3][boxc*3+2][d] == false &&
                        PossibilitiesGrid[boxr*3+1][boxc*3][d] == false &&
                        PossibilitiesGrid[boxr*3+1][boxc*3+1][d] == false &&
                        PossibilitiesGrid[boxr*3+1][boxc*3+2][d] == true &&
                        PossibilitiesGrid[boxr*3+2][boxc*3][d] == false &&
                        PossibilitiesGrid[boxr*3+2][boxc*3+1][d] == true &&
                        PossibilitiesGrid[boxr*3+2][boxc*3+2][d] == false
                    ){
                        if( d != 0 )
                            PossibilitiesGrid[boxr*3+1][boxc*3+1][d-1] = false;
                        if( d != 8 )
                            PossibilitiesGrid[boxr*3+1][boxc*3+1][d+1] = false;
                        if( d != 0 )
                            PossibilitiesGrid[boxr*3+2][boxc*3+2][d-1] = false;
                        if( d != 8 )
                            PossibilitiesGrid[boxr*3+2][boxc*3+2][d+1] = false;
                    }
                    
                }
            }
        }
    }

    // trick 1

    // horizontally
    for (int d = 0; d < 9; d++) for (int boxr = 0; boxr < 3; boxr++) {
        //1
        if (TotalPoss(PossibilitiesGrid, d, 3*boxr, 0, 3*boxr + 1, 2) == 0)
            for (int c = 3; c<9; c++)
                PossibilitiesGrid[3*boxr + 2][c][d] = false;
        if (TotalPoss(PossibilitiesGrid, d, 3*boxr + 1, 0, 3*boxr + 2, 2) == 0)
            for (int c = 3; c<9; c++)
                PossibilitiesGrid[3*boxr][c][d] = false;
        if (TotalPoss(PossibilitiesGrid, d, 3*boxr, 0, 3*boxr, 2)
            + TotalPoss(PossibilitiesGrid, d, 3*boxr+2, 0, 3*boxr+2, 2) == 0)
            for (int c = 3; c<9; c++)
                PossibilitiesGrid[3*boxr+1][c][d] = false;
        //2
        if (TotalPoss(PossibilitiesGrid, d, 3*boxr, 3, 3*boxr + 1, 5) == 0){
            for (int c = 0; c<3; c++)
                PossibilitiesGrid[3*boxr + 2][c][d] = false;
            for (int c = 6; c<9; c++)
                PossibilitiesGrid[3*boxr + 2][c][d] = false;
        }
        if (TotalPoss(PossibilitiesGrid, d, 3*boxr + 1, 3, 3*boxr + 2, 5) == 0){
            for (int c = 0; c<3; c++)
                PossibilitiesGrid[3*boxr][c][d] = false;
            for (int c = 6; c<9; c++)
                PossibilitiesGrid[3*boxr][c][d] = false;
        }
        if (TotalPoss(PossibilitiesGrid, d, 3*boxr, 3, 3*boxr, 5)
            + TotalPoss(PossibilitiesGrid, d, 3*boxr+2, 3, 3*boxr+2, 5) == 0){
            for (int c = 0; c<3; c++)
                PossibilitiesGrid[3*boxr+1][c][d] = false;
            for (int c = 6; c<9; c++)
                PossibilitiesGrid[3*boxr+1][c][d] = false;
        }
        //3
        if (TotalPoss(PossibilitiesGrid, d, 3*boxr, 6, 3*boxr + 1, 8) == 0)
            for (int c = 0; c<6; c++)
                PossibilitiesGrid[3*boxr + 2][c][d] = false;
        if (TotalPoss(PossibilitiesGrid, d, 3*boxr + 1, 6, 3*boxr + 2, 8) == 0)
            for (int c = 0; c<6; c++)
                PossibilitiesGrid[3*boxr][c][d] = false;
        if (TotalPoss(PossibilitiesGrid, d, 3*boxr, 6, 3*boxr, 8)
            + TotalPoss(PossibilitiesGrid, d, 3*boxr+2, 6, 3*boxr+2, 8) == 0)
            for (int c = 0; c<6; c++)
                PossibilitiesGrid[3*boxr+1][c][d] = false;

    }
    // vertically
    for (int d = 0; d < 9; d++) for (int boxc = 0; boxc < 3; boxc++) {
        //1
        if (TotalPoss(PossibilitiesGrid, d, 0, 3*boxc, 2, 3*boxc + 1) == 0)
            for (int r = 3; r<9; r++)
                PossibilitiesGrid[r][3*boxc+2][d] = false;
        if (TotalPoss(PossibilitiesGrid, d, 0, 3*boxc+1, 2, 3*boxc+2) == 0)
            for (int r = 3; r<9; r++)
                PossibilitiesGrid[r][3*boxc][d] = false;
        if (TotalPoss(PossibilitiesGrid, d, 0, 3*boxc, 2, 3*boxc)
            + TotalPoss(PossibilitiesGrid, d, 0, 3*boxc+2, 2, 3*boxc+2) == 0)
            for (int r = 3; r<9; r++)
                PossibilitiesGrid[r][3*boxc+1][d] = false;
        //2
        if (TotalPoss(PossibilitiesGrid, d, 3, 3*boxc, 5, 3*boxc+1) == 0){
            for (int r = 0; r<3; r++)
                PossibilitiesGrid[r][3*boxc+2][d] = false;
            for (int r = 6; r<9; r++)
                PossibilitiesGrid[r][3*boxc+2][d] = false;
        }
        if (TotalPoss(PossibilitiesGrid, d, 3, 3*boxc+1, 5, 3*boxc+2) == 0){
            for (int r = 0; r<3; r++)
                PossibilitiesGrid[r][3*boxc][d] = false;
            for (int r = 6; r<9; r++)
                PossibilitiesGrid[r][3*boxc][d] = false;
        }
        if (TotalPoss(PossibilitiesGrid, d, 3, 3*boxc, 5, 3*boxc)
            + TotalPoss(PossibilitiesGrid, d, 3, 3*boxc+2, 5, 3*boxc+2) == 0){
            for (int r = 0; r<3; r++)
                PossibilitiesGrid[r][3*boxc+1][d] = false;
            for (int r = 6; r<9; r++)
                PossibilitiesGrid[r][3*boxc+1][d] = false;
        }
        //3
        if (TotalPoss(PossibilitiesGrid, d, 6, 3*boxc, 8, 3*boxc+1) == 0)
            for (int r = 0; r<6; r++)
                PossibilitiesGrid[r][3*boxc+2][d] = false;
        if (TotalPoss(PossibilitiesGrid, d, 6, 3*boxc+1, 8, 3*boxc+2) == 0)
            for (int r = 0; r<6; r++)
                PossibilitiesGrid[r][3*boxc][d] = false;
        if (TotalPoss(PossibilitiesGrid, d, 6, 3*boxc, 8, 3*boxc)
            + TotalPoss(PossibilitiesGrid, d, 6, 3*boxc+2, 8, 3*boxc+2) == 0)
            for (int r = 0; r<6; r++)
                PossibilitiesGrid[r][3*boxc+1][d] = false;
    }
}

bool Sudoku9x9::InsertRandomDigit(){
    /* Function inserts random digit at a random place in the grid (if it's possible).
    Function returns true if random digit was succesfully inserted & false otherwise. */

    UpdatePossGrid();

    int r=10, c=10, iter=0, max_iter=100;
    do{
        r = rand()%9;
        c = rand()%9;
        iter++;
    } while ( N_Possibilities(r,c) <= 1 && iter < max_iter );

    if( iter == max_iter ) return false;

    int digits[] = {0,1,2,3,4,5,6,7,8};
    std::random_shuffle(digits,digits+9);

    for(int d=0; d<9; d++){
        if( PossibilitiesGrid[r][c][digits[d]] == 1 ){
            GivenGrid[r][c] = digits[d]+1;
            CurrentGrid[r][c] = digits[d]+1;
            break;
        }
    }
    return true;
}
