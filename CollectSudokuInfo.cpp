
#include <iostream>
#include <fstream>
#include "SolveGenerateFunctions.h"
using namespace std;

int main() {
	
    int tab[9][9] = {
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

	ofstream myfile;
	myfile.open ("GeneratingSudokuInfo.txt");
	myfile << "seed N_Given status difficulty\n";
	
	Sudoku9x9 sudoku(tab);
	for( int i=1; i<=1000; i++){
		sudoku = Generate(i);
		Sudoku9x9 sudoku2(sudoku.GivenGrid);
		sudoku2 = Solve(sudoku2);
		myfile << i << " " << sudoku2.N_Given()<< " "  << sudoku2.getStatus();
		myfile << " " << sudoku2.getDifficulty() << "\n";
	}

	myfile.close();

	return 0;
}