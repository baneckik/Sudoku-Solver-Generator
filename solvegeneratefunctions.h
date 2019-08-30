#include <vector>
#include "sudoku9x9.h"
using namespace std;


bool IsContradictory(Sudoku9x9 sudoku){

    // if there is a place in the grid with no possible digit
    int suma = 0;
    for (int r = 0; r < 9; r++)
        for (int c = 0; c < 9; c++) {
            for (int d = 0; d < 9; d++)
                suma += sudoku.PossibilitiesGrid[r][c][d];
            if (suma == 0) return true;
            suma = 0;
        }

    vector<int> quantities = {0,0,0,0,0,0,0,0,0};

    // if there is at most one digit in each row
    // and if there is at least one possibilitie for every digit in each row
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++)
            if (sudoku.CurrentGrid[r][c] != 0) quantities[sudoku.CurrentGrid[r][c] - 1]++;
        if ( any_of(quantities.begin(), quantities.end(), [](int i){return i>1;}) ) return true;
        for (int i = 0; i < 9; i++) quantities[i] = 0;

        for (int d=0; d<9; d++){
            for (int c = 0; c < 9; c++)
                quantities[d] += sudoku.PossibilitiesGrid[r][c][d];
        }
        if ( any_of(quantities.begin(), quantities.end(), [](int i){return i==0;}) ) return true;
        for (int i = 0; i < 9; i++) quantities[i] = 0;

    }
    // if there is at most one digit in each column
    // and if there is at least one possibilitie for every digit in each column
    for (int c = 0; c < 9; c++) {
        for (int r = 0; r < 9; r++)
            if (sudoku.CurrentGrid[r][c] != 0) quantities[sudoku.CurrentGrid[r][c] - 1]++;
        if ( any_of(quantities.begin(), quantities.end(), [](int i){return i>1;}) ) return true;
        for (int i = 0; i < 9; i++) quantities[i] = 0;

        for (int d=0; d<9; d++){
            for (int r = 0; r < 9; r++)
                quantities[d] += sudoku.PossibilitiesGrid[r][c][d];
        }
        if ( any_of(quantities.begin(), quantities.end(), [](int i){return i==0;}) ) return true;
        for (int i = 0; i < 9; i++) quantities[i] = 0;
    }

    // if there is at most one digit in each box
    // and if there is at least one possibilitie for every digit in each box
    for( int boxr=0; boxr<3; boxr++){
        for( int boxc=0; boxc<3; boxc++){
            for (int r = 0; r < 3; r++) {
                for (int c = 0; c < 3; c++)
                    if (sudoku.CurrentGrid[boxr*3 + r][boxc*3 + c] != 0)
                        quantities[sudoku.CurrentGrid[boxr*3 + r][boxc*3 + c] - 1]++;
                if ( any_of(quantities.begin(), quantities.end(), [](int i){return i>1;}) ) return true;
            }
            for (int i = 0; i < 9; i++) quantities[i] = 0;

            for (int d=0; d<9; d++){
                for (int r = 0; r < 3; r++) for (int c = 0; c < 3; c++)
                    quantities[d] += sudoku.PossibilitiesGrid[boxr*3 + r][boxc*3 + c][d];
            }
            if ( any_of(quantities.begin(), quantities.end(), [](int i){return i==0;}) ) return true;
            for (int i = 0; i < 9; i++) quantities[i] = 0;
        }
    }

    // only for Diagonal sudoku
    if(sudoku.getType()==2){
        for (int c = 0; c < 9; c++)
            if (sudoku.CurrentGrid[c][c] != 0) quantities[sudoku.CurrentGrid[c][c] - 1]++;
        if ( any_of(quantities.begin(), quantities.end(), [](int i){return i>1;}) ) return true;
        for (int i = 0; i < 9; i++) quantities[i] = 0;

        for (int c = 0; c < 9; c++)
            if (sudoku.CurrentGrid[8-c][c] != 0) quantities[sudoku.CurrentGrid[8-c][c] - 1]++;
        if ( any_of(quantities.begin(), quantities.end(), [](int i){return i>1;}) ) return true;
    }

    // only for Non-Consecutive sudoku
    if( sudoku.getType() == 3 ){
        for(int r=0; r<9; r++){
            for(int c=0; c<9; c++){
                if( sudoku.CurrentGrid[r][c] != 0 ){
                    int D = sudoku.CurrentGrid[r][c];
                    if( r != 0 ){
                        if( D != 1 ){
                            if ( sudoku.CurrentGrid[r-1][c] == D-1 ) return true;
                        }
                        if( D != 9 ){
                            if ( sudoku.CurrentGrid[r-1][c] == D+1 ) return true;
                        }
                    }
                    if( r != 8 ){
                        if( D != 1 ){
                            if ( sudoku.CurrentGrid[r+1][c] == D-1 ) return true;
                        }
                        if( D != 9 ){
                            if ( sudoku.CurrentGrid[r+1][c] == D+1 ) return true;
                        }
                    }
                    // eliminating possibilities from the place to the top:
                    if( c != 0 ){
                        if( D != 1 ){
                            if ( sudoku.CurrentGrid[r][c-1] == D-1 ) return true;
                        }
                        if( D != 9 ){
                            if ( sudoku.CurrentGrid[r][c-1] == D+1 ) return true;
                        }
                    }
                    // eliminating possibilities from the place to the top:
                    if( c != 8 ){
                        if( D != 1 ){
                            if ( sudoku.CurrentGrid[r][c+1] == D-1 ) return true;
                        }
                        if( D != 9 ){
                            if ( sudoku.CurrentGrid[r][c+1] == D+1 ) return true;
                        }
                    }

                }
            }
        }
    }

    return false;
}

bool IsFilled(Sudoku9x9 sudoku) {
    int suma = 0;
    for (int r = 0; r < 9; r++)
        for (int c = 0; c < 9; c++)
            suma += sudoku.CurrentGrid[r][c];
    if (suma != 45 * 9) return false;
    return true;
}

void UpdatePossGrid_Classic(Sudoku9x9 &sudoku, bool &progress){
    // actualization of PossibilitiesGrid
    // for every digit in the CurrentGrid, program checks the possibilities it eliminates
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            if (sudoku.CurrentGrid[r][c] != 0) {
                int D = sudoku.CurrentGrid[r][c];
                // eliminating possibilities in the exact place
                for (int d = 0; d < 9; d++) if (d != D-1) {
                    if (sudoku.PossibilitiesGrid[r][c][d] == true) {
                        progress = true;
                        sudoku.PossibilitiesGrid[r][c][d] = false;
                    }
                }
                // eliminating possibilities in the row
                for (int c2 = 0; c2 < 9; c2++) if (c2 != c) {
                    if (sudoku.PossibilitiesGrid[r][c2][D-1] == true) {
                        progress = true;
                        sudoku.PossibilitiesGrid[r][c2][D-1] = false;
                    }
                }
                // eliminating possibilities in the column
                for (int r2 = 0; r2 < 9; r2++) if (r2 != r) {
                    if (sudoku.PossibilitiesGrid[r2][c][D-1] == true) {
                        progress = true;
                        sudoku.PossibilitiesGrid[r2][c][D-1] = false;
                    }
                }
                // eliminating possibilities in the box
                int boxr = r/3, boxc = c/3;
                for (int r2 = 0; r2 < 3; r2++)
                    for (int c2 = 0; c2 < 3; c2++)
                        if ( boxr*3 + r2 != r || boxc*3 + c2 != c ) {
                            if (sudoku.PossibilitiesGrid[boxr*3 + r2][boxc*3 + c2][D-1] == true) {
                                    progress = true;
                                    sudoku.PossibilitiesGrid[boxr*3 + r2][boxc*3 + c2][D-1] = false;
                            }
                        }


            }
        }
    }
}

