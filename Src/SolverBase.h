#pragma once

#include <vector>
#include "Field.h"
#include "Figure.h"

//	Абстрактный решатель головоломки
class CSolverBase {
public:
	CSolverBase(const CField& initialGameField, const std::vector<CFigureRotations>& allFigures);
	virtual ~CSolverBase();

	//	Найти хотя бы одно решение. Может вернуть пустой результат, если решения нет.
	virtual CFiguresPlacement FindSolution() = 0;

protected:
	//	Изначальное игровое поле - должны быть только пустые или "закрытые" клетки
	const CField& initialGameField;
	//	Все доступные фигуры, которые необходимо разместить
	const std::vector<CFigureRotations>& allFigures;

};
