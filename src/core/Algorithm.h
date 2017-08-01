//  Algorithm.h
//
//  Author:
//       Antonio J. Nebro <antonio@lcc.uma.es>
//       Juan J. Durillo <durillo@lcc.uma.es>
//       Esteban López-Camacho <esteban@lcc.uma.es>
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

#ifndef __ALGORITHM__
#define __ALGORITHM__

#include "JMetalHeader.h"
#include <string>
#include <SolutionSet.h>
#include <Operator.h>
#include <Problem.h>

/**
 *  This class implements a generic template for the algorithms developed in
 *  jMetal. Every algorithm must have a mapping between the parameters and
 *  and their names, and another mapping between the operators and their names.
 *  The class declares an abstract method called <code>execute</code>, which
 *  defines the behavior of the algorithm.
**/

using namespace JMetal;

class Algorithm
{

public:
    Algorithm(Problem *problem);
    virtual ~Algorithm();
    virtual SolutionSet * execute() = 0; // this launch the execution
    // of an algorithm
    void addOperator(std::string name, Operator *operator_);
    Operator * getOperator(std::string name);
    void setInputParameter(std::string name, ValuePtr value);
    ValuePtr getInputParameter(std::string name);
    void setOutputParameter(std::string name, ValuePtr value);
    ValuePtr getOutputParameter(std::string name);
    Problem * getProblem();

protected:
	Problem *problem_ = nullptr;
	MapOfStringOperatorPtr operators_;
	MapOfStringFunct inputParameters_;
	MapOfStringFunct outputParameters_;

}; // Algorithm

#endif