void UpdatePossGrid_Diag(Sudoku9x9 &sudoku, bool &progress){
    /* Function updates the PossGrid of given sudoku using Diagonal sudoku rules only.
    It is only a suplementation for UpdatePossGrid method.
    It does not updates PossGrid using standard classic sudoku rules! */
    for(int c=0;c<9;c++){
        //first main diagonal
        if (sudoku.CurrentGrid[c][c] != 0){
            int D = sudoku.CurrentGrid[c][c];
            for (int c2 = 0; c2 < 9; c2++) if (c2 != c) {
                if (sudoku.PossibilitiesGrid[c2][c2][D-1] == true) {
                    progress = true;
                    sudoku.PossibilitiesGrid[c2][c2][D-1] = false;
                }
            }
        }
        //second main diagonal
        if (sudoku.CurrentGrid[8-c][c] != 0){
            int D = sudoku.CurrentGrid[8-c][c];
            for (int c2 = 0; c2 < 9; c2++) if (c2 != c) {
                if (sudoku.PossibilitiesGrid[8-c2][c2][D-1] == true) {
                    progress = true;
                    sudoku.PossibilitiesGrid[8-c2][c2][D-1] = false;
                }
            }
        }
    }
}

void UpdatePossGrid_NonCon(Sudoku9x9 &sudoku, bool &progress){
    /* Function updates the PossGrid of given sudoku using Non-Consecutive sudoku rules only.
    It is only a suplementation for UpdatePossGrid method.
    It does not updates PossGrid using standard classic sudoku rules! */
    if(sudoku.getType() != 3) return;

    // standard updating
    // When we have a given 5 digit then adjacent cells cannot contain 4 or 6.

    for(int r=0; r<9; r++){
        for(int c=0; c<9; c++){
            if( sudoku.CurrentGrid[r][c] != 0 ){
                int D = sudoku.CurrentGrid[r][c];
                // eliminating possibilities from the place to the top:
                if( r != 0 ){
                    if( D != 1 ){
                        if (sudoku.PossibilitiesGrid[r-1][c][D-2] == true) {
                            progress = true;
                            sudoku.PossibilitiesGrid[r-1][c][D-2] = false;
                        }
                    }
                    if( D != 9 ){
                        if (sudoku.PossibilitiesGrid[r-1][c][D] == true) {
                            progress = true;
                            sudoku.PossibilitiesGrid[r-1][c][D] = false;
                        }
                    }
                }
                // eliminating possibilities from the place to the bottom:
                if( r != 8 ){
                    if( D != 1 ){
                        if (sudoku.PossibilitiesGrid[r+1][c][D-2] == true) {
                            progress = true;
                            sudoku.PossibilitiesGrid[r+1][c][D-2] = false;
                        }
                    }
                    if( D != 9 ){
                        if (sudoku.PossibilitiesGrid[r+1][c][D] == true) {
                            progress = true;
                            sudoku.PossibilitiesGrid[r+1][c][D] = false;
                        }
                    }
                }
                // eliminating possibilities from the place to the left:
                if( c != 0 ){
                    if( D != 1 ){
                        if (sudoku.PossibilitiesGrid[r][c-1][D-2] == true) {
                            progress = true;
                            sudoku.PossibilitiesGrid[r][c-1][D-2] = false;
                        }
                    }
                    if( D != 9 ){
                        if (sudoku.PossibilitiesGrid[r][c-1][D] == true) {
                            progress = true;
                            sudoku.PossibilitiesGrid[r][c-1][D] = false;
                        }
                    }
                }
                // eliminating possibilities from the place to the right:
                if( c != 8 ){
                    if( D != 1 ){
                        if (sudoku.PossibilitiesGrid[r][c+1][D-2] == true) {
                            progress = true;
                            sudoku.PossibilitiesGrid[r][c+1][D-2] = false;
                        }
                    }
                    if( D != 9 ){
                        if (sudoku.PossibilitiesGrid[r][c+1][D] == true) {
                            progress = true;
                            sudoku.PossibilitiesGrid[r][c+1][D] = false;
                        }
                    }
                }

            }
        }
    }

    // Elimination technique nr 1:
    // When in certain place can go only 4,5,6, then in adjacent places cannot be 5.

    for( int r=0; r<8; r++ ){
        for( int c=0; c<9; c++ ){
            if( sudoku.CurrentGrid[r][c] == 0 ){
                for( int d=1; d<8; d++ ){
                    // d - central digit (out of three)
                    int sum = 0;
                    for( int d1=0; d1<d-1; d1++ ){
                        if( sudoku.PossibilitiesGrid[r][c][d1] == true ){ sum++; break; }
                    }
                    for( int d1=d+2; d1<9; d1++ ){
                        if( sudoku.PossibilitiesGrid[r][c][d1] == true ) { sum++; break; }
                    }
                    if( sum == 0 ){
                        if( r != 0 && sudoku.PossibilitiesGrid[r-1][c][d] == true ){
                            sudoku.PossibilitiesGrid[r-1][c][d] = false;
                            progress = true;
                        }
                        if( r != 8 && sudoku.PossibilitiesGrid[r+1][c][d] == true ){
                            sudoku.PossibilitiesGrid[r+1][c][d] = false;
                            progress = true;    
                        }
                        if( c != 0 && sudoku.PossibilitiesGrid[r][c-1][d] == true ){
                            sudoku.PossibilitiesGrid[r][c-1][d] = false;
                            progress = true;    
                        }
                        if( c != 8 && sudoku.PossibilitiesGrid[r][c+1][d] == true ){
                            sudoku.PossibilitiesGrid[r][c+1][d] = false;
                            progress = true;    
                        }
                        break;
                    }
                }
            }
        }
    }
    // Elimination technique nr 2:
    // When in certain place can go only 4,5, then in adjacent places cannot be 4 nor 5.
    for( int r=0; r<8; r++ ){
        for( int c=0; c<9; c++ ){
            if( sudoku.CurrentGrid[r][c] == 0 ){
                for( int d=0; d<8; d++ ){
                    // d - lower digit (out of two)
                    int sum = 0;
                    for( int d1=0; d1<d; d1++ ){
                        if( sudoku.PossibilitiesGrid[r][c][d1] == true ){ sum++; break; }
                    }
                    for( int d1=d+2; d1<9; d1++ ){
                        if( sudoku.PossibilitiesGrid[r][c][d1] == true ) { sum++; break; }
                    }
                    if( sum == 0 ){
                        if( r != 0 ){
                            if( sudoku.PossibilitiesGrid[r-1][c][d] ==  true ){
                                sudoku.PossibilitiesGrid[r-1][c][d] = false;
                                progress = true;
                            }
                            if( sudoku.PossibilitiesGrid[r-1][c][d+1] ==  true ){
                                sudoku.PossibilitiesGrid[r-1][c][d+1] = false;
                                progress = true;
                            }
                        }
                        if( r != 8 ){
                            if( sudoku.PossibilitiesGrid[r+1][c][d] ==  true ){
                                sudoku.PossibilitiesGrid[r+1][c][d] = false;
                                progress = true;
                            }
                            if( sudoku.PossibilitiesGrid[r+1][c][d+1] ==  true ){
                                sudoku.PossibilitiesGrid[r+1][c][d+1] = false;
                                progress = true;
                            }
                        }
                        if( c != 0 ){
                            if( sudoku.PossibilitiesGrid[r][c-1][d] ==  true ){
                                sudoku.PossibilitiesGrid[r][c-1][d] = false;
                                progress = true;
                            }
                            if( sudoku.PossibilitiesGrid[r][c-1][d+1] ==  true ){
                                sudoku.PossibilitiesGrid[r][c-1][d+1] = false;
                                progress = true;
                            }
                        }
                        if( c != 8 ){
                            if( sudoku.PossibilitiesGrid[r][c+1][d] ==  true ){
                                sudoku.PossibilitiesGrid[r][c+1][d] = false;
                                progress = true;
                            }
                            if( sudoku.PossibilitiesGrid[r][c+1][d+1] ==  true ){
                                sudoku.PossibilitiesGrid[r][c+1][d+1] = false;
                                progress = true;
                            }
                        }
                        break;
                    }
                }
            }
        }
    }
    // Elimination technique nr 3:
    // When in certain row/col 5 can fit only into three consecutive places,
    // 4 and 6 cannot be in the middle.
    for( int r=0; r<9; r++ ){
        for( int d=0; d<9; d++){
            for( int c=1; c<8; c++ ){
                // c - central column (out of three)
                if( sudoku.CurrentGrid[r][c] == 0 ){
                    int sum = 0;
                    for( int c1=0; c1<c-1; c1++ ){
                        if( sudoku.PossibilitiesGrid[r][c1][d] == true ){ sum++; break; }
                    }
                    for( int c1=c+2; c1<9; c1++ ){
                        if( sudoku.PossibilitiesGrid[r][c1][d] == true ) { sum++; break; }
                    }
                    if( sum == 0 ){
                        if( d != 0 && sudoku.PossibilitiesGrid[r][c][d-1] == true ){
                            sudoku.PossibilitiesGrid[r][c][d-1] = false;
                            progress = true;    
                        }
                        if( d != 8 && sudoku.PossibilitiesGrid[r][c][d+1] == true ){
                            sudoku.PossibilitiesGrid[r][c][d+1] = false;
                            progress = true;    
                        }
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
                if( sudoku.CurrentGrid[r][c] == 0 ){
                    int sum = 0;
                    for( int r1=0; r1<r-1; r1++ ){
                        if( sudoku.PossibilitiesGrid[r1][c][d] == true ){ sum++; break; }
                    }
                    for( int r1=r+2; r1<9; r1++ ){
                        if( sudoku.PossibilitiesGrid[r1][c][d] == true ) { sum++; break; }
                    }
                    if( sum == 0 ){
                        if( d != 0 && sudoku.PossibilitiesGrid[r][c][d-1] == true ){
                            sudoku.PossibilitiesGrid[r][c][d-1] = false;
                            progress = true;    
                        }
                        if( d != 8 && sudoku.PossibilitiesGrid[r][c][d+1] == true ){
                            sudoku.PossibilitiesGrid[r][c][d+1] = false;
                            progress = true;    
                        }
                        break;
                    }
                }
            }
        }
    }
    // Elimination technique nr 4:
    // when in certain row/col 5 can fit only into two consecutive places,
    // 4 and 6 cannot be in any of them.
    for( int r=0; r<9; r++ ){
        for( int d=0; d<9; d++){
            for( int c=0; c<8; c++ ){
                // c - left column (out of two)
                if( sudoku.CurrentGrid[r][c] == 0 ){
                    int sum = 0;
                    for( int c1=0; c1<c; c1++ ){
                        if( sudoku.PossibilitiesGrid[r][c1][d] == true ){ sum++; break; }
                    }
                    for( int c1=c+2; c1<9; c1++ ){
                        if( sudoku.PossibilitiesGrid[r][c1][d] == true ) { sum++; break; }
                    }
                    if( sum == 0 ){
                        if( d != 0 ){
                            if( sudoku.PossibilitiesGrid[r][c][d-1] == true ){
                                sudoku.PossibilitiesGrid[r][c][d-1] = false;
                                progress = true;
                            }
                            if( sudoku.PossibilitiesGrid[r][c+1][d-1] == true ){
                                sudoku.PossibilitiesGrid[r][c+1][d-1] = false;
                                progress = true;
                            }
                        }
                        if( d != 8 ){
                            if( sudoku.PossibilitiesGrid[r][c][d+1] == true ){
                                sudoku.PossibilitiesGrid[r][c][d+1] = false;
                                progress = true;
                            }
                            if( sudoku.PossibilitiesGrid[r][c+1][d+1] == true ){
                                sudoku.PossibilitiesGrid[r][c+1][d+1] = false;
                                progress = true;
                            }
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
                if( sudoku.CurrentGrid[r][c] == 0 ){
                    int sum = 0;
                    for( int r1=0; r1<r; r1++ ){
                        if( sudoku.PossibilitiesGrid[r1][c][d] == true ){ sum++; break; }
                    }
                    for( int r1=r+2; r1<9; r1++ ){
                        if( sudoku.PossibilitiesGrid[r1][c][d] == true ) { sum++; break; }
                    }
                    if( sum == 0 ){
                        if( d != 0 ){
                            if( sudoku.PossibilitiesGrid[r][c][d-1] == true ){
                                sudoku.PossibilitiesGrid[r][c][d-1] = false;
                                progress = true;
                            }
                            if( sudoku.PossibilitiesGrid[r+1][c][d-1] == true ){
                                sudoku.PossibilitiesGrid[r+1][c][d-1] = false;
                                progress = true;
                            }
                        }
                        if( d != 8 ){
                            if( sudoku.PossibilitiesGrid[r][c][d+1] == true ){
                                sudoku.PossibilitiesGrid[r][c][d+1] = false;
                                progress = true;
                            }
                            if( sudoku.PossibilitiesGrid[r+1][c][d+1] == true ){
                                sudoku.PossibilitiesGrid[r+1][c][d+1] = false;
                                progress = true;
                            }
                        }
                        break;
                    }
                }
            }
        }
    }
    // Elimination technique nr 5
    // When in certain box 5 can fit only into a cross or T-shaped region,
    // then 4 and 6 cannot fit into the middle cell of that cross/T-shape
    for( int boxr=0; boxr<3; boxr++ ){
        for( int boxc=0; boxc<3; boxc++ ){
            for( int d=0; d<9; d++ ){
                // center cross shape
                if( sudoku.PossibilitiesGrid[boxr*3][boxc*3][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+2][boxc*3][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3][boxc*3+2][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+2][d] == false){
                    if( d != 0 && sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+1][d-1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+1][d-1] = false;
                        progress = true;
                    }
                    if( d != 8 && sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+1][d+1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+1][d+1] = false;
                        progress = true;
                    }
                }
                // left T-shape
                if( sudoku.PossibilitiesGrid[boxr*3][boxc*3+1][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3][boxc*3+2][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+2][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+1][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+2][d] == false){
                    if( d != 0 && sudoku.PossibilitiesGrid[boxr*3+1][boxc*3][d-1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3+1][boxc*3][d-1] = false;
                        progress = true;
                    }
                    if( d != 8 && sudoku.PossibilitiesGrid[boxr*3+1][boxc*3][d+1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3+1][boxc*3][d+1] = false;
                        progress = true;
                    }
                }
                // right T-shape
                if( sudoku.PossibilitiesGrid[boxr*3][boxc*3][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3][boxc*3+1][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+1][boxc*3][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+2][boxc*3][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+1][d] == false){
                    if( d != 0 && sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+2][d-1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+2][d-1] = false;
                        progress = true;
                    }
                    if( d != 8 && sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+2][d+1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+2][d+1] = false;
                        progress = true;
                    }
                }
                // upper T-shape
                if( sudoku.PossibilitiesGrid[boxr*3+1][boxc*3][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+2][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+2][boxc*3][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+1][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+2][d] == false){
                    if( d != 0 && sudoku.PossibilitiesGrid[boxr*3][boxc*3+1][d-1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3][boxc*3+1][d-1] = false;
                        progress = true;
                    }
                    if( d != 8 && sudoku.PossibilitiesGrid[boxr*3][boxc*3+1][d+1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3][boxc*3+1][d+1] = false;
                        progress = true;
                    }
                }
                // lower T-shape
                if( sudoku.PossibilitiesGrid[boxr*3][boxc*3][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3][boxc*3+1][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3][boxc*3+2][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+1][boxc*3][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+2][d] == false){
                    if( d != 0 && sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+1][d-1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+1][d-1] = false;
                        progress = true;
                    }
                    if( d != 8 && sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+1][d+1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+1][d+1] = false;
                        progress = true;
                    }
                }
            }
        }
    }
    // Elimination technique nr 6 
    // When in certain box 5 can fit only into two cells adjacent by corners.
    // then 4 and 6 cannot fit into cells that touch theese two cells by side
    for( int boxr=0; boxr<3; boxr++ ){
        for( int boxc=0; boxc<3; boxc++ ){
            for( int d=0; d<9; d++ ){
                // oxx
                // xox
                // xxx
                if( sudoku.PossibilitiesGrid[boxr*3][boxc*3][d] == true &&
                    sudoku.PossibilitiesGrid[boxr*3][boxc*3+1][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3][boxc*3+2][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+1][boxc*3][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+1][d] == true &&
                    sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+2][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+2][boxc*3][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+1][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+2][d] == false
                ){
                    if( d != 0 && sudoku.PossibilitiesGrid[boxr*3][boxc*3+1][d-1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3][boxc*3+1][d-1] = false;
                        progress = true;
                    }
                    if( d != 8 && sudoku.PossibilitiesGrid[boxr*3][boxc*3+1][d+1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3][boxc*3+1][d+1] = false;
                        progress = true;
                    }
                    if( d != 0 && sudoku.PossibilitiesGrid[boxr*3+1][boxc*3][d-1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3+1][boxc*3][d-1] = false;
                        progress = true;
                    }
                    if( d != 8 && sudoku.PossibilitiesGrid[boxr*3+1][boxc*3][d+1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3+1][boxc*3][d+1] = false;
                        progress = true;
                    }
                }
                // xox
                // oxx
                // xxx
                if( sudoku.PossibilitiesGrid[boxr*3][boxc*3][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3][boxc*3+1][d] == true &&
                    sudoku.PossibilitiesGrid[boxr*3][boxc*3+2][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+1][boxc*3][d] == true &&
                    sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+1][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+2][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+2][boxc*3][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+1][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+2][d] == false
                ){
                    if( d != 0 && sudoku.PossibilitiesGrid[boxr*3][boxc*3][d-1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3][boxc*3][d-1] = false;
                        progress = true;
                    }
                    if( d != 8 && sudoku.PossibilitiesGrid[boxr*3][boxc*3][d+1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3][boxc*3][d+1] = false;
                        progress = true;
                    }
                    if( d != 0 && sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+1][d-1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+1][d-1] = false;
                        progress = true;
                    }
                    if( d != 8 && sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+1][d+1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+1][d+1] = false;
                        progress = true;
                    }
                }
                // xox
                // xxo
                // xxx
                if( sudoku.PossibilitiesGrid[boxr*3][boxc*3][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3][boxc*3+1][d] == true &&
                    sudoku.PossibilitiesGrid[boxr*3][boxc*3+2][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+1][boxc*3][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+1][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+2][d] == true &&
                    sudoku.PossibilitiesGrid[boxr*3+2][boxc*3][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+1][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+2][d] == false
                ){
                    if( d != 0 && sudoku.PossibilitiesGrid[boxr*3][boxc*3+2][d-1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3][boxc*3+2][d-1] = false;
                        progress = true;
                    }
                    if( d != 8 && sudoku.PossibilitiesGrid[boxr*3][boxc*3+2][d+1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3][boxc*3+2][d+1] = false;
                        progress = true;
                    }
                    if( d != 0 && sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+1][d-1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+1][d-1] = false;
                        progress = true;
                    }
                    if( d != 8 && sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+1][d+1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+1][d+1] = false;
                        progress = true;
                    }
                }
                // xxo
                // xox
                // xxx
                if( sudoku.PossibilitiesGrid[boxr*3][boxc*3][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3][boxc*3+1][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3][boxc*3+2][d] == true &&
                    sudoku.PossibilitiesGrid[boxr*3+1][boxc*3][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+1][d] == true &&
                    sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+2][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+2][boxc*3][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+1][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+2][d] == false
                ){
                    if( d != 0 && sudoku.PossibilitiesGrid[boxr*3][boxc*3+1][d-1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3][boxc*3+1][d-1] = false;
                        progress = true;
                    }
                    if( d != 8 && sudoku.PossibilitiesGrid[boxr*3][boxc*3+1][d+1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3][boxc*3+1][d+1] = false;
                        progress = true;
                    }
                    if( d != 0 && sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+2][d-1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+2][d-1] = false;
                        progress = true;
                    }
                    if( d != 8 && sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+2][d+1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+2][d+1] = false;
                        progress = true;
                    }
                }
                // xxx
                // oxx
                // xox
                if( sudoku.PossibilitiesGrid[boxr*3][boxc*3][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3][boxc*3+1][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3][boxc*3+2][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+1][boxc*3][d] == true &&
                    sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+1][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+2][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+2][boxc*3][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+1][d] == true &&
                    sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+2][d] == false
                ){
                    if( d != 0 && sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+1][d-1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+1][d-1] = false;
                        progress = true;
                    }
                    if( d != 8 && sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+1][d+1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+1][d+1] = false;
                        progress = true;
                    }
                    if( d != 0 && sudoku.PossibilitiesGrid[boxr*3+2][boxc*3][d-1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3+2][boxc*3][d-1] = false;
                        progress = true;
                    }
                    if( d != 8 && sudoku.PossibilitiesGrid[boxr*3+2][boxc*3][d+1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3+2][boxc*3][d+1] = false;
                        progress = true;
                    }
                }
                // xxx
                // xox
                // oxx
                if( sudoku.PossibilitiesGrid[boxr*3][boxc*3][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3][boxc*3+1][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3][boxc*3+2][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+1][boxc*3][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+1][d] == true &&
                    sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+2][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+2][boxc*3][d] == true &&
                    sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+1][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+2][d] == false
                ){
                    if( d != 0 && sudoku.PossibilitiesGrid[boxr*3+1][boxc*3][d-1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3+1][boxc*3][d-1] = false;
                        progress = true;
                    }
                    if( d != 8 && sudoku.PossibilitiesGrid[boxr*3+1][boxc*3][d+1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3+1][boxc*3][d+1] = false;
                        progress = true;
                    }
                    if( d != 0 && sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+1][d-1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+1][d-1] = false;
                        progress = true;
                    }
                    if( d != 8 && sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+1][d+1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+1][d+1] = false;
                        progress = true;
                    }
                }
                // xxx
                // xox
                // xxo
                if( sudoku.PossibilitiesGrid[boxr*3][boxc*3][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3][boxc*3+1][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3][boxc*3+2][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+1][boxc*3][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+1][d] == true &&
                    sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+2][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+2][boxc*3][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+1][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+2][d] == true
                ){
                    if( d != 0 && sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+2][d-1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+2][d-1] = false;
                        progress = true;
                    }
                    if( d != 8 && sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+2][d+1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+2][d+1] = false;
                        progress = true;
                    }
                    if( d != 0 && sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+1][d-1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+1][d-1] = false;
                        progress = true;
                    }
                    if( d != 8 && sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+1][d+1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+1][d+1] = false;
                        progress = true;
                    }
                }
                // xxx
                // xxo
                // xox
                if( sudoku.PossibilitiesGrid[boxr*3][boxc*3][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3][boxc*3+1][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3][boxc*3+2][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+1][boxc*3][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+1][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+2][d] == true &&
                    sudoku.PossibilitiesGrid[boxr*3+2][boxc*3][d] == false &&
                    sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+1][d] == true &&
                    sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+2][d] == false
                ){
                    if( d != 0 && sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+1][d-1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+1][d-1] = false;
                        progress = true;
                    }
                    if( d != 8 && sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+1][d+1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3+1][boxc*3+1][d+1] = false;
                        progress = true;
                    }
                    if( d != 0 && sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+2][d-1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+2][d-1] = false;
                        progress = true;
                    }
                    if( d != 8 && sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+2][d+1] == true ){
                        sudoku.PossibilitiesGrid[boxr*3+2][boxc*3+2][d+1] = false;
                        progress = true;
                    }
                }
                
            }
        }
    }
}

