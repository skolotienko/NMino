#include "AlgorithmXSolver.h"
#include <iostream>

using namespace std;

#pragma warning( disable : 4018 )	//	warning C4018: '<' : signed/unsigned mismatch

CAlgorithmXSolver::CAlgorithmXSolver(const CField& initialGameField, const std::vector<CFigureRotations>& allFigures)
	: CSolverBase(initialGameField, allFigures), figuresCount(allFigures.size())
{
}

//	¬ывести на экран матрицу. ¬нимание, матрица может быть очень большой! »спользовать дл€ отладки
static void printMatrix(const CDLXMatrix& matrix)
{
	for( int y = 0; y < matrix.size(); y++ ) {
		for( int x = 0; x < matrix[y].size(); x++ ) {
			if( matrix[y][x] ) {
				cout << "1";
			} else {
				cout << "0";
			}
		}
		cout << endl;
	}
}

CFiguresPlacement CAlgorithmXSolver::FindSolution()
{
	//	—начала заполн€ем матрицу
	CDLXMatrix matrix;
	fillMatrix(matrix);
	//printMatrix(matrix);	//	–аскомментировать дл€ отладки
	//	«апускаем алгоритм
	CAlgorithmX algorithm(true);
	algorithm.Init(matrix);
	algorithm.FindSolutions();
	//	ѕолучаем и преобразовываем решение
	vector<int> solutionIndexes = algorithm.GetSolution();
	CFiguresPlacement solution;
	fillSolution(solutionIndexes, solution);
	return solution;
}

//	«аполнить матрицу дл€ алгоритма
void CAlgorithmXSolver::fillMatrix(CDLXMatrix& matrix)
{
	//	—трочка в матрице будет состо€ть из двух частей.
	//	ѕервые N колонок все нули, кроме одной €чейки, соответствующей номеру фигуре (не различа€ поворот),
	//	где N - количество фигур.
	//	—ледующие за N колонки - соответствуют €чейкам пол€, где единички - если фигура занимает €чейку, ноль - если нет.
	//	ќдна строчка матрицы соответствует одному расположению какой-либо фигуры на поле.
	//	“аким образом, итоговый размер матрицы будет (figuresCount + cellsCount)x(sum{figure}(allPlacements(figure))
	fillPointIndex();
	rowIndexToFigurePlacement.clear();
	for( int figureIndex = 0; figureIndex < allFigures.size(); figureIndex++ ) {
		const CFigures& rotations = allFigures[figureIndex].Rotations();
		for( int rotationIndex = 0; rotationIndex < rotations.size(); rotationIndex++ ) {
			const CFigure& figureRotation = rotations[rotationIndex];
			addFigure(figureRotation, figureIndex, matrix);
		}
	}
}

//	«аполнить соответствие индексов колонок в матрице к точкам на поле
void CAlgorithmXSolver::fillPointIndex()
{
	//	ѕервые figuresCount индексов колонок забронированы за фигурами :)
	colIndexToPoint.assign(figuresCount, CPoint(-1, -1));

	for( int y = 0; y < initialGameField.size(); y++ ) {
		for( int x = 0; x < initialGameField[y].size(); x++ ) {
			const int cell = initialGameField[y][x];
			if( cell != EmptyFieldCell ) {
				continue;
			}
			CPoint p(x, y);
			const int colIndex = colIndexToPoint.size();
			pointToColIndex[p] = colIndex;
			colIndexToPoint.push_back(p);
		}
	}
}

//	ƒобавить все положени€ одной фигуры в матрицу
void CAlgorithmXSolver::addFigure(const CFigure& figure, int figureIndex, CDLXMatrix& matrix)
{
	//	ѕеребор всех возможных положений фигуры
	for( int y = 0; y + figure.Height() <= initialGameField.size(); y++ ) {
		for( int x = 0; x + figure.Width() <= initialGameField[y].size(); x++ ) {
			CFigurePlacement fp({ &figure, CPoint(x, y) });
			//	≈сли фигуру можно поставить в данную позицию, то обновл€ем матрицу и rowIndexToFigurePlacement
			if( canPlaceFigure(fp) ) {
				addFigurePlacement(fp, figureIndex, matrix);
			}
		}
	}
}

//	ћожно ли поставить выбранную фигуру в выбранном месте
bool CAlgorithmXSolver::canPlaceFigure(const CFigurePlacement& figurePlacement) const
{
	const CPoints& points = figurePlacement.Figure->Points();
	for( int i = 0; i < points.size(); i++ ) {
		const int y = points[i].second + figurePlacement.Position.second;
		if( y < 0 || y >= initialGameField.size() ) {
			return false;
		}

		const int x = points[i].first + figurePlacement.Position.first;
		if( x < 0 || x >= initialGameField[y].size() ) {
			return false;
		}

		if( initialGameField[y][x] != EmptyFieldCell ) {
			return false;
		}
	}
	return true;
}

//	ƒобавить возможное положение фигуры в матрицу, обновить rowIndexToFigurePlacement
void CAlgorithmXSolver::addFigurePlacement(const CFigurePlacement& figurePlacement, int figureIndex, CDLXMatrix& matrix)
{
	matrix.push_back(vector<bool>(colIndexToPoint.size(), false));
	vector<bool>& matrixRow = matrix.back();
	//	—тавим единичку, соответствующую номеру фигуры
	matrixRow[figureIndex] = true;

	//	—тавим единички, соответствующие зан€тым фигурой клеткам на поле
	for( int i = 0; i < figurePlacement.Figure->Points().size(); i++ ) {
		const int y = figurePlacement.Figure->Points()[i].second + figurePlacement.Position.second;
		const int x = figurePlacement.Figure->Points()[i].first + figurePlacement.Position.first;
		const int colIndex = pointToColIndex[CPoint(x, y)];
		matrixRow[colIndex] = true;
	}

	//	«апоминаем, что в данной строчке матрицы находитс€ выбранна€ фигура в выбранном положении
	rowIndexToFigurePlacement.push_back(figurePlacement);
}

//	»з полученного решени€ алгоритмом результата восстановить расположение фигур
void CAlgorithmXSolver::fillSolution(const std::vector<int>& solutionIndexes, CFiguresPlacement& solution) const
{
	solution.clear();
	for( int i = 0; i < solutionIndexes.size(); i++ ) {
		CFigurePlacement fp = rowIndexToFigurePlacement[solutionIndexes[i]];
		solution.push_back(fp);
	}
}
