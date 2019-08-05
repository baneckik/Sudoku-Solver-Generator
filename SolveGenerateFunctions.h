#include <vector>
#include "Sudoku9x9.h"
using namespace std;


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

Sudoku9x9 TryToSolveEasy(Sudoku9x9 sudoku){
	/* starting with unknown sudoku given, function tries to solve it and returns sudoku with status:
	solved, unsolvable, ambiguous or conradictory.
	 */

	// for classic sudoku
	if( sudoku.getType()==1 ){
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

Sudoku9x9 TryToSolve(Sudoku9x9 sudoku){
	/* starting with unknown sudoku given, function tries to solve it and returns sudoku with status:
	solved, unsolvable, ambiguous or conradictory.
	 */

	// for classic sudoku
	if( sudoku.getType()==1 ){
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

Sudoku9x9 Solve(Sudoku9x9 sudoku){
	// for classic sudoku
	if( sudoku.getType()==1 ){
		if( IsContradictory(sudoku) ){
			sudoku.setStatus(4); // contradictory
			return sudoku;
		}
		if(sudoku.N_Current() < 17){
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

			// program is looking for rows in which this digit can fit only into exactly 2 places
			if ( !IsFilled(sudoku) ) {
				DifficultyLevel++;
				// for every digit
				for (int d = 0; d < 9; d++) {
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
				}
			}
			// program is looking for columns in which this digit can fit only into exactly 2 places
			if ( !IsFilled(sudoku) ) {
				DifficultyLevel++;
				// for every digit
				for (int d = 0; d < 9; d++) {
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

	return sudoku;

}

Sudoku9x9 Generate(int seed){
	/* Function generates random sudoku using Solve() function.
	The result sudoku may be contradictory. */

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
	Sudoku9x9 sudoku(ZeroGrid), old_sudoku(ZeroGrid);
	srand(seed);
	for(int i=0; i<17; i++) sudoku.InsertRandomDigit();

	int status, iter = 0, max_iter = 20;
	while( iter <= max_iter){
		old_sudoku = sudoku;
		sudoku.InsertRandomDigit();

		sudoku = Solve(sudoku);
		status = sudoku.getStatus();
		if ( status == 1 ){
			Sudoku9x9 sudoku_gen(sudoku.GivenGrid);
			return sudoku_gen;
		} else if(  status == 4 ){
			sudoku = old_sudoku;
		}
		
		iter++;
	}

	// function should never reach this point
	return sudoku;
}
