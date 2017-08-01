//  DTLZ3.cpp
//
//  Author:
//       Antonio J. Nebro <antonio@lcc.uma.es>
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

#include <DTLZ3.h>

const double DTLZ3::PI = 3.141592653589793;

DTLZ3::DTLZ3(std::string solutionType, int numberOfVariables, int numberOfObjectives)
{
    numberOfVariables_   = numberOfVariables;
    numberOfObjectives_  = numberOfObjectives;
    numberOfConstraints_ = 0;
    problemName_ 				 = "DTLZ3";

    lowerLimit_ = snew double[numberOfVariables_];//(double *)malloc(sizeof(double)*numberOfVariables);
    if (lowerLimit_ == nullptr)
    {
        std::cout << "Impossible to reserve memory for storing the variable lower limits" << std::endl;
        exit(-1);
    }

    upperLimit_ = snew double[numberOfVariables_];//(double *)malloc(sizeof(double)*numberOfVariables);
    if (upperLimit_ == nullptr)
    {
        std::cout << "Impossible to reserve memory for storing the variable lower limits" << std::endl;
        exit(-1);
    }

    for (int i = 0; i < numberOfVariables_; i++)
    {
        lowerLimit_[i] = 0.0;
        upperLimit_[i] = 1.0;
    }

    if (solutionType.compare("BinaryReal") == 0)
        solutionType_ = snew BinaryRealSolutionType(this) ;
    else if (solutionType.compare("Real") == 0)
    {
        solutionType_ = snew RealSolutionType(this) ;
        // std::cout << "Tipo seleccionado Real" << std::endl;
    }
    else if (solutionType.compare("ArrayReal") == 0)
        solutionType_ = snew ArrayRealSolutionType(this) ;
    else
    {
        std::cout << "Error: solution type " << solutionType << " invalid" << std::endl;
        exit(-1) ;
    }
    fx_ = snew double[numberOfObjectives_] ;
    x_ = snew double[numberOfVariables_];
}

DTLZ3::~DTLZ3()
{
    delete [] lowerLimit_ ;
    delete [] upperLimit_ ;
    delete solutionType_ ;
}

/**
 * Evaluates a solution
 * @param solution The solution to evaluate
 */
void DTLZ3::evaluate(ValuePtr solution)
{
    XReal * vars = snew XReal(solution);

    int k = numberOfVariables_ - numberOfObjectives_ + 1;

    for (int i = 0; i < numberOfVariables_; i++)
        x_[i] = vars->getValue(i);

    double g = 0.0;
    for (int i = numberOfVariables_ - k; i < numberOfVariables_; i++)
        g += (x_[i] - 0.5)*(x_[i] - 0.5) - cos(20.0 * PI * (x_[i] - 0.5));

    g = 100.0 * (k + g);
    for (int i = 0; i < numberOfObjectives_; i++)
        fx_[i] = 1.0 + g;

    for (int i = 0; i < numberOfObjectives_; i++)
    {
        for (int j = 0; j < numberOfObjectives_ - (i + 1); j++)
            fx_[i] *= cos(x_[j]*0.5*PI);
        if (i != 0)
        {
            int aux = numberOfObjectives_ - (i + 1);
            fx_[i] *= sin(x_[aux]*0.5*PI);
        } // if
    } //for

    delete vars ;
} // evaluate
