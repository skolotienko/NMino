#include "AlgorithmXSolver.h"
#include <iostream>

using namespace std;

#pragma warning( disable : 4018 )	//	warning C4018: '<' : signed/unsigned mismatch

CAlgorithmXSolver::CAlgorithmXSolver(const CField& initialGameField, const std::vector<CFigureRotations>& allFigures)
	: CSolverBase(initialGameField, allFigures), figuresCount(allFigures.size())
{
}

//	������� �� ����� �������. ��������, ������� ����� ���� ����� �������! ������������ ��� �������
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
	//	������� ��������� �������
	CDLXMatrix matrix;
	fillMatrix(matrix);
	//printMatrix(matrix);	//	����������������� ��� �������
	//	��������� ��������
	CAlgorithmX algorithm(true);
	algorithm.Init(matrix);
	algorithm.FindSolutions();
	//	�������� � ��������������� �������
	vector<int> solutionIndexes = algorithm.GetSolution();
	CFiguresPlacement solution;
	fillSolution(solutionIndexes, solution);
	return solution;
}

//	��������� ������� ��� ���������
void CAlgorithmXSolver::fillMatrix(CDLXMatrix& matrix)
{
	//	������� � ������� ����� �������� �� ���� ������.
	//	������ N ������� ��� ����, ����� ����� ������, ��������������� ������ ������ (�� �������� �������),
	//	��� N - ���������� �����.
	//	��������� �� N ������� - ������������� ������� ����, ��� �������� - ���� ������ �������� ������, ���� - ���� ���.
	//	���� ������� ������� ������������� ������ ������������ �����-���� ������ �� ����.
	//	����� �������, �������� ������ ������� ����� (figuresCount + cellsCount)x(sum{figure}(allPlacements(figure))
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

//	��������� ������������ �������� ������� � ������� � ������ �� ����
void CAlgorithmXSolver::fillPointIndex()
{
	//	������ figuresCount �������� ������� ������������� �� �������� :)
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

//	�������� ��� ��������� ����� ������ � �������
void CAlgorithmXSolver::addFigure(const CFigure& figure, int figureIndex, CDLXMatrix& matrix)
{
	//	������� ���� ��������� ��������� ������
	for( int y = 0; y + figure.Height() <= initialGameField.size(); y++ ) {
		for( int x = 0; x + figure.Width() <= initialGameField[y].size(); x++ ) {
			CFigurePlacement fp({ &figure, CPoint(x, y) });
			//	���� ������ ����� ��������� � ������ �������, �� ��������� ������� � rowIndexToFigurePlacement
			if( canPlaceFigure(fp) ) {
				addFigurePlacement(fp, figureIndex, matrix);
			}
		}
	}
}

//	����� �� ��������� ��������� ������ � ��������� �����
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

//	�������� ��������� ��������� ������ � �������, �������� rowIndexToFigurePlacement
void CAlgorithmXSolver::addFigurePlacement(const CFigurePlacement& figurePlacement, int figureIndex, CDLXMatrix& matrix)
{
	matrix.push_back(vector<bool>(colIndexToPoint.size(), false));
	vector<bool>& matrixRow = matrix.back();
	//	������ ��������, ��������������� ������ ������
	matrixRow[figureIndex] = true;

	//	������ ��������, ��������������� ������� ������� ������� �� ����
	for( int i = 0; i < figurePlacement.Figure->Points().size(); i++ ) {
		const int y = figurePlacement.Figure->Points()[i].second + figurePlacement.Position.second;
		const int x = figurePlacement.Figure->Points()[i].first + figurePlacement.Position.first;
		const int colIndex = pointToColIndex[CPoint(x, y)];
		matrixRow[colIndex] = true;
	}

	//	����������, ��� � ������ ������� ������� ��������� ��������� ������ � ��������� ���������
	rowIndexToFigurePlacement.push_back(figurePlacement);
}

//	�� ����������� ������� ���������� ���������� ������������ ������������ �����
void CAlgorithmXSolver::fillSolution(const std::vector<int>& solutionIndexes, CFiguresPlacement& solution) const
{
	solution.clear();
	for( int i = 0; i < solutionIndexes.size(); i++ ) {
		CFigurePlacement fp = rowIndexToFigurePlacement[solutionIndexes[i]];
		solution.push_back(fp);
	}
}
