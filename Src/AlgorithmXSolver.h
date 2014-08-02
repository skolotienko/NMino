#pragma once

#include "SolverBase.h"
#include "AlgorithmX.h"
#include <map>

class CAlgorithmXSolver : public CSolverBase {
public:
	CAlgorithmXSolver(const CField& initialGameField, const std::vector<CFigureRotations>& allFigures);

	//	Реализация интерфейса CSolverBase
	virtual CFiguresPlacement FindSolution();

private:
	//	Общее количество фигур в allFigures
	const int figuresCount;
	//	Соответствие индекса строчки в матрице к положению конкретной фигуры
	std::vector<CFigurePlacement> rowIndexToFigurePlacement;
	//	Соответствие индекса колонки в матрице к точке на поле
	std::vector<CPoint> colIndexToPoint;
	std::map<CPoint, int> pointToColIndex;

	//	Заполнить матрицу для алгоритма
	void fillMatrix(CDLXMatrix& matrix);
	//	Заполнить соответствие индексов колонок в матрице к точкам на поле
	void fillPointIndex();
	//	Добавить все положения одной фигуры в матрицу
	void addFigure(const CFigure& figure, int figureIndex, CDLXMatrix& matrix);
	//	Можно ли поставить выбранную фигуру в выбранном месте
	bool canPlaceFigure(const CFigurePlacement& figurePlacement) const;
	//	Добавить возможное положение фигуры в матрицу, обновить rowIndexToFigurePlacement
	void addFigurePlacement(const CFigurePlacement& figurePlacement, int figureIndex, CDLXMatrix& matrix);
	//	Из полученного решения алгоритмом результата восстановить расположение фигур
	void fillSolution(const std::vector<int>& solutionIndexes, CFiguresPlacement& solution) const;

};
