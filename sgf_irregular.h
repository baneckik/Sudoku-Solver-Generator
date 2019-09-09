#include <vector>
#include "solvegeneratefunctions.h"
using namespace std;

bool IsContradictory(Sudoku9x9Irregular &sudoku){
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
    
    if( !(*sudoku.GoR).IsFilled() ) return false;
    // if there is at most one digit in each box
    // and if there is at least one possibilitie for every digit in each box
    for( int reg=0; reg<(*sudoku.GoR).getNumberOfRegions(); reg++){
        for (int cell = 0; cell < 81; cell++) {
                if ((*sudoku.GoR).Grid[cell]==reg && sudoku.CurrentGrid[cell/9][cell%9] != 0)
                    quantities[sudoku.CurrentGrid[cell/9][cell%9] - 1]++;
            if ( any_of(quantities.begin(), quantities.end(), [](int i){return i>1;}) ) 
                return true;
        }
        for (int i = 0; i < 9; i++) quantities[i] = 0;

        for (int d=0; d<9; d++){
            for (int cell = 0; cell < 81; cell++) 
                if ( (*sudoku.GoR).Grid[cell]==reg )
                    quantities[d] += sudoku.PossibilitiesGrid[cell/9][cell%9][d];
        }
        if ( any_of(quantities.begin(), quantities.end(), [](int i){return i==0;}) ) 
            return true;
        for (int i = 0; i < 9; i++) quantities[i] = 0;
    }
    
    return false;
}

void UpdatePossGrid_Regions(Sudoku9x9Irregular &sudoku, bool &progress){
    // basic actualization of PossibilitiesGrid in irregular regions of digit from 1 to 9
    if( !(*sudoku.GoR).IsEquallyDivided() ) return;

    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            if (sudoku.CurrentGrid[r][c] != 0) {
                int D = sudoku.CurrentGrid[r][c];
                int reg = (*sudoku.GoR).Grid[r*9+c];
                // eliminating possibilities in the region
                for( int Cell=0; Cell<81; Cell++ ){
                    if ( (*sudoku.GoR).Grid[Cell] == reg && Cell != r*9+c ) {
                        if (sudoku.PossibilitiesGrid[Cell/9][Cell%9][D-1] == true) {
                                progress = true;
                                sudoku.PossibilitiesGrid[Cell/9][Cell%9][D-1] = false;
                        }
                    }
                }
            }
        }
    }
}

void UpdateCurrentGrid(Sudoku9x9Irregular &sudoku, bool &progress){
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
        for(int reg=0; reg<(*sudoku.GoR).getNumberOfRegions(); reg++){
            int sum = 0;
            for( int cell=0; cell<81; cell++ ) 
                if( (*sudoku.GoR).Grid[cell] == reg )
                    sum += sudoku.PossibilitiesGrid[cell/9][cell%9][d];
            if( sum == 1 ){
                for( int cell=0; cell<81; cell++ )
                    if( (*sudoku.GoR).Grid[cell] == reg && sudoku.PossibilitiesGrid[cell/9][cell%9][d] == true ){
                        if( sudoku.CurrentGrid[cell/9][cell%9] == 0 ){
                            progress = true;
                            sudoku.CurrentGrid[cell/9][cell%9] = d+1;
                        }
                        break;
                    }
            }
        }
    }
}

void TryToSolve(Sudoku9x9Irregular &sudoku){
    /* Function tries to solve sudoku using only hidden singles
    and rows/cols/boxes where certain digit can fit into only one place. */
    std::cout<<"Trytosolve function for irregular\n";
    
    if( IsContradictory(sudoku) ){
        sudoku.setStatus(4); // contradictory
        std::cout<<"Jest sprzecznosc!!!\n";
        return;
    }

    bool progress = true;
    while(progress){
        progress = false;

        // actualization of PossibilitiesGrid
        UpdatePossGrid_RowsCols(sudoku, progress);
        UpdatePossGrid_Regions(sudoku, progress);
        
        // actualization of CurrentGrid
        UpdateCurrentGrid(sudoku, progress);
        
    }
}