void UpdatePossGrid_AntiKnight(Sudoku9x9 &sudoku, bool &progress){
    /* Function updates the PossGrid of given sudoku using Anti-Knight sudoku rules only.
    It is only a suplementation for UpdatePossGrid method.
    It does not updates PossGrid using standard classic sudoku rules! */
    for( int r=0; r<9; r++ ){
        for( int c=0; c<9; c++ ){
            if( sudoku.CurrentGrid[r][c] != 0 ){
                int d = sudoku.CurrentGrid[r][c]-1;
                if( r>0 && c>1 && sudoku.PossibilitiesGrid[r-1][c-2][d] == true ){
                    sudoku.PossibilitiesGrid[r-1][c-2][d] = false;
                    progress = true;
                }
                if( r>1 && c>0 && sudoku.PossibilitiesGrid[r-2][c-1][d] == true ){
                    sudoku.PossibilitiesGrid[r-2][c-1][d] = false;
                    progress = true;
                }
                if( r>1 && c<8 && sudoku.PossibilitiesGrid[r-2][c+1][d] == true ){
                    sudoku.PossibilitiesGrid[r-2][c+1][d] = false;
                    progress = true;
                }
                if( r>0 && c<7 && sudoku.PossibilitiesGrid[r-1][c+2][d] == true ){
                    sudoku.PossibilitiesGrid[r-1][c+2][d] = false;
                    progress = true;
                }
                if( r<8 && c<7 && sudoku.PossibilitiesGrid[r+1][c+2][d] == true ){
                    sudoku.PossibilitiesGrid[r+1][c+2][d] = false;
                    progress = true;
                }
                if( r<7 && c<8 && sudoku.PossibilitiesGrid[r+2][c+1][d] == true ){
                    sudoku.PossibilitiesGrid[r+2][c+1][d] = false;
                    progress = true;
                }
                if( r<7 && c>0 && sudoku.PossibilitiesGrid[r+2][c-1][d] == true ){
                    sudoku.PossibilitiesGrid[r+2][c-1][d] = false;
                    progress = true;
                }
                if( r<8 && c>1 && sudoku.PossibilitiesGrid[r+1][c-2][d] == true ){
                    sudoku.PossibilitiesGrid[r+1][c-2][d] = false;
                    progress = true;
                }
            }
        }
    }
}

