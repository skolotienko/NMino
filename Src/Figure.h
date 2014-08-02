#pragma once

#include <vector>
#include <string>

//	Точка в двумерном пространстве
//	TODO: По-хорошему, для них тоже лучше завести отдельный класс, а то сейчас
//	вместо point.x и point.y приходится писать point.first и point.second
typedef std::pair<int, int> CPoint;
//	Последовательность точек
typedef std::vector<CPoint> CPoints;

//	Одна фигура - это множество точек, отсортированное по горизонтали+вертикали,
//	с посчитанными шириной и высотой, а также с минимальной координатой (0, 0)
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

//	Множество фигур
typedef std::vector<CFigure> CFigures;

//	Фигура со всеми её поворотами без дубликатов.
class CFigureRotations {
public:
	//	Из одной формы фигуры создаёт все её вращения/отражения, устраняя дубликаты
	void InitFromFigure(const CFigure& figure);

	const CFigures& Rotations() const { return rotations; }

private:
	CFigures rotations;

	void fillRotations(const CFigure& figure);
	void removeDuplicates();

};

//	Фигура с положением на поле - ссылка на фигуру с какой-то позицией в двумерном пространстве. 
struct CFigurePlacement {
	const CFigure* Figure;
	CPoint Position;
};

//	Последовательность фигур с положением
typedef std::vector<CFigurePlacement> CFiguresPlacement;
