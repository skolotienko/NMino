#pragma once

#include <vector>
#include "Field.h"
#include "Figure.h"

//	����������� �������� �����������
class CSolverBase {
public:
	CSolverBase(const CField& initialGameField, const std::vector<CFigureRotations>& allFigures);
	virtual ~CSolverBase();

	//	����� ���� �� ���� �������. ����� ������� ������ ���������, ���� ������� ���.
	virtual CFiguresPlacement FindSolution() = 0;

protected:
	//	����������� ������� ���� - ������ ���� ������ ������ ��� "��������" ������
	const CField& initialGameField;
	//	��� ��������� ������, ������� ���������� ����������
	const std::vector<CFigureRotations>& allFigures;

};