void UpdateCurrentGrid(Sudoku9x9 &sudoku, bool &progress){
    // actualization of the CurrentGrid
    // for every digit checking if there is only one possibilitie
    // in a certain place, row, column or box
    for (int d = 0; d < 9; d++) {
        // checking if there is only one possibilitie in a place
        for (int r = 0; r < 9; r++) {
            for (int c = 0; c < 9; c++) {
                if( sudoku.CurrentGrid[r][c] == 0 && sudoku.N_Possibilities(r,c) == 1 ){
                    for (int d = 0; d < 9; d++){
                        if( sudoku.PossibilitiesGrid[r][c][d] == true ){
                            progress = true;
                            sudoku.CurrentGrid[r][c] = d+1;
                            break;
                        }
                    }
                }
            }
        }
        // checking if there is only one possibilitie in a row
        for (int r = 0; r < 9; r++) {
            if( sudoku.N_Possibilities(d,r,0) == 1 ){
                for (int c = 0; c < 9; c++){
                    if( sudoku.PossibilitiesGrid[r][c][d] == true ){
                        if( sudoku.CurrentGrid[r][c] == 0 ){
                            progress = true;
                            sudoku.CurrentGrid[r][c] = d+1;
                        }
                        break;
                    }
                }
            }
        }
        // checking if there is only one possibilitie in a column
        for (int c = 0; c < 9; c++) {
            if( sudoku.N_Possibilities(d,c,1) == 1 ){
                for (int r = 0; r < 9; r++){
                    if( sudoku.PossibilitiesGrid[r][c][d] == true ){
                        if( sudoku.CurrentGrid[r][c] == 0 ){
                            progress = true;
                            sudoku.CurrentGrid[r][c] = d+1;
                        }
                        break;
                    }
                }
            }
        }
        // checking if there is only one possibilitie in a box
        for(int boxr=0; boxr<3; boxr++){
            for(int boxc=0; boxc<3; boxc++){
                int sum = 0;
                for(int r=0; r<3; r++)
                    for(int c=0; c<3; c++)
                        sum += sudoku.PossibilitiesGrid[boxr*3 + r][boxc*3 + c][d];
                if( sum == 1 ){
                    for(int r=0; r<3; r++)
                        for(int c=0; c<3; c++)
                            if( sudoku.PossibilitiesGrid[boxr*3 + r][boxc*3 + c][d] == true ){
                                if( sudoku.CurrentGrid[boxr*3 + r][boxc*3 + c] == 0 ){
                                    progress = true;
                                    sudoku.CurrentGrid[boxr*3 + r][boxc*3 + c] = d+1;
                                }
                                break;
                            }
                }
            }
        }
    }
}

