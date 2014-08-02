#pragma once

#include <vector>
#include <string>

//	����� � ��������� ������������
//	TODO: ��-��������, ��� ��� ���� ����� ������� ��������� �����, � �� ������
//	������ point.x � point.y ���������� ������ point.first � point.second
typedef std::pair<int, int> CPoint;
//	������������������ �����
typedef std::vector<CPoint> CPoints;

//	���� ������ - ��� ��������� �����, ��������������� �� �����������+���������,
//	� ������������ ������� � �������, � ����� � ����������� ����������� (0, 0)
class CFigure {
public:
	CFigure();
	CFigure(const CPoints& points);

	void InitFromPoints(const CPoints& points);

	const CPoints& Points() const { return points; }
	int Width() const { return width; }
	int Height() const { return height;  }

	void LoadFromFile(const std::string& file);

	bool operator==(const CFigure& other) const;

private:
	CPoints points;
	int width;
	int height;

};

//	��������� �����
typedef std::vector<CFigure> CFigures;

//	������ �� ����� � ���������� ��� ����������.
class CFigureRotations {
public:
	//	�� ����� ����� ������ ������ ��� � ��������/���������, �������� ���������
	void InitFromFigure(const CFigure& figure);

	const CFigures& Rotations() const { return rotations; }

private:
	CFigures rotations;

	void fillRotations(const CFigure& figure);
	void removeDuplicates();

};

//	������ � ���������� �� ���� - ������ �� ������ � �����-�� �������� � ��������� ������������. 
struct CFigurePlacement {
	const CFigure* Figure;
	CPoint Position;
};

//	������������������ ����� � ����������
typedef std::vector<CFigurePlacement> CFiguresPlacement;
