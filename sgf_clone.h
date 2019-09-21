#include <vector>
#include "solvegeneratefunctions.h"
#include "sudoku9x9clone.h"

#ifndef FUNCTIONS_CLONE
#define FUNCTIONS_CLONE

bool IsContradictory(Sudoku9x9Clone &sudoku){
    // if there is a place in the grid with no possible digit
    int suma = 0;
    for (int r = 0; r < 9; r++)
        for (int c = 0; c < 9; c++) {
            for (int d = 0; d < 9; d++)
                suma += sudoku.PossibilitiesGrid[r][c][d];
            if (suma == 0) return true;
            suma = 0;
        }

    std::vector<int> quantities = {0,0,0,0,0,0,0,0,0};
    
    // if there is at most one digit in each row
    // and if there is at least one possibilitie for every digit in each row
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++)
            if (sudoku.CurrentGrid[r][c] != 0) quantities[sudoku.CurrentGrid[r][c] - 1]++;
        if ( std::any_of(quantities.begin(), quantities.end(), [](int i){return i>1;}) ) return true;
        for (int i = 0; i < 9; i++) quantities[i] = 0;

        for (int d=0; d<9; d++){
            for (int c = 0; c < 9; c++)
                quantities[d] += sudoku.PossibilitiesGrid[r][c][d];
        }
        if ( std::any_of(quantities.begin(), quantities.end(), [](int i){return i==0;}) ) return true;
        for (int i = 0; i < 9; i++) quantities[i] = 0;

    }
    
    // if there is at most one digit in each column
    // and if there is at least one possibilitie for every digit in each column
    for (int c = 0; c < 9; c++) {
        for (int r = 0; r < 9; r++)
            if (sudoku.CurrentGrid[r][c] != 0) quantities[sudoku.CurrentGrid[r][c] - 1]++;
        if ( std::any_of(quantities.begin(), quantities.end(), [](int i){return i>1;}) ) return true;
        for (int i = 0; i < 9; i++) quantities[i] = 0;

        for (int d=0; d<9; d++){
            for (int r = 0; r < 9; r++)
                quantities[d] += sudoku.PossibilitiesGrid[r][c][d];
        }
        if ( std::any_of(quantities.begin(), quantities.end(), [](int i){return i==0;}) ) return true;
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
                if ( std::any_of(quantities.begin(), quantities.end(), [](int i){return i>1;}) ) return true;
            }
            for (int i = 0; i < 9; i++) quantities[i] = 0;

            for (int d=0; d<9; d++){
                for (int r = 0; r < 3; r++) for (int c = 0; c < 3; c++)
                    quantities[d] += sudoku.PossibilitiesGrid[boxr*3 + r][boxc*3 + c][d];
            }
            if ( std::any_of(quantities.begin(), quantities.end(), [](int i){return i==0;}) ) return true;
            for (int i = 0; i < 9; i++) quantities[i] = 0;
        }
    }
    
    if( !sudoku.IsRegShiftedInGrid() ) return true;
    // if there are same digits on corresponding cells in shifted regions
    for( int cell=0; cell<81; cell++ ){
        if( sudoku.reg.Grid[cell] && 
        sudoku.CurrentGrid[cell/9][cell%9] != 0 &&
        sudoku.CurrentGrid[cell/9+sudoku.getShiftR()][cell%9+sudoku.getShiftC()] != 0 &&
        sudoku.CurrentGrid[cell/9][cell%9]!=sudoku.CurrentGrid[cell/9+sudoku.getShiftR()][cell%9+sudoku.getShiftC()]){
            return true;
        }
    }

    return false;
}

void UpdatePossGrid_Clones(Sudoku9x9Clone &sudoku, bool &progress){
    // basic actualization of PossibilitiesGrid in cloned regions
    for( int cell=0; cell<81; cell++ ){
        if( sudoku.reg.Grid[cell] ){
            for( int d=0; d<9; d++ ){
                if( sudoku.PossibilitiesGrid[cell/9][cell%9][d]!=sudoku.PossibilitiesGrid[cell/9+sudoku.getShiftR()][cell%9+sudoku.getShiftC()][d]){
                    progress = true;
                    sudoku.PossibilitiesGrid[cell/9][cell%9][d] = false;
                    sudoku.PossibilitiesGrid[cell/9+sudoku.getShiftR()][cell%9+sudoku.getShiftC()][d] = false;
                }
            }
        }
    }
}