void Solve(Sudoku9x9Irregular &sudoku){
    /* Main solving function.
    Solves sudoku, estimates difficulty and gives appropriate status. */
    std::cout<<"Solving...\n";
    if( IsContradictory(sudoku) ){
        sudoku.setStatus(4); // contradictory
        std::cout<<"bylo sprzeczne :(\n";
        return;
    }

    TryToSolve(sudoku);
    if( IsContradictory(sudoku) ){
        sudoku.setStatus(4); 		// contradictory
        std::cout<<"bylo sprzeczne :(\n";
        return;
    }
    if( IsFilled(sudoku) ){
        sudoku.setStatus(1); 		// solved
        sudoku.setDifficulty(1); 	// Easy
        return;
    }

    bool progress = true;
    int DifficultyLevel = 1;

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

                        // first option
                        sudoku.CurrentGrid[r][c1] = d+1;
                        TryToSolve(sudoku);
                        bool contraS1 = IsContradictory(sudoku), filledS1 = IsFilled(sudoku);
                        for( int r=0;r<9; r++ )
                            for( int c=0; c<9; c++)
                                sudoku.CurrentGrid[r][c] = temp_CurrentGrid[r][c];

                        // second option
                        sudoku.CurrentGrid[r][c2] = d+1;
                        TryToSolve(sudoku);
                        bool contraS2 = IsContradictory(sudoku), filledS2 = IsFilled(sudoku);
                        for( int r=0;r<9; r++ )
                            for( int c=0; c<9; c++)
                                sudoku.CurrentGrid[r][c] = temp_CurrentGrid[r][c];

                        std::cout<<contraS1<<contraS2<<filledS1<<filledS2<<"\n";
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

                        // first option
                        sudoku.CurrentGrid[r1][c] = d+1;
                        TryToSolve(sudoku);
                        bool contraS1 = IsContradictory(sudoku), filledS1 = IsFilled(sudoku);
                        for( int r=0;r<9; r++ )
                            for( int c=0; c<9; c++)
                                sudoku.CurrentGrid[r][c] = temp_CurrentGrid[r][c];

                        // second option
                        sudoku.CurrentGrid[r2][c] = d+1;
                        TryToSolve(sudoku);
                        bool contraS2 = IsContradictory(sudoku), filledS2 = IsFilled(sudoku);
                        for( int r=0;r<9; r++ )
                            for( int c=0; c<9; c++)
                                sudoku.CurrentGrid[r][c] = temp_CurrentGrid[r][c];

                        if( contraS1 && contraS2 ){
                            sudoku.setStatus(4); 	// contradictory
                            return;
                        }
                        if( filledS1 && filledS2 && !contraS1 && !contraS2 ){
                            sudoku.setStatus(3);	// ambiguous
                            return;
                        }
                        if ( contraS1 ) {
                            sudoku.CurrentGrid[r2][c] = d+1;
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

    std::cout<<"Solve nie dalo rady...\n";
    sudoku.setStatus(2); 		// unsolvable
    sudoku.setDifficulty(0);	// unknown
}

Sudoku9x9Irregular Generate_Irregular(int seed){
    /* Function generates random irregular sudoku using Solve() function.
    The result sudoku theoretically may be contradictory.
    If so, function returns sudoku with an appropriate status. */

    Sudoku9x9Irregular sudoku;
    srand(seed);

    int status, iter = 0, iter2 = 0, max_iter = 20, max_iter2 = 100;
    while( iter2 <= max_iter2 ){
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++) {
                sudoku.GivenGrid[i][j] = 0;
                sudoku.CurrentGrid[i][j] = 0;
            }
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++)
                for (int k = 0; k < 9; k++)
                    sudoku.PossibilitiesGrid[i][j][k] = true;
        
        (*sudoku.GoR).CreateRegions(seed);
        int startingDigits = 3;

        for(int i=0; i<startingDigits; i++) sudoku.InsertRandomDigit();
        std::cout<<"Wygenerowano nowy grid\n";
        iter = 0;
        while( iter <= max_iter ){
            int temp_Grid[9][9];
            for( int r=0;r<9; r++ )
                for( int c=0; c<9; c++)
                    temp_Grid[r][c] = sudoku.GivenGrid[r][c];

            sudoku.InsertRandomDigit();
            std::cout<<"Added digit\n";

            Solve(sudoku);
            status = sudoku.getStatus();
            //sudoku.PrintToConsole();
            //cout<<"------------------"<<status<<"\n";
            if ( status == 1 ){
                return sudoku;
            } else if(  status == 4 ){
                for( int r=0;r<9; r++ )
                    for( int c=0; c<9; c++)
                        sudoku.GivenGrid[r][c] = temp_Grid[r][c];
                sudoku.ResetCurrentGrid();
            }

            iter++;
        }
        iter2++;
    }

    std::cout<<"NIe wyszlo :( "<<sudoku.getStatus()<<"\n";
    // function should never reach this point
    sudoku.setSeed(123);
    return sudoku;
}

void RestrictDigits(Sudoku9x9Irregular &sudoku){
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

void AddDigits(Sudoku9x9Irregular &sudoku, int I){
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
            sudoku.CurrentGrid[r][c] = sudoku.CurrentGrid[r][c];
            i++;
        }
    }
    sudoku.ResetCurrentGrid();
}