Sudoku9x9 TryToSolveEasy(Sudoku9x9 sudoku){
    /* Function tries to solve sudoku using only hidden singles
    and rows/cols/boxes where certain digit can fit into only one place. */

    if( IsContradictory(sudoku) ){
        sudoku.setStatus(4); // contradictory
        return sudoku;
    }

    // for classic sudoku only
    if( sudoku.getType()==1 && sudoku.N_Current() < 17){
        sudoku.setStatus(3); // ambiguous
        return sudoku;
    }

    bool progress = true;
    while(progress){
        progress = false;

        // actualization of PossibilitiesGrid
        UpdatePossGrid_Classic(sudoku, progress);

        // only for Diagonal sudoku
        if( sudoku.getType() == 2 ){
            UpdatePossGrid_Diag(sudoku,progress);
        }

        // Only for Non-Consecutive sudoku
        if( sudoku.getType() == 3 ){
            UpdatePossGrid_NonCon(sudoku,progress);
        }

        // Only for Anti-Knight sudoku
        if( sudoku.getType() == 4 ){
            UpdatePossGrid_AntiKnight(sudoku,progress);
        }

        // actualization of CurrentGrid
        UpdateCurrentGrid(sudoku, progress);
        
    }

    return sudoku;
}

Sudoku9x9 TryToSolve(Sudoku9x9 sudoku){
    /* Function tries to solve sudoku using TryToSolveEasy()
    and restricting certain rows/cols techniques. */

    if( IsContradictory(sudoku) ){
        sudoku.setStatus(4); // contradictory
        return sudoku;
    }

    // for classic sudoku only
    if( sudoku.getType()==1 && sudoku.N_Current() < 17){
        sudoku.setStatus(3); // ambiguous
        return sudoku;
    }

    bool progress = true;
    while(progress){
        progress = false;

        // actualization of PossibilitiesGrid
        UpdatePossGrid_Classic(sudoku, progress);

        // only for Diagonal sudoku
        if( sudoku.getType() == 2 ){
            UpdatePossGrid_Diag(sudoku,progress);
        }

        // Only for Non-Consecutive sudoku
        if( sudoku.getType() == 3 ){
            UpdatePossGrid_NonCon(sudoku,progress);
        }

        // Only for Anti-Knight sudoku
        if( sudoku.getType() == 4 ){
            UpdatePossGrid_AntiKnight(sudoku,progress);
        }
        

        // Basic elimination technique nr 1
        // When in certain box digit can fit only into cells in one row/col, then the possibilities
        // from the rest of this row/col for this digit can be eliminated. 

        // horizontally
        for (int d = 0; d < 9; d++) for (int boxr = 0; boxr < 3; boxr++) {
            //1
            if (TotalPoss(sudoku.PossibilitiesGrid, d, 3*boxr, 0, 3*boxr + 1, 2) == 0)
                for (int c = 3; c<9; c++)
                    if (sudoku.PossibilitiesGrid[3*boxr + 2][c][d] == true) {
                        progress = true;
                        sudoku.PossibilitiesGrid[3*boxr + 2][c][d] = false;
                    }
            if (TotalPoss(sudoku.PossibilitiesGrid, d, 3*boxr + 1, 0, 3*boxr + 2, 2) == 0)
                for (int c = 3; c<9; c++)
                    if (sudoku.PossibilitiesGrid[3*boxr][c][d] == true) {
                        progress = true;
                        sudoku.PossibilitiesGrid[3*boxr][c][d] = false;
                    }
            if (TotalPoss(sudoku.PossibilitiesGrid, d, 3*boxr, 0, 3*boxr, 2)
                + TotalPoss(sudoku.PossibilitiesGrid, d, 3*boxr+2, 0, 3*boxr+2, 2) == 0)
                for (int c = 3; c<9; c++)
                    if (sudoku.PossibilitiesGrid[3*boxr+1][c][d] == true) {
                        progress = true;
                        sudoku.PossibilitiesGrid[3*boxr+1][c][d] = false;
                    }
            //2
            if (TotalPoss(sudoku.PossibilitiesGrid, d, 3*boxr, 3, 3*boxr + 1, 5) == 0){
                for (int c = 0; c<3; c++)
                    if (sudoku.PossibilitiesGrid[3*boxr + 2][c][d] == true) {
                        progress = true;
                        sudoku.PossibilitiesGrid[3*boxr + 2][c][d] = false;
                    }
                for (int c = 6; c<9; c++)
                    if (sudoku.PossibilitiesGrid[3*boxr + 2][c][d] == true) {
                        progress = true;
                        sudoku.PossibilitiesGrid[3*boxr + 2][c][d] = false;
                    }
            }
            if (TotalPoss(sudoku.PossibilitiesGrid, d, 3*boxr + 1, 3, 3*boxr + 2, 5) == 0){
                for (int c = 0; c<3; c++)
                    if (sudoku.PossibilitiesGrid[3*boxr][c][d] == true) {
                        progress = true;
                        sudoku.PossibilitiesGrid[3*boxr][c][d] = false;
                    }
                for (int c = 6; c<9; c++)
                    if (sudoku.PossibilitiesGrid[3*boxr][c][d] == true) {
                        progress = true;
                        sudoku.PossibilitiesGrid[3*boxr][c][d] = false;
                    }
            }
            if (TotalPoss(sudoku.PossibilitiesGrid, d, 3*boxr, 3, 3*boxr, 5)
                + TotalPoss(sudoku.PossibilitiesGrid, d, 3*boxr+2, 3, 3*boxr+2, 5) == 0){
                for (int c = 0; c<3; c++)
                    if (sudoku.PossibilitiesGrid[3*boxr+1][c][d] == true) {
                        progress = true;
                        sudoku.PossibilitiesGrid[3*boxr+1][c][d] = false;
                    }
                for (int c = 6; c<9; c++)
                    if (sudoku.PossibilitiesGrid[3*boxr+1][c][d] == true) {
                        progress = true;
                        sudoku.PossibilitiesGrid[3*boxr+1][c][d] = false;
                    }
            }
            //3
            if (TotalPoss(sudoku.PossibilitiesGrid, d, 3*boxr, 6, 3*boxr + 1, 8) == 0)
                for (int c = 0; c<6; c++)
                    if (sudoku.PossibilitiesGrid[3*boxr + 2][c][d] == true) {
                        progress = true;
                        sudoku.PossibilitiesGrid[3*boxr + 2][c][d] = false;
                    }
            if (TotalPoss(sudoku.PossibilitiesGrid, d, 3*boxr + 1, 6, 3*boxr + 2, 8) == 0)
                for (int c = 0; c<6; c++)
                    if (sudoku.PossibilitiesGrid[3*boxr][c][d] == true) {
                        progress = true;
                        sudoku.PossibilitiesGrid[3*boxr][c][d] = false;
                    }
            if (TotalPoss(sudoku.PossibilitiesGrid, d, 3*boxr, 6, 3*boxr, 8)
                + TotalPoss(sudoku.PossibilitiesGrid, d, 3*boxr+2, 6, 3*boxr+2, 8) == 0)
                for (int c = 0; c<6; c++)
                    if (sudoku.PossibilitiesGrid[3*boxr+1][c][d] == true) {
                        progress = true;
                        sudoku.PossibilitiesGrid[3*boxr+1][c][d] = false;
                    }

        }
        // vertically
        for (int d = 0; d < 9; d++) for (int boxc = 0; boxc < 3; boxc++) {
            //1
            if (TotalPoss(sudoku.PossibilitiesGrid, d, 0, 3*boxc, 2, 3*boxc + 1) == 0)
                for (int r = 3; r<9; r++)
                    if (sudoku.PossibilitiesGrid[r][3*boxc+2][d] == true) {
                        progress = true;
                        sudoku.PossibilitiesGrid[r][3*boxc+2][d] = false;
                    }
            if (TotalPoss(sudoku.PossibilitiesGrid, d, 0, 3*boxc+1, 2, 3*boxc+2) == 0)
                for (int r = 3; r<9; r++)
                    if (sudoku.PossibilitiesGrid[r][3*boxc][d] == true) {
                        progress = true;
                        sudoku.PossibilitiesGrid[r][3*boxc][d] = false;
                    }
            if (TotalPoss(sudoku.PossibilitiesGrid, d, 0, 3*boxc, 2, 3*boxc)
                + TotalPoss(sudoku.PossibilitiesGrid, d, 0, 3*boxc+2, 2, 3*boxc+2) == 0)
                for (int r = 3; r<9; r++)
                    if (sudoku.PossibilitiesGrid[r][3*boxc+1][d] == true) {
                        progress = true;
                        sudoku.PossibilitiesGrid[r][3*boxc+1][d] = false;
                    }
            //2
            if (TotalPoss(sudoku.PossibilitiesGrid, d, 3, 3*boxc, 5, 3*boxc+1) == 0){
                for (int r = 0; r<3; r++)
                    if (sudoku.PossibilitiesGrid[r][3*boxc+2][d] == true) {
                        progress = true;
                        sudoku.PossibilitiesGrid[r][3*boxc+2][d] = false;
                    }
                for (int r = 6; r<9; r++)
                    if (sudoku.PossibilitiesGrid[r][3*boxc+2][d] == true) {
                        progress = true;
                        sudoku.PossibilitiesGrid[r][3*boxc+2][d] = false;
                    }
            }
            if (TotalPoss(sudoku.PossibilitiesGrid, d, 3, 3*boxc+1, 5, 3*boxc+2) == 0){
                for (int r = 0; r<3; r++)
                    if (sudoku.PossibilitiesGrid[r][3*boxc][d] == true) {
                        progress = true;
                        sudoku.PossibilitiesGrid[r][3*boxc][d] = false;
                    }
                for (int r = 6; r<9; r++)
                    if (sudoku.PossibilitiesGrid[r][3*boxc][d] == true) {
                        progress = true;
                        sudoku.PossibilitiesGrid[r][3*boxc][d] = false;
                    }
            }
            if (TotalPoss(sudoku.PossibilitiesGrid, d, 3, 3*boxc, 5, 3*boxc)
                + TotalPoss(sudoku.PossibilitiesGrid, d, 3, 3*boxc+2, 5, 3*boxc+2) == 0){
                for (int r = 0; r<3; r++)
                    if (sudoku.PossibilitiesGrid[r][3*boxc+1][d] == true) {
                        progress = true;
                        sudoku.PossibilitiesGrid[r][3*boxc+1][d] = false;
                    }
                for (int r = 6; r<9; r++)
                    if (sudoku.PossibilitiesGrid[r][3*boxc+1][d] == true) {
                        progress = true;
                        sudoku.PossibilitiesGrid[r][3*boxc+1][d] = false;
                    }
            }
            //3
            if (TotalPoss(sudoku.PossibilitiesGrid, d, 6, 3*boxc, 8, 3*boxc+1) == 0)
                for (int r = 0; r<6; r++)
                    if (sudoku.PossibilitiesGrid[r][3*boxc+2][d] == true) {
                        progress = true;
                        sudoku.PossibilitiesGrid[r][3*boxc+2][d] = false;
                    }
            if (TotalPoss(sudoku.PossibilitiesGrid, d, 6, 3*boxc+1, 8, 3*boxc+2) == 0)
                for (int r = 0; r<6; r++)
                    if (sudoku.PossibilitiesGrid[r][3*boxc][d] == true) {
                        progress = true;
                        sudoku.PossibilitiesGrid[r][3*boxc][d] = false;
                    }
            if (TotalPoss(sudoku.PossibilitiesGrid, d, 6, 3*boxc, 8, 3*boxc)
                + TotalPoss(sudoku.PossibilitiesGrid, d, 6, 3*boxc+2, 8, 3*boxc+2) == 0)
                for (int r = 0; r<6; r++)
                    if (sudoku.PossibilitiesGrid[r][3*boxc+1][d] == true) {
                        progress = true;
                        sudoku.PossibilitiesGrid[r][3*boxc+1][d] = false;
                    }

        }

        // actualization of the CurrentGrid
        UpdateCurrentGrid(sudoku, progress);
    }


    return sudoku;
}

