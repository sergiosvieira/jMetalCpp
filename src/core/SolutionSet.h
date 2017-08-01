//  SolutionSet.h
//
//  Author:
//       Esteban López-Camacho <esteban@lcc.uma.es>
//       Antonio J. Nebro <antonio@lcc.uma.es>
//       Juan J. Durillo <durillo@lcc.uma.es>
//
//  Copyright (c) 2011 Antonio J. Nebro, Juan J. Durillo
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __SOLUTION_SET__
#define __SOLUTION_SET__

#include "JMetalHeader.h"
#include <string>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <stddef.h>
#include <Solution.h>
#include <Comparator.h>

using namespace JMetal;

//class Solution ;
class Comparator;

/**
 * @class SolutionSet
 * @brief This class is aimed at representing a solution set
**/
class SolutionSet
{

protected:
	VectorOfSolutionPtr solutionsList_;
    int capacity_;

public:

    SolutionSet();
    SolutionSet(int maximumSize);
    ~SolutionSet();

    bool add(ValuePtr solution);
    bool add(int index, ValuePtr solution);
	ValuePtr get(int index);
    int getMaxSize();
    void sort(SharedComparator comparator);
    int indexBest(SharedComparator comparator);
	ValuePtr best(SharedComparator comparator);
    int indexWorst(SharedComparator comparator);
	ValuePtr worst(SharedComparator comparator);
    int size();
    void printObjectivesToFile(std::string path);
    void printObjectivesToFile(std::string path, bool append);
    void printVariablesToFile(std::string file);
    void printVariablesToFile(std::string file, bool append);
    void clear();
    void clear(bool del);
    void remove(int i);
    void remove(int i, bool del);
    SolutionSet * join(SolutionSet * another);
    void replace(int position, ValuePtr solution);
    void replace(int position, ValuePtr solution, bool del);
	MatrixOfDouble writeObjectivesToMatrix();
    void printObjectives();

    //void reset() ;
    //std::string toString();
};


#endif
