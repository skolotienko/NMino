#pragma once

#include <vector>

//	Двумерная матрица из 0 и 1 (false и true), точное покрытие которой нужно найти
typedef std::vector< std::vector<bool> > CDLXMatrix;

//	Алгоритм Dancing links (а также его ещё называют DLX или AlgorithmX) Кнута.
//	Основан на коде из https://github.com/jlaire/dlx-cpp
class CAlgorithmX {
public:
	CAlgorithmX(bool stopAtFirst);
	~CAlgorithmX();

	//	Инициализировать алгоритм матрицей из 0 и 1
	void Init(const CDLXMatrix& dlxMatrix);
	//	Запустить поиск всех решений. Ответ - количество найденных решений
	long long FindSolutions();
	//	Ответ - индексы строк, которые делают полное покрытие(exact cover)
	const std::vector<int>& GetSolution() const;

private:
	//	Dancing list node
	struct CNode;

	//	Параметр алгоритма - нужно ли останавливаться на первом найденном решении.
	const bool stopAtFirst;
	//	Корневая вершина. Слева и справа от неё - заголовки колонок.
	CNode* root;
	//	Все созданные вершины
	std::vector<CNode*> allNodes;
	//	Заголовки колонок
	std::vector<CNode*> columnHeaders;
	//	Стэк из использованных строк. Если нашли решение - его можно восстановить по стэку.
	std::vector<int> stack;
	//	Последнее найденное решение.
	std::vector<int> lastSolution;

	//	Убрать колонку
	void coverColumn(CNode* col);
	//	Вернуть колонку
	void uncoverColumn(CNode* col);
	//	Рекурсивный алгоритм.
	long long dlx();

};