Sudoku9x9 Solve(Sudoku9x9 sudoku){
    /* Main solving function.
    Solves sudoku, estimates difficulty and gives appropriate status. */

    if( IsContradictory(sudoku) ){
        sudoku.setStatus(4); // contradictory
        return sudoku;
    }

    // for classic sudoku only
    if( sudoku.getType()==1 && sudoku.N_Current() < 17){
        sudoku.setStatus(3); // ambiguous
        return sudoku;
    }

    sudoku = TryToSolveEasy(sudoku);
    if( IsContradictory(sudoku) ){
        sudoku.setStatus(4); 		// contradictory
        return sudoku;
    }
    if( IsFilled(sudoku) ){
        sudoku.setStatus(1); 		// solved
        sudoku.setDifficulty(1); 	// Very Easy
        return sudoku;
    }

    sudoku = TryToSolve(sudoku);
    if( IsContradictory(sudoku) ){
        sudoku.setStatus(4); 		// contradictory
        return sudoku;
    }
    if( IsFilled(sudoku) ){
        sudoku.setStatus(1); 		// solved
        sudoku.setDifficulty(2); 	// Easy
        return sudoku;
    }


    bool progress = true;
    int DifficultyLevel = 2;

    while(progress){
        progress = false;

        // program is looking for rows/cols in which this digit can fit only into exactly 2 places
        if ( !IsFilled(sudoku) ) {
            DifficultyLevel++;
            // for every digit
            for (int d = 0; d < 9; d++) {
                // rows in which this digit can fit only into exactly 2 places:
                if (progress == true) break;
                int c1 = 0, c2 = 0;
                for (int r = 0; r < 9; r++) {
                    if (progress == true) break;
                    if ( TotalPoss(sudoku.PossibilitiesGrid, d, r, 0, r, 8) == 2 ) {
                        // c1 and c2 are columns we are looking for
                        for (int c = 0; c < 8; c++) {
                            if ( sudoku.PossibilitiesGrid[r][c][d] == true ) {
                                c1 = c;
                                break;
                            }
                        }
                        for (int c = c1 + 1; c < 9; c++) {
                            if ( sudoku.PossibilitiesGrid[r][c][d] == true ) {
                                c2 = c;
                                break;
                            }
                        }

                        // checking
                        Sudoku9x9 S1(sudoku), S2(sudoku);
                        S1.CurrentGrid[r][c1] = d+1;
                        S2.CurrentGrid[r][c2] = d+1;
                        S1 = TryToSolve(S1);
                        S2 = TryToSolve(S2);

                        bool contraS1 = IsContradictory(S1), contraS2 = IsContradictory(S2);
                        bool filledS1 = IsFilled(S1), filledS2 = IsFilled(S2);

                        if( contraS1 && contraS2 ){
                            sudoku.setStatus(4); 	// contradictory
                            return sudoku;
                        }
                        if( filledS1 && filledS2 && !contraS1 && !contraS2 ){
                            sudoku.setStatus(3);	// ambiguous
                            return sudoku;
                        }
                        if ( contraS1 ) {
                            sudoku = S2;
                            if ( filledS2 ){
                                sudoku.setStatus(1); 		// solved
                                sudoku.setDifficulty(DifficultyLevel);
                                return sudoku;
                            }
                            progress = true;
                            break;
                        }
                        if ( contraS2 ) {
                            sudoku = S1;
                            if ( filledS1 ){
                                sudoku.setStatus(1); 		// solved
                                sudoku.setDifficulty(DifficultyLevel);
                                return sudoku;
                            }
                            progress = true;
                            break;
                        }
                    }
                }
                // columns in which this digit can fit only into exactly 2 places:
                if (progress == true) break;
                int r1 = 0, r2 = 0;
                for (int c = 0; c < 9; c++) {
                    if (progress == true) break;
                    if ( TotalPoss(sudoku.PossibilitiesGrid, d, 0, c, 8, c) == 2 ) {
                        // r1 and r2 are rows we are looking for
                        for (int r = 0; r < 8; r++) {
                            if ( sudoku.PossibilitiesGrid[r][c][d] == true) {
                                r1 = r;
                                break;
                            }
                        }
                        for (int r = r1 + 1; r < 9; r++) {
                            if ( sudoku.PossibilitiesGrid[r][c][d] == true ) {
                                r2 = r;
                                break;
                            }
                        }

                        // checking
                        Sudoku9x9 S1(sudoku), S2(sudoku);
                        S1.CurrentGrid[r1][c] = d+1;
                        S2.CurrentGrid[r2][c] = d+1;
                        S1 = TryToSolve(S1);
                        S2 = TryToSolve(S2);

                        bool contraS1 = IsContradictory(S1), contraS2 = IsContradictory(S2);
                        bool filledS1 = IsFilled(S1), filledS2 = IsFilled(S2);

                        if( contraS1 && contraS2 ){
                            sudoku.setStatus(4); 	// contradictory
                            return sudoku;
                        }
                        if( filledS1 && filledS2 && !contraS1 && !contraS2 ){
                            sudoku.setStatus(3);	// ambiguous
                            return sudoku;
                        }
                        if ( contraS1 ) {
                            sudoku = S2;
                            if ( filledS2 ){
                                sudoku.setStatus(1); 		// solved
                                sudoku.setDifficulty(DifficultyLevel);
                                return sudoku;
                            }
                            progress = true;
                            break;
                        }
                        if ( contraS2 ) {
                            sudoku = S1;
                            if ( filledS1 ){
                                sudoku.setStatus(1); 		// solved
                                sudoku.setDifficulty(DifficultyLevel);
                                return sudoku;
                            }
                            progress = true;
                            break;
                        }
                    }
                }

            }

        }

    }

    sudoku.setStatus(2); 		// unsolvable
    sudoku.setDifficulty(0);	// unknown
    return sudoku;

}

