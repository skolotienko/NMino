#pragma once

#include "SolverBase.h"

class CBruteForceSolver : public CSolverBase {
public:
	CBruteForceSolver(const CField& initialGameField, const std::vector<CFigureRotations>& allFigures);

	//	���������� ���������� CSolverBase
	virtual CFiguresPlacement FindSolution();

private:
	const int width;
	const int height;
	//	������� ������� ����
	CField currentGameField;
	//	������� �������
	CFiguresPlacement solution;
	//	�������������� ������
	std::vector<bool> usedFigures;

	bool placeFiguresRecursively();
	bool placeFigure(int figureIndex, const CFigure& figure, const CPoint& position);
	void removeFigure(int figureIndex, const CFigure& figure, const CPoint& position);

};
