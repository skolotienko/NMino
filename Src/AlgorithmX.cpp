#include "AlgorithmX.h"
#include <memory>
#include <assert.h>

using namespace std;

#pragma warning( disable : 4018 )	//	warning C4018: '<' : signed/unsigned mismatch

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Dancing list node - каждая единичка из матрицы будет представлять собой узел в двух зацикленных двунаправленных
//	списках: по горизонтали и по вертикали, причем в вертикальном списке будет ещё один дополнительный узел - заголовок
//	колонки. Также узел может быть заголовком колонки - в таком случае ещё будет заполнено поле Size
struct CAlgorithmX::CNode {
	CNode* L;	//	Left
	CNode* R;	//	Right
	CNode* U;	//	Up
	CNode* D;	//	Down
	CNode* C;	//	Column header
	int Size;	//	Size
	int X;	//	X coordinate in matrix
	int Y;	//	Y coordinate in matrix

	void HideLR() { L->R = R, R->L = L; }
	void ShowLR() { L->R = R->L = this; }
	void HideUD() { U->D = D, D->U = U; }
	void ShowUD() { U->D = D->U = this; }
	void LinkL(CNode* n) { n->R = this, n->L = L; L = L->R = n; }
	void LinkU(CNode* n) { n->D = this, n->U = U; U = U->D = n; }

	CNode() : L(this), R(this), U(this), D(this), C(0), Size(0), X(-1), Y(-1) {}
	CNode(vector<CNode*>& allNodes) : L(this), R(this), U(this), D(this), C(0), Size(0), X(-1), Y(-1) { allNodes.push_back(this); }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Алгоритм Dancing links (а также его ещё называют DLX или AlgorithmX) Кнута.
CAlgorithmX::CAlgorithmX(bool _stopAtFirst) : stopAtFirst(_stopAtFirst)
{
}

CAlgorithmX::~CAlgorithmX()
{
	for( int i = 0; i < allNodes.size(); i++ ) {
		delete allNodes[i];
	}
}

//	Инициализировать алгоритм матрицей из 0 и 1
void CAlgorithmX::Init(const std::vector< std::vector<bool> >& dlxMatrix)
{
	//	Проверяем размеры матрицы
	int width = 0;
	const int height = dlxMatrix.size();
	for( int y = 0; y < height; y++ ) {
		if( y == 0 ) {
			width = dlxMatrix[y].size();
		} else {
			assert(width == dlxMatrix[y].size());
		}
	}

	//	Создаём корень и все заголовки колонок
	root = new CNode(allNodes);
	for( int x = 0; x < width; x++ ) {
		CNode* columnHeader = new CNode(allNodes);
		columnHeaders.push_back(columnHeader);
		columnHeader->X = x;
		root->LinkL(columnHeader);
	}

	//	Теперь проходимся по всем единичкам в таблице
	for( int y = 0; y < height; y++ ) {
		const vector<bool>& matrixRow = dlxMatrix[y];
		//	Временный узел для связывания всех ячеек в строке
		unique_ptr<CNode> row( new CNode );
		for( int x = 0; x < width; x++ ) {
			if( !matrixRow[x] ) {
				continue;
			}
			//	Добавляем ячейку
			CNode* cell = new CNode(allNodes);
			cell->X = x;
			cell->Y = y;
			columnHeaders[x]->LinkU(cell);
			columnHeaders[x]->Size++;
			row->LinkL(cell);
		}
		//	Скрываем и удаляем временный узел
		row->HideLR();
	}
}

//	Запустить поиск всех решений. Ответ - количество найденных решений
long long CAlgorithmX::FindSolutions()
{
	return dlx();
}

//	Ответ - индексы строк, которые делают полное покрытие(exact cover)
const std::vector<int>& CAlgorithmX::GetSolution() const
{
	return lastSolution;
}

//	Убрать колонку
void CAlgorithmX::coverColumn(CNode* col)
{
	//	Убираем саму колонку
	col->HideLR();
	//	Убираем все строки колонки (идём вниз)
	for( CNode* row = col->D; row != col; row = row->D ) {
		//	Скрываем каждую ячейку строки (идём вправо)
		for( CNode* cell = row->R; cell != row; cell = cell->R ) {
			cell->HideUD();
			//	Обновляем количество ячеек в колонке
			columnHeaders[cell->X]->Size--;
		}
	}
}

//	Вернуть колонку
void CAlgorithmX::uncoverColumn(CNode* col)
{
	//	Возврат будем делать в обратном порядке!
	//	Возвращаем все строки колонки (идём вверх)
	for( CNode* row = col->U; row != col; row = row->U ) {
		//	Возвращаем все ячейки колонки (идём влево)
		for( CNode* cell = row->L; cell != row; cell = cell->L ) {
			cell->ShowUD();
			//	Обновляем количество ячеек в колонке
			columnHeaders[cell->X]->Size++;
		}
	}
	//	Возвращаем саму колонку
	col->ShowLR();
}

//	Рекурсивный алгоритм.
long long CAlgorithmX::dlx()
{
	//	Проверка, что все колонки были убраны
	if( root->R == root ) {
		//	Нашли решение!
		lastSolution = stack;
		return 1;
	}

	//	Среди оставшися холонок выберем для удаления ту, в которой меньше всего узлов.
	CNode* minCol = root->R;
	int minSize = minCol->Size;
	for( CNode* col = minCol->R; col != root; col = col->R ) {
		if( col->Size < minSize ) {
			minSize = col->Size;
			minCol = col;
		}
	}
	if( minSize < 1 ) {
		//	Не удалось найти покрытие, т.к. остались колонки без ячеек.
		return 0;
	}

	//	Удаляем колонку вместе с какой-либо выбранной строчкой из колонки
	long long solutions = 0;
	//	Удаляем колонку
	coverColumn(minCol);
	//	Выбираем строчку
	for( CNode* row = minCol->D; row != minCol; row = row->D ) {
		//	Запоминаем выбранную строчку
		stack.push_back(row->Y);
		//	Удаляем все колонки строчки
		for( CNode* cell = row->R; cell != row; cell = cell->R ) {
			coverColumn(columnHeaders[cell->X]);
		}
		//	Продолжаем рекурсию
		solutions += dlx();
		//	Если указано параметром алгоритма - останавливаемся при первом найденном решении
		if( stopAtFirst && solutions > 0 ) {
			return solutions;
		}
		//	Возвращаем все колонки строчки в обратном порядке
		for( CNode* cell = row->L; cell != row; cell = cell->L ) {
			uncoverColumn(columnHeaders[cell->X]);
		}
		//	Восстанавливаем стэк
		stack.pop_back();
	}
	//	Возвращаем колонку
	uncoverColumn(minCol);
	return solutions;
}