Sudoku9x9 Generate(int seed,int type){
    /* Function generates random sudoku using Solve() function.
    The result sudoku theoretically may be contradictory.
    If so, function returns sudoku with an appropriate status.
    In practise receiving a contradictory sudoku is impossible ( ~3.92e-124 chance ). */

    int ZeroGrid[9][9] = {
    { 0,0,0,0,0,0,0,0,0 },
    { 0,0,0,0,0,0,0,0,0 },
    { 0,0,0,0,0,0,0,0,0 },
    { 0,0,0,0,0,0,0,0,0 },
    { 0,0,0,0,0,0,0,0,0 },
    { 0,0,0,0,0,0,0,0,0 },
    { 0,0,0,0,0,0,0,0,0 },
    { 0,0,0,0,0,0,0,0,0 },
    { 0,0,0,0,0,0,0,0,0 }
    };
    Sudoku9x9 sudoku(ZeroGrid,type), old_sudoku(ZeroGrid,type);
    srand(seed);

    int status, iter = 0, iter2 = 0, max_iter = 20, max_iter2 = 100;
    while( iter2 <= max_iter2 ){
        Sudoku9x9 sudoku(ZeroGrid,type);
        int startingDigits = 17;
        if( type == 3 ) startingDigits = 2;
        for(int i=0; i<startingDigits; i++) sudoku.InsertRandomDigit();
        iter = 0;
        while( iter <= max_iter ){
            old_sudoku = sudoku;
            sudoku.InsertRandomDigit();

            sudoku = Solve(sudoku);
            status = sudoku.getStatus();
            //sudoku.PrintToConsole();
            //cout<<"------------------"<<status<<"\n";
            if ( status == 1 ){
                Sudoku9x9 sudoku_gen(sudoku.GivenGrid,type);
                sudoku_gen.setDifficulty(sudoku.getDifficulty());
                sudoku_gen.setSeed(seed);
                return sudoku_gen;
            } else if(  status == 4 ){
                sudoku = old_sudoku;
            }

            iter++;
        }
        iter2++;
    }

    // function should never reach this point
    sudoku.setSeed(123);
    return sudoku;
}

