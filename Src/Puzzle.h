#pragma once

#include "Field.h"
#include "Figure.h"
#include <vector>
#include <string>

//	Класс, который загружает задачу и решает её
class CPuzzle {
public:
	void Init(const std::string& gameFieldFile, const std::string& figuresDirectory);
	void SolveAndPrintSolution(bool useBruteForce) const;

private:
	//	Игровое поле
	CField initialGameField;
	//	Все доступные фигуры и их возможные повороты.
	//	TODO: Можно искать одинаковые фигуры во входных данных и сокращать их, запоминая количество
	std::vector<CFigureRotations> allFigures;

	static void getFilesFromDirectory(const std::string& directory, std::vector<std::string>& files);
	void loadFiguresFromDirectory(const std::string& figuresDirectory);

	static void print(const CFiguresPlacement& solution, const CField& field);

};
