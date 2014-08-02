#include "Puzzle.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	if( argc != 3 ) {
		cout << "Usage: NMino.exe <game field file> <folder with figure files>\r\n";
		return 0;
	}

	CPuzzle puzzle;
	puzzle.Init(argv[1], argv[2]);
	puzzle.SolveAndPrintSolution(false);

	return 0;
}
