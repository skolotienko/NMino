#include "AlgorithmX.h"
#include <memory>
#include <assert.h>

using namespace std;

#pragma warning( disable : 4018 )	//	warning C4018: '<' : signed/unsigned mismatch

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Dancing list node - ������ �������� �� ������� ����� ������������ ����� ���� � ���� ����������� ���������������
//	�������: �� ����������� � �� ���������, ������ � ������������ ������ ����� ��� ���� �������������� ���� - ���������
//	�������. ����� ���� ����� ���� ���������� ������� - � ����� ������ ��� ����� ��������� ���� Size
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
//	�������� Dancing links (� ����� ��� ��� �������� DLX ��� AlgorithmX) �����.
CAlgorithmX::CAlgorithmX(bool _stopAtFirst) : stopAtFirst(_stopAtFirst)
{
}

CAlgorithmX::~CAlgorithmX()
{
	for( int i = 0; i < allNodes.size(); i++ ) {
		delete allNodes[i];
	}
}

//	���������������� �������� �������� �� 0 � 1
void CAlgorithmX::Init(const std::vector< std::vector<bool> >& dlxMatrix)
{
	//	��������� ������� �������
	int width = 0;
	const int height = dlxMatrix.size();
	for( int y = 0; y < height; y++ ) {
		if( y == 0 ) {
			width = dlxMatrix[y].size();
		} else {
			assert(width == dlxMatrix[y].size());
		}
	}

	//	������ ������ � ��� ��������� �������
	root = new CNode(allNodes);
	for( int x = 0; x < width; x++ ) {
		CNode* columnHeader = new CNode(allNodes);
		columnHeaders.push_back(columnHeader);
		columnHeader->X = x;
		root->LinkL(columnHeader);
	}

	//	������ ���������� �� ���� ��������� � �������
	for( int y = 0; y < height; y++ ) {
		const vector<bool>& matrixRow = dlxMatrix[y];
		//	��������� ���� ��� ���������� ���� ����� � ������
		unique_ptr<CNode> row( new CNode );
		for( int x = 0; x < width; x++ ) {
			if( !matrixRow[x] ) {
				continue;
			}
			//	��������� ������
			CNode* cell = new CNode(allNodes);
			cell->X = x;
			cell->Y = y;
			columnHeaders[x]->LinkU(cell);
			columnHeaders[x]->Size++;
			row->LinkL(cell);
		}
		//	�������� � ������� ��������� ����
		row->HideLR();
	}
}

//	��������� ����� ���� �������. ����� - ���������� ��������� �������
long long CAlgorithmX::FindSolutions()
{
	return dlx();
}

//	����� - ������� �����, ������� ������ ������ ��������(exact cover)
const std::vector<int>& CAlgorithmX::GetSolution() const
{
	return lastSolution;
}

//	������ �������
void CAlgorithmX::coverColumn(CNode* col)
{
	//	������� ���� �������
	col->HideLR();
	//	������� ��� ������ ������� (��� ����)
	for( CNode* row = col->D; row != col; row = row->D ) {
		//	�������� ������ ������ ������ (��� ������)
		for( CNode* cell = row->R; cell != row; cell = cell->R ) {
			cell->HideUD();
			//	��������� ���������� ����� � �������
			columnHeaders[cell->X]->Size--;
		}
	}
}

//	������� �������
void CAlgorithmX::uncoverColumn(CNode* col)
{
	//	������� ����� ������ � �������� �������!
	//	���������� ��� ������ ������� (��� �����)
	for( CNode* row = col->U; row != col; row = row->U ) {
		//	���������� ��� ������ ������� (��� �����)
		for( CNode* cell = row->L; cell != row; cell = cell->L ) {
			cell->ShowUD();
			//	��������� ���������� ����� � �������
			columnHeaders[cell->X]->Size++;
		}
	}
	//	���������� ���� �������
	col->ShowLR();
}

//	����������� ��������.
long long CAlgorithmX::dlx()
{
	//	��������, ��� ��� ������� ���� ������
	if( root->R == root ) {
		//	����� �������!
		lastSolution = stack;
		return 1;
	}

	//	����� ��������� ������� ������� ��� �������� ��, � ������� ������ ����� �����.
	CNode* minCol = root->R;
	int minSize = minCol->Size;
	for( CNode* col = minCol->R; col != root; col = col->R ) {
		if( col->Size < minSize ) {
			minSize = col->Size;
			minCol = col;
		}
	}
	if( minSize < 1 ) {
		//	�� ������� ����� ��������, �.�. �������� ������� ��� �����.
		return 0;
	}

	//	������� ������� ������ � �����-���� ��������� �������� �� �������
	long long solutions = 0;
	//	������� �������
	coverColumn(minCol);
	//	�������� �������
	for( CNode* row = minCol->D; row != minCol; row = row->D ) {
		//	���������� ��������� �������
		stack.push_back(row->Y);
		//	������� ��� ������� �������
		for( CNode* cell = row->R; cell != row; cell = cell->R ) {
			coverColumn(columnHeaders[cell->X]);
		}
		//	���������� ��������
		solutions += dlx();
		//	���� ������� ���������� ��������� - ��������������� ��� ������ ��������� �������
		if( stopAtFirst && solutions > 0 ) {
			return solutions;
		}
		//	���������� ��� ������� ������� � �������� �������
		for( CNode* cell = row->L; cell != row; cell = cell->L ) {
			uncoverColumn(columnHeaders[cell->X]);
		}
		//	��������������� ����
		stack.pop_back();
	}
	//	���������� �������
	uncoverColumn(minCol);
	return solutions;
}
