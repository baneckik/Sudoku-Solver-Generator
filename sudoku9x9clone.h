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
        int getRegSize(){ return reg_Size; };

        void InsertRegionCell(int r, int c);
        void UpdateRegShifted();
        bool IsRegShiftedInGrid();
        bool IsRegShiftedAdjacentBySide();
        bool IsRegShiftedAdjacentByCorner();
        void CreateRegion(int seed);

        void UpdatePossGrid();
        void InsertRandomDigit();

        void PrintToConsole();
};

#endif  //SUDOKU9x9CLONE

#ifndef SUDOKU9X9CLONE
#define SUDOKU9X9CLONE

Sudoku9x9Clone::Sudoku9x9Clone(){
    setType(6);
    setStatus(0);
    setDifficulty(0);
    setSeed(0);

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
    setType(6);
    setStatus(0);
    setDifficulty(0);
    setSeed(0);

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

void Sudoku9x9Clone::CreateRegion(int seed){
    /* Function generates random region of size reg_Size and shift coordinates, so
    reg_shifted lays in the grid and is not adjacent by corner with first region. */

    if( reg_Size > 36 || reg_Size < 1 ) return;

    srand(seed);
    int max_iter=100; // number of times we want to create new region 
    // ( it is possible that for some of regions it does not exists a good shift )

    for( int iter=0; iter<max_iter; iter++ ){

        for( int cell=0; cell<81; cell++ ){
            reg.Grid[cell] = false;
            reg_shifted.Grid[cell] = false;
            shift_r = 0;
            shift_c = 0;
        }

        // Creating reg.

        // start cell
        int startR = rand()%9;
        int startC = rand()%9;
        reg.Grid[startR*9+startC] = true;
        // next cells
        bool WeAreStuck = false;
        for( int k=1; k<reg_Size; k++ ){
            if( WeAreStuck ) break;
            // for every region we choose the cell "Cell" to grow region from
            bool progress = false;
            WeAreStuck = false;
            
            int Cell = rand()%reg.RegionSize(); // first candidate for grow cell
            int Cell1 = -1, iter4=0;
            int r, c;

            for( int C=0; C<81; C++ ){
                if( reg.Grid[C] ) Cell1++;

                if( reg.Grid[C] && Cell1 == Cell ){
                    if( reg.IsBoundary(C/9,C%9) ){
                        // We have found good cell to grow region from
                        Cell1 = C; // Cell1 is now index of our Cell in Grid
                        r = Cell1/9;
                        c = Cell1%9;
                        progress = true;
                        break;
                    }
                    else{
                        // We are looking for another good cell to grow region from
                        Cell++;
                        Cell = Cell%reg.RegionSize();
                        C=-1;
                        Cell1=-1;
                    }
                }
            }
            
            if( !progress ){
                // means if we haven't found a cell good to grow from (that's impossible if reg_Size is lower than 81)
                WeAreStuck = true;
                break;
            }else{
                // if we have found a cell good to grow from
        
                std::vector<int> directories = {0,1,2,3};
                std::random_device rd;
                std::mt19937 g(rd());
                std::shuffle(directories.begin(),directories.end(),g);
                for( int l=0; l<4; l++ ){
                    // cell to the top is empty
                    if( directories[l] == 0 && r>0 && !reg.Grid[Cell1-9] ){
                        InsertRegionCell(r-1,c);
                        break;
                    }
                    // cell to the bottom is empty
                    if( directories[l] == 1 && r<8 && !reg.Grid[Cell1+9] ){
                        InsertRegionCell(r+1,c);
                        break;
                    }
                    // cell to the left is empty
                    if( directories[l] == 2 && c>0 && !reg.Grid[Cell1-1] ){
                        InsertRegionCell(r,c-1);
                        break;
                    }
                    // cell to the right is empty
                    if( directories[l] == 3 && c<8 && !reg.Grid[Cell1+1] ){
                        InsertRegionCell(r,c+1);
                        break;
                    }
                }
            }
        }
        
        if( WeAreStuck ) break;

        UpdateRegShifted();

        // Choosing shift coordinates
        // possible shifts: r\in[-startR,8-startR], c\in[-startC,8-startC]
        std::vector<int> rows = {0,1,2,3,4,5,6,7,8};
        std::vector<int> cols = {0,1,2,3,4,5,6,7,8};
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(rows.begin(),rows.end(),g);
        std::shuffle(cols.begin(),cols.end(),g);
        for(int r=0; r<9; r++ ){
            for( int c=0; c<9; c++ ){
                shift_r = rows[r] - startR;
                shift_c = cols[c] - startC;
                UpdateRegShifted();
                if( shift_r!=0 && shift_c!=0 && IsRegShiftedInGrid() && !AreAdjacentByCorner(reg,reg_shifted,9,9) ){
                    return;
                }
            }
        }
        for(int r=0; r<9; r++ ){
            for( int c=0; c<9; c++ ){
                shift_r = rows[r] - startR;
                shift_c = cols[c] - startC;
                UpdateRegShifted();
                if( shift_r!=0 && shift_c!=0 && IsRegShiftedInGrid() && !AreAdjacentBySide(reg,reg_shifted,9,9) ){
                    return;
                }
            }
        }
    }
    // In case there is no way the function can find a region and its shift
    // Function resets both regions
    // Function should never reach this point
    for( int cell=0; cell<81; cell++ ){
        reg.Grid[cell] = false;
        reg_shifted.Grid[cell] = false;
        shift_r = 0;
        shift_c = 0;
    }
}

void Sudoku9x9Clone::UpdatePossGrid(){
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

    // eliminating possibilities in corresponding clone cells
    if( !IsRegShiftedInGrid() ) return; 
    for( int cell=0; cell<81; cell++ ){
        if( reg.Grid[cell] ){
            for( int d=0; d<9; d++ ){
                if( PossibilitiesGrid[cell/9][cell%9][d]!=PossibilitiesGrid[cell/9+getShiftR()][cell%9+getShiftC()][d]){
                    PossibilitiesGrid[cell/9][cell%9][d] = false;
                    PossibilitiesGrid[cell/9+getShiftR()][cell%9+getShiftC()][d] = false;
                }
            }
        }
    }
}

void Sudoku9x9Clone::PrintToConsole(){
    int Width = 9;
    int Hight = 9;

    for (int r = 0; r < Hight; r++) {
        // Current Grid
        for (int c = 0; c < Width; c++) {
            if ( CurrentGrid[r][c] == 0 )
                std::cout << "\033[1;31m";
            else if( CurrentGrid[r][c] == GivenGrid[r][c])
                 std::cout << "\033[1;37m";
            else
                std::cout << "\033[1;32m";

            std::cout << CurrentGrid[r][c] << " ";
        }
        // Cloned regions
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
        // Possibilities Grid
        std::cout<<" ";
        for( int c=0;c<9;c++ ){
            if( N_Possibilities(r,c) == 9 ) std::cout<<"\033[1;32m";
            else if( N_Possibilities(r,c) == 1 ) std::cout<<"\033[1;37m";
            else if( N_Possibilities(r,c) == 0 ) std::cout<<"\033[1;31m";
            else std::cout<<"\033[1;33m";
            
            std::cout<<N_Possibilities(r,c)<< " ";
        }
        // Information
        std::cout <<"\033[0m";
        if( r == 0 ){ 
            std::cout<<"Type: ";
            if( getType() == 1 ) std::cout<<"Classic";
            else if( getType() == 2 ) std::cout<<"Diagonal";
            else if( getType() == 3 ) std::cout<<"Non-Consecutive";
            else if( getType() == 4 ) std::cout<<"Anti-Knight";
            else if( getType() == 5 ) std::cout<<"Irregular";
            else if( getType() == 6 ) std::cout<<"Clone";
            else std::cout<<"Unknown";
        }
        else if( r == 1 ){ 
            std::cout<<"Status: ";
            if( getStatus() == 1 ) std::cout<<"\033[1;32m"<<"Solved";
            else if( getStatus() == 2 ) std::cout<<"Unknown";
            else if( getStatus() == 3 ) std::cout<<"Ambiguous";
            else if( getStatus() == 4 ) std::cout<<"\033[1;31m"<<"Contradictory";
            else std::cout<<"Not checked yet";
        }
        else if( r == 2 ){ 
            std::cout<<"Difficulty: ";
            if( getDifficulty() == 0 ) std::cout<<"Unknown";
            else std::cout<<getDifficulty();
        }
        else if( r == 3 ){ 
            std::cout<<"Seed: ";
            if( getSeed() == 0 ) std::cout<<"-not generated-";
            else if( getSeed() == -1 ) std::cout<<"\033[1;31m"<<"Generator failed";
            else std::cout<<getSeed();
        }
        std::cout << std::endl;
    }
    std::cout<<"\033[0m"<<"\n";
}

#endif
