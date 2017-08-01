//  StandardPSO2007.cpp
//
//  Author:
//       Esteban López-Camacho <esteban@lcc.uma.es>
//       Sérgio Vieira <sergiosvieira@gmail.com>
//       Antonio J. Nebro <antonio@lcc.uma.es>
//
//  Copyright (c) 2014 Antonio J. Nebro
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


#include <StandardPSO2007.h>


/**
 * Class implementing a Standard PSO 2007 algorithm
 */


/**
 * Constructor
 * @param problem Problem to solve
 */
StandardPSO2007::StandardPSO2007(Problem * problem) : Algorithm(problem)
{
	W_ = 1.0/(2.0 * log(2)) ; //0.721;
	C_ = 1.0/2.0 + log(2) ; //1.193;

	comparator_ = MakeShared(ObjectiveComparator, ObjectiveComparator(0)) ; // Single objective comparator
	MapOfStringFunct parameters ; // Operator parameters

	parameters["comparator"] = comparator_;
	findBestSolution_ = new BestSolutionSelection(parameters) ;

	evaluations_ = 0 ;
} // StandardPSO2007


/**
 * Destructor
 */
StandardPSO2007::~StandardPSO2007()
{
    delete findBestSolution_;
} // ~StandardPSO2007


double StandardPSO2007::getW()
{
    return W_;
} // getW


double StandardPSO2007::getC()
{
    return C_;
} // getC


/**
 * Initialize all parameters of the algorithm
 */
void StandardPSO2007::initParams()
{
    swarmSize_ = IntValue(getInputParameter("swarmSize"));
    maxIterations_ = IntValue(getInputParameter("maxIterations"));
    numberOfParticlesToInform_ = IntValue(getInputParameter("numberOfParticlesToInform"));

    std::cout << "Swarm size: " << swarmSize_ << std::endl;

    iteration_ = 0;

    swarm_ = new SolutionSet(swarmSize_);

    // Create the speed_ vector
    speed_ = snew double*[swarmSize_];
} // initParams


/**
 * Delete all parameters of the algorithm
 */
void StandardPSO2007::deleteParams()
{
    delete swarm_;
    delete [] speed_;
} // deleteParams


ValuePtr StandardPSO2007::getNeighborBest(int i)
{
    ValuePtr bestLocalBestSolution = nullptr ;
    for (int index : neighborhood_->getNeighbors(i))
    {
		SharedValue(Solution) solution1 = CastValue(bestLocalBestSolution, Solution);
		SharedValue(Solution) solution2 = CastValue(localBest_[index], Solution);
        if ((bestLocalBestSolution == nullptr) 
			|| (solution1->getData().getObjective(0) > solution2->getData().getObjective(0)))
        {
            bestLocalBestSolution = localBest_[index] ;
        }
    }
    return bestLocalBestSolution ;
} // getNeighborBest


void StandardPSO2007::computeSpeed()
{
    double r1, r2 ;

    for (int i = 0; i < swarmSize_; i++)
    {

        XReal * particle = new XReal(swarm_->get(i)) ;
        XReal * localBest = new XReal(localBest_[i]) ;
        XReal * neighborhoodBest = new XReal(neighborhoodBest_[i]) ;
        //XReal * neighborhoodBest = new XReal(globalBest_) ;

        r1 = PseudoRandom::randDouble(0, C_);
        r2 = PseudoRandom::randDouble(0, C_);

        //W_ = 0.9 ;
        if (localBest_[i] != neighborhoodBest_[i])
        {
            for (int var = 0; var < particle->getNumberOfDecisionVariables(); var++)
            {
                speed_[i][var] = W_ * speed_[i][var] +
                                 r1 * (localBest->getValue(var) - particle->getValue(var)) +
                                 r2 * (neighborhoodBest->getValue(var) - particle->getValue(var));
            }
        }
        else
        {
            for (int var = 0; var < particle->getNumberOfDecisionVariables(); var++)
            {
                speed_[i][var] = W_* speed_[i][var] +
                                 r1 * (localBest->getValue(var) - particle->getValue(var)) ;
            }
        }

        delete particle;
        delete localBest;
        delete neighborhoodBest;

    }
} // computeSpeed


/**
 * Update the position of each particle
 */
void StandardPSO2007::computeNewPositions()
{
    for (int i = 0; i < swarmSize_; i++)
    {
        //Variable[] particle = swarm_.get(i).getDecisionVariables();
        XReal * particle = new XReal(swarm_->get(i)) ;
        //particle.move(speed_[i]);
        for (int var = 0; var < particle->size(); var++)
        {
            particle->setValue(var, particle->getValue(var) +  speed_[i][var]) ;
            if (particle->getValue(var) < problem_->getLowerLimit(var))
            {
                particle->setValue(var, problem_->getLowerLimit(var));
                speed_[i][var] = 0;
            }
            if (particle->getValue(var) > problem_->getUpperLimit(var))
            {
                particle->setValue(var, problem_->getUpperLimit(var));
                speed_[i][var] = 0;
            }
        }
        delete particle;
    }
} // computeNewPositions


/**
 * Runs of the StandardPSO2007 algorithm.
 * @return a <code>SolutionSet</code> that is a set of non dominated solutions
 * as a result of the algorithm execution
 */
