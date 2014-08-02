#pragma once

#include <vector>

//	��������� ������� �� 0 � 1 (false � true), ������ �������� ������� ����� �����
typedef std::vector< std::vector<bool> > CDLXMatrix;

//	�������� Dancing links (� ����� ��� ��� �������� DLX ��� AlgorithmX) �����.
//	������� �� ���� �� https://github.com/jlaire/dlx-cpp
class CAlgorithmX {
public:
	CAlgorithmX(bool stopAtFirst);
	~CAlgorithmX();

	//	���������������� �������� �������� �� 0 � 1
	void Init(const CDLXMatrix& dlxMatrix);
	//	��������� ����� ���� �������. ����� - ���������� ��������� �������
	long long FindSolutions();
	//	����� - ������� �����, ������� ������ ������ ��������(exact cover)
	const std::vector<int>& GetSolution() const;

private:
	//	Dancing list node
	struct CNode;

	//	�������� ��������� - ����� �� ��������������� �� ������ ��������� �������.
	const bool stopAtFirst;
	//	�������� �������. ����� � ������ �� �� - ��������� �������.
	CNode* root;
	//	��� ��������� �������
	std::vector<CNode*> allNodes;
	//	��������� �������
	std::vector<CNode*> columnHeaders;
	//	���� �� �������������� �����. ���� ����� ������� - ��� ����� ������������ �� �����.
	std::vector<int> stack;
	//	��������� ��������� �������.
	std::vector<int> lastSolution;

	//	������ �������
	void coverColumn(CNode* col);
	//	������� �������
	void uncoverColumn(CNode* col);
	//	����������� ��������.
	long long dlx();

};
