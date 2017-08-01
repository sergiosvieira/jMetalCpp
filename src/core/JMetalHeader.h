#ifndef __JMETAL_HEADER__
#define __JMETAL_HEADER__

#ifdef MSVC
	#ifdef _DEBUG
		#define snew new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
	#endif
#else
	#define snew new
#endif

#include <string>
#include <map>
#include <vector>
#include <ValueBase.h>
#include <Value.h>

class Operator;
class Solution;
class Comparator;
class SolutionSet;

namespace JMetal
{
	using SharedSolution = SharedValue(Solution);
	using SharedComparator = SharedValue(Comparator);
	using SharedOperator = SharedValue(Operator);
	using SharedSolutionSet = SharedValue(SolutionSet);
	using MapOfStringFunct = std::map<std::string, ValuePtr>;
	using MapOfStringOperatorPtr = std::map<std::string, Operator*>;
	using MapOfStringDouble = std::map<std::string, double>;
	using MapOfStringBool = std::map<std::string, bool>;
	using VectorOfSolutionPtr = std::vector<SharedSolution>;
	using VectorOfDouble = std::vector<double>;
	using MatrixOfDouble = std::vector<VectorOfDouble>;	
	using VectorOfInteger = std::vector<int>;
	using MatrixOfInteger = std::vector<VectorOfInteger>;
	using VectorOfBoolean = std::vector<bool>;
	using MatrixOfBoolean = std::vector<VectorOfBoolean>;
	using VectorOfString = std::vector<std::string>;
	using VectorOfValuePtr = std::vector<ValuePtr>;
	using MatrixOfValuePtr = std::vector<VectorOfValuePtr>;
}

#endif