void UpdateCurrentGrid(Sudoku9x9Clone &sudoku, bool &progress){
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

void TryToSolveEasy(Sudoku9x9Clone &sudoku){
    /* Function tries to solve sudoku using only hidden singles
    and rows/cols/boxes where certain digit can fit into only one place. */
    
    if( IsContradictory(sudoku) ){
        sudoku.setStatus(4); // contradictory
        return;
    }

    bool progress = true;
    while(progress){
        progress = false;

        // actualization of PossibilitiesGrid
        UpdatePossGrid_Classic(sudoku, progress);
        UpdatePossGrid_Clones(sudoku, progress);

        // actualization of CurrentGrid
        UpdateCurrentGrid(sudoku, progress);
    }
}

void TryToSolve(Sudoku9x9Clone &sudoku){
    /* Function tries to solve sudoku using TryToSolveEasy()
    and restricting certain rows/cols techniques. */
    
    if( IsContradictory(sudoku) ){
        sudoku.setStatus(4); // contradictory
        return;
    }

    bool progress = true;
    while(progress){
        progress = false;

        // actualization of PossibilitiesGrid
        UpdatePossGrid_Classic(sudoku, progress);
        UpdatePossGrid_Clones(sudoku, progress);
        
        // Basic elimination technique nr 1
        // When in certain box digit can fit only into cells in one row/col, then the possibilities
        // from the rest of this row/col for this digit can be eliminated. 
        // ( not for irregular sudoku )
        int type = sudoku.getType();

        if( type==1 || type==2 || type==3 || type==4 || type==6 ){
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
        }

        // actualization of CurrentGrid
        UpdateCurrentGrid(sudoku, progress);
    }
}

void Solve(Sudoku9x9Clone &sudoku){
    /* Main solving function.
    Solves sudoku, estimates difficulty and gives appropriate status. */

    if( IsContradictory(sudoku) ){
        sudoku.setStatus(4); // contradictory
        return;
    }

    TryToSolveEasy(sudoku);
    if( IsContradictory(sudoku) ){
        sudoku.setStatus(4); 		// contradictory
        return;
    }
    if( IsFilled(sudoku) ){
        sudoku.setStatus(1); 		// solved
        sudoku.setDifficulty(1); 	// Very Easy
        return;
    }

    TryToSolve(sudoku);
    if( IsContradictory(sudoku) ){
        sudoku.setStatus(4); 		// contradictory
        return;
    }
    if( IsFilled(sudoku) ){
        sudoku.setStatus(1); 		// solved
        sudoku.setDifficulty(2); 	// Easy
        return;
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
                if ( progress ) break;
                int c1 = 0, c2 = 0;
                for (int r = 0; r < 9; r++) {
                    if ( progress ) break;
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
                        int temp_CurrentGrid[9][9];
                        for( int r=0;r<9; r++ )
                            for( int c=0; c<9; c++)
                                temp_CurrentGrid[r][c] = sudoku.CurrentGrid[r][c];
                        bool temp_PossGrid[9][9][9];
                        for( int r=0;r<9; r++ )
                            for( int c=0; c<9; c++)
                                for( int d=0; d<9; d++ )
                                    temp_PossGrid[r][c][d] = sudoku.PossibilitiesGrid[r][c][d];
                        
                        

                        // first option: trying to solve and save info(is contradictory or filled)
                        sudoku.CurrentGrid[r][c1] = d+1;
                        TryToSolve(sudoku);
                        bool contraS1 = IsContradictory(sudoku), filledS1 = IsFilled(sudoku);

                        // reseting sudoku to the state before first option
                        for( int r=0;r<9; r++ )
                            for( int c=0; c<9; c++)
                                sudoku.CurrentGrid[r][c] = temp_CurrentGrid[r][c];
                        for( int r=0;r<9; r++ )
                            for( int c=0; c<9; c++)
                                for( int d=0; d<9; d++ )
                                    sudoku.PossibilitiesGrid[r][c][d] = temp_PossGrid[r][c][d];

                        // second option: trying to solve and save info(is contradictory or filled)
                        sudoku.CurrentGrid[r][c2] = d+1;
                        TryToSolve(sudoku);
                        bool contraS2 = IsContradictory(sudoku), filledS2 = IsFilled(sudoku);

                        // reseting sudoku to the state before first option
                        for( int r=0;r<9; r++ )
                            for( int c=0; c<9; c++)
                                sudoku.CurrentGrid[r][c] = temp_CurrentGrid[r][c];
                        for( int r=0;r<9; r++ )
                            for( int c=0; c<9; c++)
                                for( int d=0; d<9; d++ )
                                    sudoku.PossibilitiesGrid[r][c][d] = temp_PossGrid[r][c][d];

                        if( contraS1 && contraS2 ){
                            sudoku.setStatus(4); 	// contradictory
                            return;
                        }
                        if( filledS1 && filledS2 && !contraS1 && !contraS2 ){
                            sudoku.setStatus(3);	// ambiguous
                            return;
                        }
                        if ( contraS1 ) {
                            sudoku.CurrentGrid[r][c2] = d+1;
                            TryToSolve(sudoku);
                            if ( filledS2 ){
                                sudoku.setStatus(1); 		// solved
                                sudoku.setDifficulty(DifficultyLevel);
                                return;
                            }
                            progress = true;
                            break;
                        }
                        if ( contraS2 ) {
                            sudoku.CurrentGrid[r][c1] = d+1;
                            TryToSolve(sudoku);
                            if ( filledS1 ){
                                sudoku.setStatus(1); 		// solved
                                sudoku.setDifficulty(DifficultyLevel);
                                return;
                            }
                            progress = true;
                            break;
                        }
                    }
                }
                // columns in which this digit can fit only into exactly 2 places:
                if ( progress ) break;
                int r1 = 0, r2 = 0;
                for (int c = 0; c < 9; c++) {
                    if ( progress ) break;
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
                        int temp_CurrentGrid[9][9];
                        for( int r=0;r<9; r++ )
                            for( int c=0; c<9; c++)
                                temp_CurrentGrid[r][c] = sudoku.CurrentGrid[r][c];
                        bool temp_PossGrid[9][9][9];
                        for( int r=0;r<9; r++ )
                            for( int c=0; c<9; c++)
                                for( int d=0; d<9; d++ )
                                    temp_PossGrid[r][c][d] = sudoku.PossibilitiesGrid[r][c][d];

                        // first option: trying to solve and save info(is contradictory or filled)
                        sudoku.CurrentGrid[r1][c] = d+1;
                        TryToSolve(sudoku);
                        bool contraS1 = IsContradictory(sudoku), filledS1 = IsFilled(sudoku);

                        // reseting sudoku to the state before first option
                        for( int r=0;r<9; r++ )
                            for( int c=0; c<9; c++)
                                sudoku.CurrentGrid[r][c] = temp_CurrentGrid[r][c];
                        for( int r=0;r<9; r++ )
                            for( int c=0; c<9; c++)
                                for( int d=0; d<9; d++ )
                                    sudoku.PossibilitiesGrid[r][c][d] = temp_PossGrid[r][c][d];

                        // second option: trying to solve and save info(is contradictory or filled)
                        sudoku.CurrentGrid[r2][c] = d+1;
                        TryToSolve(sudoku);
                        bool contraS2 = IsContradictory(sudoku), filledS2 = IsFilled(sudoku);

                        // reseting sudoku to the state before first option
                        for( int r=0;r<9; r++ )
                            for( int c=0; c<9; c++)
                                sudoku.CurrentGrid[r][c] = temp_CurrentGrid[r][c];
                        for( int r=0;r<9; r++ )
                            for( int c=0; c<9; c++)
                                for( int d=0; d<9; d++ )
                                    sudoku.PossibilitiesGrid[r][c][d] = temp_PossGrid[r][c][d];

                        if( contraS1 && contraS2 ){
                            sudoku.setStatus(4); 	// contradictory
                            std::cout<<"jest sprzecznosc trudna\n";
                            return;
                        }
                        if( filledS1 && filledS2 && !contraS1 && !contraS2 ){
                            sudoku.setStatus(3);	// ambiguous
                            return;
                        }
                        if ( contraS1 ) {
                            sudoku.CurrentGrid[r2][c] = d+1;
                            TryToSolve(sudoku);
                            if ( filledS2 ){
                                sudoku.setStatus(1); 		// solved
                                sudoku.setDifficulty(DifficultyLevel);
                                return;
                            }
                            progress = true;
                            break;
                        }
                        if ( contraS2 ) {
                            sudoku.CurrentGrid[r1][c] = d+1;
                            TryToSolve(sudoku);
                            if ( filledS1 ){
                                sudoku.setStatus(1); 		// solved
                                sudoku.setDifficulty(DifficultyLevel);
                                return;
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
}

void Sudoku9x9Clone::InsertRandomDigit(){
    /* Function inserts random digit at a random place in the grid (if it's possible).
    Function returns true if random digit was succesfully inserted & false otherwise. */

    UpdatePossGrid();

    int r=10, c=10, iter=0, max_iter=100, max_iter2=100;
    for( int i=0;i<max_iter2; i++ ){
        do{
            r = rand()%9;
            c = rand()%9;
            iter++;
        } while ( N_Possibilities(r,c) <= 1 && iter < max_iter );

        if( iter >= max_iter ) return;

        int digits[] = {0,1,2,3,4,5,6,7,8};
        std::random_shuffle(digits,digits+9);

        for(int d=0; d<9; d++){
            if( PossibilitiesGrid[r][c][digits[d]] == true ){
                
                CurrentGrid[r][c] = digits[d]+1;
                Solve(*this);
                
                if( this->getStatus() != 4 ){
                    this->ResetCurrentGrid();
                    GivenGrid[r][c] = digits[d]+1;
                    CurrentGrid[r][c] = digits[d]+1;
                    return;
                }
                this->ResetCurrentGrid();
            }
        }
    }
}

void Generate_Clone(Sudoku9x9Clone &sudoku, int seed){
    /* Function generates random clone sudoku using Solve() function.
    The result sudoku theoretically may be contradictory.
    If so, function returns sudoku with an appropriate status. */

    sudoku.setStatus(0);
    sudoku.setDifficulty(0);
    sudoku.setSeed(seed);    

    srand(seed);

    int status, iter = 0, iter2 = 0, max_iter = 40, max_iter2 = 100;
    // max_iter2 times we are trying from the beginning
    while( iter2 <= max_iter2 ){
        // clearing the grids
        sudoku.ResetAllGrids();
        
        // creating clones
        sudoku.CreateRegion(seed+iter2);

        // inserting starting digits
        int startingDigits = 1;
        for(int i=0; i<startingDigits; i++) sudoku.InsertRandomDigit();

        iter = 0;
        // trying to insert random digit max_iter times
        while( iter <= max_iter ){
            int temp_Grid[9][9];
            for( int r=0;r<9; r++ )
                for( int c=0; c<9; c++)
                    temp_Grid[r][c] = sudoku.GivenGrid[r][c];

            sudoku.InsertRandomDigit();
            int R,C,D; // coordinates of inserted digit
            for( int cell=0; cell<81; cell++ )
                if( temp_Grid[cell/9][cell%9] != sudoku.GivenGrid[cell/9][cell%9] ){
                    R = cell/9;
                    C = cell%9;
                    D = sudoku.GivenGrid[cell/9][cell%9];
                    break;
                }

            // trying to fit any of possible digits in R,C cell
            int current_D=D;
            while( current_D != (D+7)%9+1 ){
                sudoku.GivenGrid[R][C] = current_D;
                sudoku.CurrentGrid[R][C] = current_D;
                Solve(sudoku);
                status = sudoku.getStatus();

                if ( status == 1 ){
                    sudoku.ResetCurrentGrid();
                    sudoku.setSeed(seed);
                    return;
                } else if(  status != 4 ){
                    break;
                }
                
                current_D = current_D%9 +1;
                sudoku.ResetCurrentGrid();
            }
            // if none of digits fit, return to the temp_Grid
            if( status == 4 ){
                break;
            }

            iter++;
        }
        iter2++;
    }

    // function should never reach this point
    sudoku.ResetAllGrids();
    sudoku.setStatus(0);
    sudoku.setDifficulty(0);
    sudoku.setSeed(-1);
}

void RestrictDigits(Sudoku9x9Clone &sudoku){
    int rows[] = {0,1,2,3,4,5,6,7,8};
    int cols[] = {0,1,2,3,4,5,6,7,8};
    std::random_shuffle(rows,rows+9);
    std::random_shuffle(cols,cols+9);

    for(int r=0;r<9;r++){
        for(int c=0;c<9;c++){
            if( sudoku.GivenGrid[rows[r]][cols[c]] != 0 ){
                int temp_Grid[9][9];
                for( int r=0;r<9; r++ )
                    for( int c=0; c<9; c++)
                        temp_Grid[r][c] = sudoku.GivenGrid[r][c];

                sudoku.setSeed(sudoku.getSeed());
                sudoku.GivenGrid[rows[r]][cols[c]] = 0;
                sudoku.CurrentGrid[rows[r]][cols[c]] = 0;
                Solve(sudoku);
                if( sudoku.getStatus() == 1 ){
                    sudoku.setDifficulty(sudoku.getDifficulty());
                    sudoku.setSeed(sudoku.getSeed());
                }
                for( int r=0;r<9; r++ )
                    for( int c=0; c<9; c++)
                        sudoku.GivenGrid[r][c] = temp_Grid[r][c];
                sudoku.ResetCurrentGrid();
            }
        }
    }
}

void AddDigits(Sudoku9x9Clone &sudoku, int I){
    /* Function adds to sudoku a random digit from its solution.
    So, at least in theory, it makes sudoku easier. */

    if ( I <= 0 || I  > 30 ) return;

    int temp_Grid[9][9];
    for( int r=0;r<9; r++ )
        for( int c=0; c<9; c++)
            temp_Grid[r][c] = sudoku.GivenGrid[r][c];

    Solve(sudoku);
    if ( sudoku.getStatus() != 1 ) return;
    int i = 0, r, c;

    while( i < I ){
        r = rand()%9; 
        c = rand()%9;
        if( sudoku.GivenGrid[r][c] == 0 ){
            sudoku.GivenGrid[r][c] = sudoku.CurrentGrid[r][c];
            i++;
        }
    }
    sudoku.ResetCurrentGrid();
}

#endif  //FUNCTIONS_CLONE