#pragma once

#include <vector>
#include <string>

//	Специальные значения для пустых и "закрытых" клеток. Положительные значения забронированы за индексами фигур
const int EmptyFieldCell = -1;
const int LockedFieldCell = -2;

//	Текстовые символы, соответствующие специальным значениям
const char EmptyFieldCellSymbol = '.';
const char LockedFieldCellSymbol = '#';

//	Игровое поле будет в виде двумерной матрицы int-ов
//	Значение в клетке - либо ячейка пустая, либо "закрытая", либо индекс фигуры (начиная с нуля).
//	TODO: По-хорошему, надо было бы сделать полноценный класс с методами типа IsFreeCell, GetCell, SetCell,
//	гарантировать прямоугольность и т.п.
typedef std::vector< std::vector<int> > CField;

//	Загрузить поле из файла
void LoadFieldFromFile(const std::string& fileName, CField& field);

//	Нарисовать поле
void PrintField(const CField& field);
