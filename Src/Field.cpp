#include "Field.h"
#include <fstream>
#include <iostream>
#include <assert.h>

using namespace std;

#pragma warning( disable : 4018 )	//	warning C4018: '<' : signed/unsigned mismatch

void LoadFieldFromFile(const std::string& gameFieldFile, CField& field)
{
	field.clear();
	ifstream input(gameFieldFile);
	string line;
	while( getline(input, line) ) {
		field.push_back(vector<int>());
		for( int i = 0; i < line.size(); i++ ) {
			switch( line[i] ) {
				case EmptyFieldCellSymbol:
					field.back().push_back(EmptyFieldCell);
					break;
				case LockedFieldCellSymbol:
					field.back().push_back(LockedFieldCell);
					break;
				default:
					//	Встретили неизвестный символ
					assert(false);
			}
		}
	}
}

void PrintField(const CField& field)
{
	for( int y = 0; y < field.size(); y++ ) {
		for( int x = 0; x < field[y].size(); x++ ) {
			const int cell = field[y][x];
			if( cell == EmptyFieldCell ) {
				cout << EmptyFieldCellSymbol;
			} else if( cell == LockedFieldCell ) {
				cout << LockedFieldCellSymbol;
			} else if( cell >= 0 && cell <= 26 ) {
				cout << static_cast<char>('A' + cell);
			} else if( cell > 26 ) {
				//	Буквы кончились
				cout << '?';
			} else {
				//	Какая-то ошибка в данных
				assert(false);
			}
		}
		cout << endl;
	}
}
