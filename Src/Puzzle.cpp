#include "Puzzle.h"
#include "AlgorithmXSolver.h"
#include "BruteForceSolver.h"
#include <memory>
//	Нафиг кроссплатформенность :)
#include <windows.h>

#pragma warning( disable : 4018 )	//	warning C4018: '<' : signed/unsigned mismatch

using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Класс, который загружает задачу и решает её
void CPuzzle::Init(const std::string& gameFieldFile, const std::string& figuresDirectory)
{
	LoadFieldFromFile(gameFieldFile, initialGameField);
	loadFiguresFromDirectory(figuresDirectory);
}

void CPuzzle::SolveAndPrintSolution(bool useBruteForce) const
{
	unique_ptr<CSolverBase> solver;
	if( useBruteForce ) {
		solver = make_unique<CBruteForceSolver>(initialGameField, allFigures);
	} else {
		solver = make_unique<CAlgorithmXSolver>(initialGameField, allFigures);
	}
	CFiguresPlacement solution = solver->FindSolution();
	print(solution, initialGameField);
}

void CPuzzle::getFilesFromDirectory(const std::string& directory, std::vector<std::string>& files)
{
	//	Спасибо http://stackoverflow.com/questions/306533/how-do-i-get-a-list-of-files-in-a-directory-in-c
	//	Только студия по-умолчанию использует юникодный WinApi, пришлось выключить юникод для простоты.
	files.clear();
	HANDLE dir;
	WIN32_FIND_DATA file_data;
	if( (dir = FindFirstFile((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE ) {
		return;
	}

	do {
		const string file_name = file_data.cFileName;
		const string full_file_name = directory + "/" + file_name;
		const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
		if( file_name[0] == '.' ) {
			continue;
		}
		if( is_directory ) {
			continue;
		}
		files.push_back(full_file_name);
	} while( FindNextFile(dir, &file_data) );

	FindClose(dir);
}

void CPuzzle::loadFiguresFromDirectory(const std::string& figuresDirectory)
{
	allFigures.clear();
	vector<string> files;
	getFilesFromDirectory(figuresDirectory, files);
	for( int i = 0; i < files.size(); i++ ) {
		CFigure f;
		f.LoadFromFile(files[i]);
		allFigures.push_back(CFigureRotations());
		allFigures.back().InitFromFigure(f);
	}
}

void CPuzzle::print(const CFiguresPlacement& solution, const CField& field)
{
	CField fieldWithFigures = field;
	for( int i = 0; i < solution.size(); i++ ) {
		const CPoints& points = solution[i].Figure->Points();
		const CPoint& position = solution[i].Position;
		for( int j = 0; j < points.size(); j++ ) {
			fieldWithFigures[points[j].second + position.second][points[j].first + position.first] = i;
		}
	}
	PrintField(fieldWithFigures);
}
