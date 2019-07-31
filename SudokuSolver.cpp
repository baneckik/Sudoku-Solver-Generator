//#include <windows.h>
#include <iostream>
#include <algorithm>
#include <vector>
//#include <string>
//#include <stdlib.h>
//#include <time.h>
using namespace std;

/*
int getGivenDigit(int row, int column){return GivenGrid[row][column];};
int getCurrentDigit(int row, int column){return CurrentGrid[row][column];};
int getPossibilitie(int row, int column, int digit){return PossibilitiesGrid[row][column][digit];};
int getType(){return Type;};
int getStatus(){return Status;};

void setGivenDigit(int row, int column, int d){GivenGrid[row][column] = d;};
void setCurrentDigit(int row, int column, int d){CurrentGrid[row][column] = d;};
void setPossibilitie(int row, int column, int digit, bool p){PossibilitiesGrid[row][column][digit] = p;};
void setStatus(int s){Status = s;};
 */


class Sudoku9x9{
    private:
        int Type;       // 1 - Classic, 2 - Nonconsecutive, 3 - Diagonal, 4 - AntiKnight
        int Status;     // 0 - Unknown, 1 - Solved, 2 - Unsolvable(by this solver), 3 - Ambiguous, 4 - Contradictionary
    public:
        int GivenGrid[9][9];        // [row][column]
        int CurrentGrid[9][9];      // [row][column]
        bool PossibilitiesGrid[9][9][9];     // [row][column][digit]
        
        Sudoku9x9(int Given[9][9], int type = 1);
        
        int getType(){return Type;};
        int getStatus(){return Status;};

        void setStatus(int s){Status = s;};

        int N_Given();     // number of digits given
        int N_Current();   // number of digits currently placed on the grid
        int N_Possibilities(int r, int c); // number of digits possible at row r and column c
		int N_Possibilities(int d, int rc, bool type); 
			// number of digits possible for a digit d in a row(type=0) or column(type=1) rc

		void PrintToConsole();	
};

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
				cout << "\033[1;31m";
			else if( CurrentGrid[r][c] == GivenGrid[r][c]) 
				 cout << "\033[1;37m";
			else 
				cout << "\033[1;32m";

			cout << CurrentGrid[r][c];
			cout <<"\033[0m";
			if ((c + 1) % 3 == 0)cout << " ";
		}
		if ((r + 1) % 3 == 0)cout << endl;
		cout << endl;
	}
}

bool IsContradictory(Sudoku9x9 sudoku){
	if(sudoku.getType()==1){
		
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

int TotalPoss(bool Possibilities[9][9][9], int d, int r1, int c1, int r2, int c2) {
	int suma = 0;
	for (int r = r1; r <= r2; r++) 
		for (int c = c1; c <= c2; c++)
			suma += Possibilities[r][c][d];
	return suma;
}

Sudoku9x9 TryToSolve(Sudoku9x9 sudoku){
	/* starting with unknown sudoku given, function tries to solve it and returns sudoku with status:
	solved, unsolvable, ambiguous or conradictory.
	 */

	// for unknown, classic sudoku
	if( sudoku.getStatus() == 0 && sudoku.getType()==1 ){
		if( IsContradictory(sudoku) ){
			sudoku.setStatus(4); // contradictory
			return sudoku;
		}
		if(sudoku.N_Current() < 17){
			sudoku.setStatus(3); // ambiguous
			return sudoku;
		}

		bool progress = true;
		while(progress){
			progress = false;

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

			// trick 1

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
	}

	return sudoku;
}

int main() {
	
    int tab[9][9] = {
		{ 1,0,6,0,0,0,0,0,0 },
		{ 0,0,7,0,3,2,0,0,0 },
		{ 0,4,0,9,7,0,0,0,0 },
		{ 0,9,0,7,0,0,3,0,0 },
		{ 0,6,0,0,1,0,0,0,5 },
		{ 0,0,0,8,0,0,7,9,0 },
		{ 0,0,0,0,0,0,1,3,0 },
		{ 0,0,8,1,4,0,2,0,0 },
		{ 0,0,4,0,0,0,6,0,0 }
	};

    Sudoku9x9 sudoku(tab);

	cout<<sudoku.N_Current()<<" "<<IsFilled(sudoku)<<" "<<IsContradictory(sudoku)<<"\n";

	sudoku = TryToSolve(sudoku);
	sudoku.PrintToConsole();

	cout<<sudoku.N_Current()<<" "<<IsFilled(sudoku)<<" "<<IsContradictory(sudoku)<<"\n";

	for(int r=0;r<9;r++){
		for(int c=0;c<9;c++){
			cout<<sudoku.N_Possibilities(r,c);
		}
		cout<<endl;
	}

	
	return 0;
}