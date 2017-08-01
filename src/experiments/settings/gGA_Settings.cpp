//  gGA_Settings.cpp
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

#include <gGA_Settings.h>

/**
 * Default constructor
 */
gGA_Settings::gGA_Settings () : Settings()
{
} // gGA_Settings

/**
 * Destructor
 */
gGA_Settings::~gGA_Settings ()
{
    delete algorithm ;
    delete crossover ; // Crossover operator
    delete mutation  ; // Mutation operator
    delete selection ; // Selection operator
} // ~gGA_Settings

/**
 * Constructor
 */
gGA_Settings::gGA_Settings(std::string problemName)
{
    problemName_ = problemName ;

    problem_ = ProblemFactory::getProblem((char *) problemName_.c_str());

    // Algorithm parameters
    populationSize_ = 100;
    maxEvaluations_ = 25000;
    mutationProbability_         = 1.0/problem_->getNumberOfVariables() ;
    crossoverProbability_        = 0.9   ;
    mutationDistributionIndex_   = 20.0  ;
    crossoverDistributionIndex_  = 20.0  ;
} // gGA_Settings

/**
 * Configure method
 */
Algorithm * gGA_Settings::configure()
{

    algorithm = snew gGA(problem_);
    algorithm->setInputParameter("populationSize", IntValue(populationSize_));
    algorithm->setInputParameter("maxEvaluations", IntValue(maxEvaluations_));

    // Mutation and Crossover for Real codification
   MapOfStringFunct parameters;

    double crossoverProbability = crossoverProbability_;
    double crossoverDistributionIndex = crossoverDistributionIndex_ ;
    parameters["probability"] =  DoubleValue(crossoverProbability);
    parameters["distributionIndex"] = DoubleValue(crossoverDistributionIndex);
    crossover = snew SBXCrossover(parameters);

    parameters.clear();
    double mutationProbability = mutationProbability_;
    double mutationDistributionIndex = mutationDistributionIndex_;
    parameters["probability"] = DoubleValue(mutationProbability);
    parameters["distributionIndex"] = DoubleValue(mutationDistributionIndex);
    mutation = snew PolynomialMutation(parameters);

    // Selection Operator
    parameters.clear();
    selection = snew BinaryTournament2(parameters);

    // Add the operators to the algorithm
    algorithm->addOperator("crossover",crossover);
    algorithm->addOperator("mutation",mutation);
    algorithm->addOperator("selection",selection);

    std::cout << "gGA algorithm initialized." << std::endl;

    return algorithm ;
} // configure

