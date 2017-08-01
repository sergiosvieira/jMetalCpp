//  GDE3_Settings.cpp
//
//  Author:
//       Esteban López-Camacho <esteban@lcc.uma.es>
//       Sérgio Vieira <sergiosvieira@gmail.com>
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


#include <GDE3_Settings.h>


/**
 * Default constructor
 */
GDE3_Settings::GDE3_Settings () : Settings()
{
} // GDE3_Settings


/**
 * Destructor
 */
GDE3_Settings::~GDE3_Settings ()
{
    delete algorithm ;
    delete crossover ; // Crossover operator
    delete selection ; // Selection operator
} // ~GDE3_Settings


/**
 * Constructor
 */
GDE3_Settings::GDE3_Settings(std::string problemName)
{

    problemName_ = problemName ;

    problem_ = ProblemFactory::getProblem((char *) problemName_.c_str());

    // Default settings
    CR_ = 0.5;
    F_ = 0.5;
    populationSize_ = 100;
    maxIterations_ = 250;

} // GDE3_Settings


/**
 * Configure method
 */
Algorithm * GDE3_Settings::configure()
{

    algorithm = snew GDE3(problem_);
    algorithm->setInputParameter("populationSize", IntValue(populationSize_));
    algorithm->setInputParameter("maxIterations", IntValue(maxIterations_));

    // Mutation and Crossover for Real codification
   MapOfStringFunct parameters;

    double CR = CR_;
    double F = F_;
    parameters["CR"] =  DoubleValue(CR);
    parameters["F"] = DoubleValue(F);
    crossover = snew DifferentialEvolutionCrossover(parameters);

    // Selection Operator
    parameters.clear();
    selection = snew DifferentialEvolutionSelection(parameters);

    // Add the operators to the algorithm
    algorithm->addOperator("crossover",crossover);
    algorithm->addOperator("selection",selection);

    std::cout << "GDE3 algorithm initialized." << std::endl;

    return algorithm ;

} // configure