SolutionSet * StandardPSO2007::execute()
{
    initParams();

    // Step 1 Create the initial population and evaluate
    for (int i = 0; i < swarmSize_; i++)
    {
        //Solution * particle = new Solution(problem_);
        SharedSolution particle = MakeShared(Solution, Solution(problem_));
		problem_->evaluate(particle);
        evaluations_ ++ ;
        swarm_->add(particle);
    }

    neighborhood_ = new AdaptiveRandomNeighborhood(swarm_, numberOfParticlesToInform_) ;

    std::cout << "SwarmSize: " << swarmSize_ << std::endl;
    std::cout << "Swarm size: " << swarm_->size() << std::endl;
    std::cout << "list size: " << neighborhood_->getNeighborhood().size() << std::endl;

    //-> Step2. Initialize the speed_ of each particle
    for (int i = 0; i < swarmSize_; i++)
    {
        XReal * particle = new XReal(swarm_->get(i))  ;
        speed_[i] = snew double[problem_->getNumberOfVariables()];
        for (int j = 0; j < problem_->getNumberOfVariables(); j++)
        {
            speed_[i][j] = (PseudoRandom::randDouble(particle->getLowerBound(j),particle->getUpperBound(j))
                            - particle->getValue(j))/2.0 ;
        }
        delete particle;
    }

    //-> Step 6. Initialize the memory of each particle
    for (int i = 0; i < swarm_->size(); i++)
    {
		Solution& sol = CastValue(swarm_->get(i), Solution)->getData();
		SharedSolution particle = MakeShared(Solution, Solution(sol));
        localBest_[i] = particle;
    }

    for (int i = 0; i < swarm_->size(); i++)
    {
        neighborhoodBest_[i] = getNeighborBest(i) ;
    }

    //std::cout << "neighborhood_i " << neighborhood_->getNeighbors(0) << std::endl;
    std::cout << "[";
    for (int i = 0; i < neighborhood_->getNeighbors(0).size()-1; i++)
    {
        std::cout << neighborhood_->getNeighbors(0).at(i) << ",";
    }
    std::cout << neighborhood_->getNeighbors(0).back() << "]" << std::endl;
    for (int s :  neighborhood_->getNeighbors(0))
    {
		Solution& currentSol = CastValue(localBest_[s], Solution)->getData();
        std::cout << s << ": " << currentSol.getObjective(0) << std::endl;
    }
	Solution& localBestSolution = CastValue(localBest_[0], Solution)->getData();
    std::cout << "localBest_i " << localBestSolution.getObjective(0) << std::endl;
	Solution& bestNeighborSolution = CastValue(getNeighborBest(0), Solution)->getData();
    std::cout << "neighborhoodBest_i " << bestNeighborSolution.getObjective(0) << std::endl;

    std::cout << "Swarm: " << swarm_ << std::endl;
    swarm_->printObjectives();
	Solution& bestSolution = CastValue(swarm_->best(comparator_), Solution)->getData();
    double b = bestSolution.getObjective(0) ;
    std::cout << "Best: " << b << std::endl;

    double bestFoundFitness = numeric_limits<double>::max();

    while (iteration_ < maxIterations_)
    {
        //Compute the speed
        computeSpeed() ;

        //Compute the new positions for the swarm
        computeNewPositions();

        //Evaluate the new swarm_ in new positions
        for (int i = 0; i < swarm_->size(); i++)
        {
            ValuePtr particle = swarm_->get(i);
            problem_->evaluate(particle);
            evaluations_ ++ ;
        }

        //Update the memory of the particles
        for (int i = 0; i < swarm_->size(); i++)
        {
			Solution& currentSolution = CastValue(swarm_->get(i), Solution)->getData();
			Solution& localBestSolution = CastValue(localBest_[i], Solution)->getData();
            if ((currentSolution.getObjective(0) < localBestSolution.getObjective(0)))
            {
                SharedSolution particle = MakeShared(Solution, Solution(currentSolution));                
                localBest_[i] = particle;
            } // if
        }
        for (int i = 0; i < swarm_->size(); i++)
        {
            neighborhoodBest_[i] = getNeighborBest(i) ;
        }

        iteration_++;
        //System.out.println("Swarm( " + iteration_+ "): " + swarm_) ;
        //swarm_.printObjectives();
		bestSolution = CastValue(swarm_->best(comparator_), Solution)->getData();
        double bestCurrentFitness = bestSolution.getObjective(0) ;
        std::cout << "Best: " << bestCurrentFitness << std::endl;

        if (bestCurrentFitness == bestFoundFitness)
        {
            std::cout << "Recomputing" << std::endl;
            neighborhood_->recompute();
        }

        if (bestCurrentFitness < bestFoundFitness)
        {
            bestFoundFitness = bestCurrentFitness ;
        }
    }

    // Return a population with the best individual
    SolutionSet * resultPopulation = new SolutionSet(1) ;
	SharedSolutionSet sharedSolutionSet = MakeShared(SolutionSet, swarm_);
	int bestSolutionIdx = IntValue(GetValue(findBestSolution_, Operator).execute(sharedSolutionSet));
	Solution& newBestSolution = GetValue(swarm_->get(bestSolutionIdx), Solution);
	resultPopulation->add(MakeShared(Solution, Solution(bestSolution)));

    // Free memory
    for (int i = 0; i < swarmSize_; i++)
    {
        delete [] speed_[i];
    }

    delete neighborhood_;
    deleteParams();

    return resultPopulation ;
} // execute
