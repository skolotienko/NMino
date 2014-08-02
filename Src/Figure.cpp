#include "Figure.h"
#include <algorithm>
#include <fstream>
#include <assert.h>

using namespace std;

#pragma warning( disable : 4018 )	//	warning C4018: '<' : signed/unsigned mismatch

//	“екстовые символы, отсутствию или наличию клетки фигуры
static const char NoFigureSymbol= '.';
static const char FigureSymbol = '@';

CFigure::CFigure() : width(0), height(0)
{
}

CFigure::CFigure(const CPoints& _points) : width(0), height(0)
{
	InitFromPoints(_points);
}

void CFigure::InitFromPoints(const CPoints& _points)
{
	points = _points;
	//	—читаем минимальные и максимальные координаты
	int minX = INT_MAX;
	int minY = INT_MAX;
	int maxX = INT_MIN;
	int maxY = INT_MIN;
	for( int i = 0; i < points.size(); i++ ) {
		minX = min(minX, points[i].first);
		minY = min(minY, points[i].second);
		maxX = max(maxX, points[i].first);
		maxY = max(maxY, points[i].second);
	}
	//	ѕриводим минимальную координату к (0, 0)
	for( int i = 0; i < points.size(); i++ ) {
		points[i].first -= minX;
		points[i].second -= minY;
	}
	//	—читаем ширину и высоту
	width = maxX - minX + 1;
	height = maxY - minY + 1;
	//	“очки будем держать отсортированными, чтобы ускорить операцию сравнени€
	sort(points.begin(), points.end());
}

void CFigure::LoadFromFile(const string& file)
{
	ifstream input(file);
	string line;
	CPoints rawPoints;
	int y = 0;
	while( getline(input, line) ) {
		for( int x = 0; x < line.size(); x++ ) {
			if( line[x] == FigureSymbol ) {
				rawPoints.push_back(CPoint(x, y));
			} else {
				//	ѕроверка на формат
				assert(line[x] == NoFigureSymbol);
			}
		}
		y++;
	}
	InitFromPoints(rawPoints);
}

bool CFigure::operator==(const CFigure& other) const
{
	if( points.size() != other.points.size() ) {
		return false;
	}
	//	“.к. точки отсортированы, то можем идти по двум массивам синхронно, пока не найдЄм несовпадение
	for( int i = 0; i < points.size(); i++ ) {
		if( points[i] != other.points[i] ) {
			return false;
		}
	}
	return true;
}

void CFigureRotations::InitFromFigure(const CFigure& figure)
{
	fillRotations(figure);
	removeDuplicates();
}

void CFigureRotations::fillRotations(const CFigure& figure)
{
	//	¬се вращени€ получаютс€ путЄм перебора комбинаций из транспонировани€ и отражени€ по горизонтали/вертикали
	for( int transpose = 0; transpose <= 1; transpose++ ) {
		for( int vertMirror = 0; vertMirror <= 1; vertMirror++ ) {
			for( int horzMirror = 0; horzMirror <= 1; horzMirror++ ) {
				//	ƒл€ каждой комбинации собираем преобразованный массив из точек и складываем в rotations
				CPoints rotatedPoints;
				for( int i = 0; i < figure.Points().size(); i++ ) {
					CPoint p = figure.Points().at(i);
					if( horzMirror ) {
						p.first = figure.Width() - p.first - 1;
					}
					if( vertMirror ) {
						p.second = figure.Height() - p.second - 1;
					}
					if( transpose ) {
						swap(p.first, p.second);
					}
					rotatedPoints.push_back(p);
				}
				rotations.push_back(rotatedPoints);
			}
		}
	}
}

void CFigureRotations::removeDuplicates()
{
	for( int i = rotations.size() - 1; i >= 0; i-- ) {
		for( int j = i - 1; j >= 0; j-- ) {
			if( rotations[i] == rotations[j] ) {
				//	Ќашли дубликат - удал€ем его и переходим к следующему
				rotations.erase(rotations.begin() + i);
				break;
			}
		}
	}
}
