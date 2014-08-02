#pragma once

#include "SolverBase.h"
#include "AlgorithmX.h"
#include <map>

class CAlgorithmXSolver : public CSolverBase {
public:
	CAlgorithmXSolver(const CField& initialGameField, const std::vector<CFigureRotations>& allFigures);

	//	���������� ���������� CSolverBase
	virtual CFiguresPlacement FindSolution();

private:
	//	����� ���������� ����� � allFigures
	const int figuresCount;
	//	������������ ������� ������� � ������� � ��������� ���������� ������
	std::vector<CFigurePlacement> rowIndexToFigurePlacement;
	//	������������ ������� ������� � ������� � ����� �� ����
	std::vector<CPoint> colIndexToPoint;
	std::map<CPoint, int> pointToColIndex;

	//	��������� ������� ��� ���������
	void fillMatrix(CDLXMatrix& matrix);
	//	��������� ������������ �������� ������� � ������� � ������ �� ����
	void fillPointIndex();
	//	�������� ��� ��������� ����� ������ � �������
	void addFigure(const CFigure& figure, int figureIndex, CDLXMatrix& matrix);
	//	����� �� ��������� ��������� ������ � ��������� �����
	bool canPlaceFigure(const CFigurePlacement& figurePlacement) const;
	//	�������� ��������� ��������� ������ � �������, �������� rowIndexToFigurePlacement
	void addFigurePlacement(const CFigurePlacement& figurePlacement, int figureIndex, CDLXMatrix& matrix);
	//	�� ����������� ������� ���������� ���������� ������������ ������������ �����
	void fillSolution(const std::vector<int>& solutionIndexes, CFiguresPlacement& solution) const;

};
