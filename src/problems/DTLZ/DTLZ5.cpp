//  DTLZ5.cpp
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

#include <DTLZ5.h>

const double DTLZ5::PI = 3.141592653589793;

DTLZ5::DTLZ5(std::string solutionType, int numberOfVariables, int numberOfObjectives)
{
    numberOfVariables_   = numberOfVariables;
    numberOfObjectives_  = numberOfObjectives;
    numberOfConstraints_ = 0;
    problemName_ 				 = "DTLZ5";

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
        //std::cout << "Tipo seleccionado Real" << std::endl;
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
    theta_ = snew double[numberOfObjectives_-1];
}

DTLZ5::~DTLZ5()
{
    delete [] lowerLimit_ ;
    delete [] upperLimit_ ;
    delete solutionType_ ;
    delete [] theta_     ;
}

/**
 * Evaluates a solution
 * @param solution The solution to evaluate
 */
void DTLZ5::evaluate(ValuePtr solution)
{
    XReal * vars = snew XReal(solution);
    double g = 0.0;

    int k = numberOfVariables_ - numberOfObjectives_ + 1;
    double alpha = 100.0;

    for (int i = 0; i < numberOfVariables_; i++)
        x_[i] = vars->getValue(i);

    for (int i = numberOfVariables_ - k; i < numberOfVariables_; i++)
        g += (x_[i] - 0.5)*(x_[i] - 0.5);

    double t = PI / (4.0 * (1.0 + g));

    theta_[0] = x_[0] * PI / 2.0;
    for (int i = 1; i < (numberOfObjectives_-1); i++)
        theta_[i] = t * (1.0 + 2.0 * g * x_[i]);

    for (int i = 0; i < numberOfObjectives_; i++)
        fx_[i] = 1.0 + g;

    for (int i = 0; i < numberOfObjectives_; i++)
    {
        for (int j = 0; j < numberOfObjectives_ - (i + 1); j++)
            fx_[i] *= cos(theta_[j]);
        if (i != 0)
        {
            int aux = numberOfObjectives_ - (i + 1);
            fx_[i] *= sin(theta_[aux]);
        } // if
    } //for
	Solution& sol = CastValue(solution, Solution)->getData();
    for (int i = 0; i < numberOfObjectives_; i++)
        sol.setObjective(i, fx_[i]);

    delete vars ;
} // evaluate
