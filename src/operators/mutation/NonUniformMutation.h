//  NonUniformMutation.h
//
//  Author:
//       Esteban López-Camacho <esteban@lcc.uma.es>
//       Sérgio Vieira <sergiosvieira@gmail.com>
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

#ifndef __NON_UNIFORM_MUTATION__
#define __NON_UNIFORM_MUTATION__

#include <Mutation.h>
#include <Solution.h>
#include <PseudoRandom.h>
#include <XReal.h>

/**
 * This class implements a non-uniform mutation operator.
 */
class NonUniformMutation : public Mutation
{

private:

    /**
     * Valid solution types to apply this operator.
     */
    static const std::string VALID_TYPES[];

    /**
     * perturbation_ stores the perturbation value used in the Non Uniform
     * mutation operator
     */
    double perturbation_ = 0.0;

    /**
     * maxIterations_ stores the maximun number of iterations.
     */
    int maxIterations_ = 0;

    /**
     * currentIteration_ stores the iteration in which the operator is going to be
     * applied
     */
    int currentIteration_ = 0;

    double mutationProbability_ = 0;

    /**
     * Performs the operation.
     * @param probability Mutation probability
     * @param solution The solution to mutate
     * @throws JMException
     */
    void doMutation(double probability, Solution *solution);

    /**
     * Calculates the delta value used in NonUniform mutation operator
     */
    double delta(double y, double bMutationParameter);

public:

    /**
     * Constructor.
     * Creates a new instance of the non uniform mutation
     */
    NonUniformMutation(MapOfStringFunct parameters);

    /**
     * Executes the operation
     * @param object An object containing the solution to mutate
     */
    void *execute(void *);

}; // NonUniformMutation

#endif
