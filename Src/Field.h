#pragma once

#include <vector>
#include <string>

//	����������� �������� ��� ������ � "��������" ������. ������������� �������� ������������� �� ��������� �����
const int EmptyFieldCell = -1;
const int LockedFieldCell = -2;

//	��������� �������, ��������������� ����������� ���������
const char EmptyFieldCellSymbol = '.';
const char LockedFieldCellSymbol = '#';

//	������� ���� ����� � ���� ��������� ������� int-��
//	�������� � ������ - ���� ������ ������, ���� "��������", ���� ������ ������ (������� � ����).
//	TODO: ��-��������, ���� ���� �� ������� ����������� ����� � �������� ���� IsFreeCell, GetCell, SetCell,
//	������������� ��������������� � �.�.
typedef std::vector< std::vector<int> > CField;

//	��������� ���� �� �����
void LoadFieldFromFile(const std::string& fileName, CField& field);

//	���������� ����
void PrintField(const CField& field);
