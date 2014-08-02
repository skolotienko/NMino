#include "BruteForceSolver.h"

using namespace std;

#pragma warning( disable : 4018 )	//	warning C4018: '<' : signed/unsigned mismatch

CBruteForceSolver::CBruteForceSolver(const CField& initialGameField, const std::vector<CFigureRotations>& allFigures)
	: CSolverBase(initialGameField, allFigures), width(initialGameField[0].size()), height(initialGameField.size())
{
	usedFigures.assign(allFigures.size(), false);
}

CFiguresPlacement CBruteForceSolver::FindSolution()
{
	currentGameField = initialGameField;
	placeFiguresRecursively();
	return solution;
}

//	TODO: ������ ��� ��������� ��������� �������. � ��������, ����� ��������.
bool CBruteForceSolver::placeFiguresRecursively()
{
	if( find(usedFigures.begin(), usedFigures.end(), false) == usedFigures.end() ) {
		//	��� ������ ���������. ����� �������, ��� �� ����� �������! :)
		return true;
	}

	for( int figureIndex = 0; figureIndex < usedFigures.size(); figureIndex++ ) {
		if( usedFigures[figureIndex] ) {
			//	������ ��� ������������.
			continue;
		}
		const CFigures& rotations = allFigures[figureIndex].Rotations();
		//	������� ����� - ������� ���������, ������ ���� - ������� ��������
		for( int y = 0; y < height; y++ ) {
			for( int x = 0; x < width; x++ ) {
				const CPoint position(x, y);
				for( int rotationIndex = 0; rotationIndex < rotations.size(); rotationIndex++ ) {
					const CFigure& figure = rotations[rotationIndex];
					if( placeFigure(figureIndex, figure, position) ) {
						//	������� ��������� ������ - ������ �� ��������� ������� ��������,
						//	����� ��������� ����� ������.
						if( placeFiguresRecursively() ) {
							//	����� �������!
							return true;
						}
						//	�� ����� ������� - ������� �����.
						removeFigure(figureIndex, figure, position);
					}
				}
			}
		}
	}
	//	���� �� ����� �� ����, �� �� ���� �� ������� �� ���� ������.
	return false;
}

//	�������� ��������� ������. true - ���� �������, false - ���� ��������� ������.
//	��� ������ ����� ��������� currentGameField, usedFigures � solution
bool CBruteForceSolver::placeFigure(int figureIndex, const CFigure& figure, const CPoint& position)
{
	//	������� ���������, ��� ������ ������ ����� ���������
	for( int i = 0; i < figure.Points().size(); i++ ) {
		const int x = figure.Points()[i].first + position.first;
		if( x < 0 || x >= width ) {
			return false;
		}
		const int y = figure.Points()[i].second + position.second;
		if( y < 0 || y >= height ) {
			return false;
		}
		if( currentGameField[y][x] != EmptyFieldCell ) {
			return false;
		}
	}
	//	���� �� ����� �� ����, �� ������ ����� ��������� ������
	for( int i = 0; i < figure.Points().size(); i++ ) {
		const CPoint& point = figure.Points()[i];
		currentGameField[point.second + position.second][point.first + position.first] = figureIndex;
	}
	usedFigures[figureIndex] = true;
	solution.push_back({ &figure, position });
	return true;
}

//	������� ����� ������������ � ������� placeFigure ������, ���������
//	� ���������� ��������� currentGameField, usedFigures � solution
void CBruteForceSolver::removeFigure(int figureIndex, const CFigure& figure, const CPoint& position)
{
	for( int i = 0; i < figure.Points().size(); i++ ) {
		const CPoint& point = figure.Points()[i];
		currentGameField[point.second + position.second][point.first + position.first] = EmptyFieldCell;
	}
	usedFigures[figureIndex] = false;
	solution.pop_back();
}
