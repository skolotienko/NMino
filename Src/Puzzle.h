#pragma once

#include "Field.h"
#include "Figure.h"
#include <vector>
#include <string>

//	�����, ������� ��������� ������ � ������ �
class CPuzzle {
public:
	void Init(const std::string& gameFieldFile, const std::string& figuresDirectory);
	void SolveAndPrintSolution(bool useBruteForce) const;

private:
	//	������� ����
	CField initialGameField;
	//	��� ��������� ������ � �� ��������� ��������.
	//	TODO: ����� ������ ���������� ������ �� ������� ������ � ��������� ��, ��������� ����������
	std::vector<CFigureRotations> allFigures;

	static void getFilesFromDirectory(const std::string& directory, std::vector<std::string>& files);
	void loadFiguresFromDirectory(const std::string& figuresDirectory);

	static void print(const CFiguresPlacement& solution, const CField& field);

};
