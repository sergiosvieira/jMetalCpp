//  ssGA_Settings.cpp
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

#include <ssGA_Settings.h>

/**
 * Default constructor
 */
ssGA_Settings::ssGA_Settings () : Settings()
{
} // ssGA_Settings

/**
 * Destructor
 */
ssGA_Settings::~ssGA_Settings ()
{
    delete algorithm ;
    delete crossover ; // Crossover operator
    delete mutation  ; // Mutation operator
    delete selection ; // Selection operator
} // ~ssGA_Settings

/**
 * Constructor
 */
ssGA_Settings::ssGA_Settings(std::string problemName)
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
} // ssGA_Settings

/**
 * Configure method
 */
Algorithm * ssGA_Settings::configure()
{

    algorithm = new ssGA(problem_);
    algorithm->setInputParameter("populationSize",&populationSize_);
    algorithm->setInputParameter("maxEvaluations",&maxEvaluations_);

    // Mutation and Crossover for Real codification
   MapOfStringFunct parameters;

    double crossoverProbability = crossoverProbability_;
    double crossoverDistributionIndex = crossoverDistributionIndex_ ;
    parameters["probability"] =  &crossoverProbability;
    parameters["distributionIndex"] = &crossoverDistributionIndex;
    crossover = new SBXCrossover(parameters);

    parameters.clear();
    double mutationProbability = mutationProbability_;
    double mutationDistributionIndex = mutationDistributionIndex_;
    parameters["probability"] = &mutationProbability;
    parameters["distributionIndex"] = &mutationDistributionIndex;
    mutation = new PolynomialMutation(parameters);

    // Selection Operator
    parameters.clear();
    selection = new BinaryTournament(parameters);

    // Add the operators to the algorithm
    algorithm->addOperator("crossover",crossover);
    algorithm->addOperator("mutation",mutation);
    algorithm->addOperator("selection",selection);

    std::cout << "ssGA algorithm initialized." << std::endl;

    return algorithm ;
} // configure

