//  StandardPSO2011.cpp
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


#include <StandardPSO2011.h>


/**
 * Class implementing a Standard PSO 2011 algorithm
 */


/**
 * Constructor
 * @param problem Problem to solve
 */
StandardPSO2011::StandardPSO2011(Problem * problem) : Algorithm(problem)
{

    W_ = 1.0/(2.0 * log(2)) ; //0.721;
    C_ = 1.0/2.0 + log(2) ; //1.193;
    ChVel_ = -0.5 ;

    comparator_ = MakeShared(ObjectiveComparator, ObjectiveComparator(0)); // Single objective comparator
   MapOfStringFunct parameters ; // Operator parameters
    parameters["comparator"] = comparator_;
    findBestSolution_ = MakeShared(BestSolutionSelection, BestSolutionSelection(parameters));

    evaluations_ = 0 ;
} // StandardPSO2011


/**
 * Destructor
 */
StandardPSO2011::~StandardPSO2011()
{

} // ~StandardPSO2011


double StandardPSO2011::getW()
{
    return W_;
} // getW


double StandardPSO2011::getC()
{
    return C_;
} // getC


/**
 * Initialize all parameters of the algorithm
 */
void StandardPSO2011::initParams()
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
void StandardPSO2011::deleteParams()
{
    delete swarm_;
    delete [] speed_;
} // deleteParams


ValuePtr StandardPSO2011::getNeighborBest(int i)
{
    ValuePtr bestLocalBestSolution = nullptr ;	
    for (int index : neighborhood_->getNeighbors(i))
    {
		SharedValue(Solution) solution1 = CastValue(bestLocalBestSolution, Solution);
		SharedValue(Solution) solution2 = CastValue(localBest_[index], Solution);
        if ((bestLocalBestSolution == nullptr) 
			|| (solution1->getData().getObjective(0) > solution2->getData().getObjective(0)))
        {
            bestLocalBestSolution = localBest_[index];
        }
    }

    return bestLocalBestSolution ;
} // getNeighborBest


void StandardPSO2011::computeSpeed()
{

    for (int i = 0; i < swarmSize_; i++)
    {
        XReal * particle = new XReal(swarm_->get(i)) ;
        XReal * localBest = new XReal(localBest_[i]) ;
        XReal * neighborhoodBest = new XReal(neighborhoodBest_[i]) ;
		ValuePtr gravityCenterSolution = MakeShared(Solution, Solution(problem_));
        XReal * gravityCenter = new XReal(gravityCenterSolution);
        //Solution * randomParticleSolution = new Solution(swarm_->get(i));
		Solution& sol = CastValue(swarm_->get(i), Solution)->getData();
		ValuePtr randomParticleSolution = MakeShared(Solution, Solution(sol));
        XReal * randomParticle = new XReal (randomParticleSolution) ;

        if (localBest_[i] != neighborhoodBest_[i])
        {
            for (int var = 0; var < particle->size(); var++)
            {
                double G;
                G = particle->getValue(var) +
                    C_ * (localBest->getValue(var) + neighborhoodBest->getValue(var) - 2 * particle->getValue(var)) / 3.0;

                gravityCenter->setValue(var, G);
            }
        }
        else
        {
            for (int var = 0; var < particle->size(); var++)
            {
                double G;
                G = particle->getValue(var) +
                    C_ * (localBest->getValue(var) - particle->getValue(var)) / 2.0;

                gravityCenter->setValue(var, G);
            }
        }

        double radius = 0;
        Distance * distance = new Distance();
        radius = distance->distanceBetweenSolutions(gravityCenter->getSolution(), particle->getSolution());
        delete distance;

        double * random = PseudoRandom::randSphere(problem_->getNumberOfVariables()) ;
        for (int var = 0; var < particle->size(); var++)
        {
            randomParticle->setValue(var, gravityCenter->getValue(var) + radius*random[var]) ;
        }
        delete [] random;

        for (int var = 0; var < particle->getNumberOfDecisionVariables(); var++)
        {
            speed_[i][var] = W_*speed_[i][var] + randomParticle->getValue(var) - particle->getValue(var);
        }

        delete particle;
        delete localBest;
        delete neighborhoodBest;        
        delete gravityCenter;        
        delete randomParticle;

    }
} // computeSpeed


/**
 * Update the position of each particle
 */
void StandardPSO2011::computeNewPositions()
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
                speed_[i][var] = ChVel_ * speed_[i][var];
            }
            if (particle->getValue(var) > problem_->getUpperLimit(var))
            {
                particle->setValue(var, problem_->getUpperLimit(var));
                speed_[i][var] = ChVel_ * speed_[i][var];
            }
        }
        delete particle;
    }
} // computeNewPositions


/**
 * Runs of the StandardPSO2011 algorithm.
 * @return a <code>SolutionSet</code> that is a set of non dominated solutions
 * as a result of the algorithm execution
 */
SolutionSet * StandardPSO2011::execute()
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
            speed_[i][j] = (PseudoRandom::randDouble(
                                particle->getLowerBound(j) - particle->getValue(0),
                                particle->getUpperBound(j) - particle->getValue(0))) ;
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
		Solution& sol = CastValue(localBest_[s], Solution)->getData();
        std::cout << s << ": " << sol.getObjective(0) << std::endl;
    }
	Solution& localBestSolution = CastValue(localBest_[0], Solution)->getData();
    std::cout << "localBest_i " << localBestSolution.getObjective(0) << std::endl;
	Solution& neighborSolution = CastValue(getNeighborBest(0), Solution)->getData();
    std::cout << "neighborhoodBest_i " << neighborSolution.getObjective(0) << std::endl;

    std::cout << "Swarm: " << swarm_ << std::endl;
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
            problem_->evaluate(swarm_->get(i));
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
        //std::cout << "Best: " << bestCurrentFitness << std::endl;

        if (bestCurrentFitness == bestFoundFitness)
        {
            //std::cout << "Recomputing" << std::endl;
            neighborhood_->recompute();
        }

        if (bestCurrentFitness < bestFoundFitness)
        {
            bestFoundFitness = bestCurrentFitness ;
        }
    }

    // Return a population with the best individual
    SolutionSet * resultPopulation = new SolutionSet(1) ;
	Operator& oper = CastValue(findBestSolution_, Operator)->getData();
	SharedValue(SolutionSet) sharedSolutionSet = MakeShared(SolutionSet, swarm_);
    int bestSolutionIdx = IntValue(oper.execute(sharedSolutionSet));
	Solution& bestSolution = CastValue(swarm_->get(bestSolutionIdx), Solution)->getData();
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
