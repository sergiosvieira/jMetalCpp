//  Fonseca.cpp
//
//  Authors:
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

#include <Fonseca.h>

/**
 * Class constructor
 */
Fonseca::Fonseca(std::string solutionType)
{
    numberOfVariables_   = 3;
    numberOfObjectives_  = 2;
    numberOfConstraints_ = 0;
    problemName_         = "Fonseca";

    lowerLimit_ = snew double[numberOfVariables_];
    if (lowerLimit_ == nullptr)
    {
        std::cout << "Fonseca::Fonseca. Error reserving memory for storing the array of lower limits" << std::endl;
    } // if

    upperLimit_ = snew double[numberOfVariables_];
    if (upperLimit_ == nullptr)
    {
        std::cout << "Fonseca::Fonseca. Error reserving memory for storing the array of upper limits" << std::endl;
        exit(-1) ;
    } // if

    for (int i = 0; i < numberOfVariables_; i++)
    {
        lowerLimit_[i] = -4.0;
        upperLimit_[i] =  4.0;
    } // for

    if (solutionType.compare("BinaryReal") == 0)
    {
        //std::cout << "Selected solution type: BinaryReal" << std::endl;
        solutionType_ = snew BinaryRealSolutionType(this) ;
    }
    else if (solutionType.compare("Real") == 0)
    {
        solutionType_ = snew RealSolutionType(this) ;
        //std::cout << "Selected solution type: Real" << std::endl;
    }
    else if (solutionType.compare("ArrayReal") == 0)
    {
        solutionType_ = snew ArrayRealSolutionType(this) ;
    }
    else
    {
        std::cout << "Error: solution type " << solutionType << " invalid" << std::endl;
        exit(-1) ;
    }
} // Fonseca


/**
 * Destructor
 */
Fonseca::~Fonseca()
{
    delete [] lowerLimit_ ;
    delete [] upperLimit_ ;
    delete solutionType_ ;
} // ~Fonseca


/**
  * Evaluates a solution
  */
void Fonseca::evaluate(ValuePtr solution)
{
    double *fx;
    double *x ;
    Variable **variables = solution->getDecisionVariables();

    fx = snew double[numberOfObjectives_];
    if (fx == nullptr)
    {
        std::cout << "Fonseca::evaluate: Error reserving memory for the function values array" << std::endl;
        exit(-1);
    } // if

    x = snew double[numberOfVariables_];
    if (x == nullptr)
    {
        std::cout << "Fonseca::evaluate: Error reserving memory for the variable values array" << std::endl;
        exit(-1);
    } // if

    for (int i = 0; i < numberOfVariables_; i++)
        x[i] = variables[i]->getValue() ;

    double sum1 = 0.0 ;
    for (int i = 0; i < numberOfVariables_; i++)
        sum1 += pow(x[i] - (1.0/sqrt((double)numberOfVariables_)),2.0);

    double exp1 = exp((-1.0)*sum1);
    fx[0] = 1 - exp1;

    double sum2 = 0.0;
    for (int i = 0; i < numberOfVariables_; i++)
    {
        sum2 += pow(x[i] + (1.0/sqrt((double)numberOfVariables_)),2.0);
    }
    double exp2 = exp((-1.0)*sum2);
    fx[1] = 1 - exp2;

    solution->setObjective(0,fx[0]);
    solution->setObjective(1,fx[1]);

    delete[]fx ;
    delete[]x ;
} // evaluate