void RestrictDigits(Sudoku9x9 &sudoku){
    int rows[] = {0,1,2,3,4,5,6,7,8};
    int cols[] = {0,1,2,3,4,5,6,7,8};
    std::random_shuffle(rows,rows+9);
    std::random_shuffle(cols,cols+9);

    for(int r=0;r<9;r++){
        for(int c=0;c<9;c++){
            if( sudoku.GivenGrid[rows[r]][cols[c]] != 0 ){
                Sudoku9x9 S = Sudoku9x9(sudoku.GivenGrid,sudoku.getType());
                S.setSeed(sudoku.getSeed());
                S.GivenGrid[rows[r]][cols[c]] = 0;
                S.CurrentGrid[rows[r]][cols[c]] = 0;
                S = Solve(S);
                if( S.getStatus() == 1 ){
                    sudoku = Sudoku9x9(S.GivenGrid,S.getType());
                    sudoku.setDifficulty(S.getDifficulty());
                    sudoku.setSeed(S.getSeed());
                }
            }
        }
    }
}

void AddDigits(Sudoku9x9 &sudoku, int I){
    /* Function adds to sudoku a random digit from its solution.
    So, at least in theory, it makes sudoku easier. */

    if ( I <= 0 || I  > 30 ) return;

    Sudoku9x9 S = Solve(sudoku);
    if ( S.getStatus() != 1 ) return;
    int i = 0, r, c;

    while( i < I ){
        r = rand()%9, c = rand()%9;
        if( sudoku.GivenGrid[r][c] == 0 ){
            sudoku.GivenGrid[r][c] = S.CurrentGrid[r][c];
            sudoku.CurrentGrid[r][c] = S.CurrentGrid[r][c];
            i++;
        }
    }
}

void Rotate90(Sudoku9x9 &sudoku){
    int temp;
    for( int r=0;r<4;r++){
        for( int c=0;c<5;c++){
            temp = sudoku.GivenGrid[r][c];
            sudoku.GivenGrid[r][c] = sudoku.GivenGrid[8-c][r];
            sudoku.GivenGrid[8-c][r] = sudoku.GivenGrid[8-r][8-c];
            sudoku.GivenGrid[8-r][8-c] = sudoku.GivenGrid[c][8-r];
            sudoku.GivenGrid[c][8-r] = temp;
            temp = sudoku.CurrentGrid[r][c];
            sudoku.CurrentGrid[r][c] = sudoku.CurrentGrid[8-c][r];
            sudoku.CurrentGrid[8-c][r] = sudoku.CurrentGrid[8-r][8-c];
            sudoku.CurrentGrid[8-r][8-c] = sudoku.CurrentGrid[c][8-r];
            sudoku.CurrentGrid[c][8-r] = temp;
        }
    }
    //sudoku.UpdatePossGrid();
}
