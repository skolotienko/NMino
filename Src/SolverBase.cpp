#include "SolverBase.h"

using namespace std;

#pragma warning( disable : 4018 )	//	warning C4018: '<' : signed/unsigned mismatch

CSolverBase::CSolverBase(const CField& _initialGameField, const std::vector<CFigureRotations>& _allFigures)
	: initialGameField(_initialGameField), allFigures(_allFigures)
{
}

CSolverBase::~CSolverBase()
{
}
