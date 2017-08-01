//  DifferentialEvolutionCrossover.cpp
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


#include <DifferentialEvolutionCrossover.h>


/**
 * Differential evolution crossover operators
 * Comments:
 * - The operator receives two parameters: the current individual and an array
 *   of three parent individuals
 * - The best and rand variants depends on the third parent, according whether
 *   it represents the current of the "best" individual or a randon one.
 *   The implementation of both variants are the same, due to that the parent
 *   selection is external to the crossover operator.
 * - Implemented variants:
 *   - rand/1/bin (best/1/bin)
 *   - rand/1/exp (best/1/exp)
 *   - current-to-rand/1 (current-to-best/1)
 *   - current-to-rand/1/bin (current-to-best/1/bin)
 *   - current-to-rand/1/exp (current-to-best/1/exp)
 */

/**
 * DEFAULT_CR defines a default CR (crossover operation control) value
 */
const double DifferentialEvolutionCrossover::DEFAULT_CR = 0.5;

/**
 * DEFAULT_F defines the default F (Scaling factor for mutation) value
 */
const double DifferentialEvolutionCrossover::DEFAULT_F = 0.5;

/**
 * DEFAULT_K defines a default K value used in variants current-to-rand/1
 * and current-to-best/1
 */
const double DifferentialEvolutionCrossover::DEFAULT_K = 0.5;

/**
 * DEFAULT_VARIANT defines the default DE variant
 */
const std::string DifferentialEvolutionCrossover::DEFAULT_DE_VARIANT = "rand/1/bin";


/**
 * Constructor
 */
DifferentialEvolutionCrossover::DifferentialEvolutionCrossover(MapOfStringFunct parameters)
    : Crossover(parameters)
{

    CR_ = DEFAULT_CR ;
    F_  = DEFAULT_F  ;
    K_  = DEFAULT_K   ;
    DE_Variant_ = DEFAULT_DE_VARIANT ;

    if (parameters["CR"] != nullptr)
    {
        //CR_ = *(double *) parameters["CR"];
		CR_ = DoubleValue(parameters["CR"]);
    }
    if (parameters["F"] != nullptr)
    {
        F_ = DoubleValue(parameters["F"]);
    }
    if (parameters["K"] != nullptr)
    {
        K_ = DoubleValue(parameters["K"]);
    }
    if (parameters["DE_VARIANT"] != nullptr)
    {
        DE_Variant_ = StringValue(parameters["DE_VARIANT"]);
    }

} // DifferentialEvolutionCrossover


/**
* Executes the operation
* @param object An object containing an array of three parents
* @return An object containing the offSprings
*/
ValuePtr DifferentialEvolutionCrossover::execute(ValuePtr object)
{
	VectorOfValuePtr& parameters = GetValue(object, VectorOfValuePtr);
	Solution& current = GetValue(parameters[0], Solution);
	VectorOfValuePtr parent = GetValue(parameters[1], VectorOfValuePtr);
    // TODO: Comprobar la longitud de parents

    // TODO: Chequear el tipo de parents
    //  if (!(VALID_TYPES.contains(parent[0].getType().getClass()) &&
    //        VALID_TYPES.contains(parent[1].getType().getClass()) &&
    //        VALID_TYPES.contains(parent[2].getType().getClass())) ) {
    //
    //    Configuration.logger_.severe("DifferentialEvolutionCrossover.execute: " +
    //        " the solutions " +
    //        "are not of the right type. The type should be 'Real' or 'ArrayReal', but " +
    //        parent[0].getType() + " and " +
    //        parent[1].getType() + " and " +
    //        parent[2].getType() + " are obtained");
    //
    //    Class cls = java.lang.String.class;
    //    String name = cls.getName();
    //    throw new JMException("Exception in " + name + ".execute()") ;
    //  }

    int jrand;

	//Solution * child = snew Solution(current);
	SharedSolution child = MakeShared(Solution, Solution(current));

    XReal * xParent0 = snew XReal(parent[0]);
    XReal * xParent1 = snew XReal(parent[1]);
    XReal * xParent2 = snew XReal(parent[2]);
    XReal * xCurrent = snew XReal(parameters[0]);
    XReal * xChild   = snew XReal(child);

    int numberOfVariables = xParent0->getNumberOfDecisionVariables();
    jrand = PseudoRandom::randInt(0, numberOfVariables - 1);

    // STEP 4. Checking the DE variant
    if ((DE_Variant_.compare("rand/1/bin") == 0) ||
            (DE_Variant_.compare("best/1/bin") == 0))
    {

        for (int j=0; j < numberOfVariables; j++)
        {
            if (PseudoRandom::randDouble(0, 1) < CR_ || j == jrand)
            {
                double value;
                value = xParent2->getValue(j)  + F_ * (xParent0->getValue(j) -
                                                       xParent1->getValue(j));

                if (value < xChild->getLowerBound(j))
                {
                    value =  xChild->getLowerBound(j);
                }
                if (value > xChild->getUpperBound(j))
                {
                    value = xChild->getUpperBound(j);
                }
                /*
                if (value < xChild.getLowerBound(j)) {
                  double rnd = PseudoRandom.randDouble(0, 1) ;
                  value = xChild.getLowerBound(j) + rnd *(xParent2.getValue(j) - xChild.getLowerBound(j)) ;
                }
                if (value > xChild.getUpperBound(j)) {
                  double rnd = PseudoRandom.randDouble(0, 1) ;
                  value = xChild.getUpperBound(j) - rnd*(xChild.getUpperBound(j)-xParent2.getValue(j)) ;
                }
                */
                xChild->setValue(j, value);
            }
            else
            {
                double value ;
                value = xCurrent->getValue(j);
                xChild->setValue(j, value);
            } // else
        } // for

    }
    else if ((DE_Variant_.compare("rand/1/exp") == 0) ||
             (DE_Variant_.compare("best/1/exp") == 0))
    {

        for (int j=0; j < numberOfVariables; j++)
        {
            if (PseudoRandom::randDouble(0, 1) < CR_ || j == jrand)
            {
                double value;
                value = xParent2->getValue(j)  + F_ * (xParent0->getValue(j) -
                                                       xParent1->getValue(j));

                if (value < xChild->getLowerBound(j))
                {
                    value =  xChild->getLowerBound(j);
                }
                if (value > xChild->getUpperBound(j))
                {
                    value = xChild->getUpperBound(j);
                }

                xChild->setValue(j, value);
            }
            else
            {
                CR_ = 0.0;
                double value;
                value = xCurrent->getValue(j);
                xChild->setValue(j, value);
            } // if
        } // for

    }
    else if ((DE_Variant_.compare("current-to-rand/1") == 0) ||
             (DE_Variant_.compare("current-to-best/1") == 0))
    {

        for (int j=0; j < numberOfVariables; j++)
        {
            double value;
            value = xCurrent->getValue(j) + K_ * (xParent2->getValue(j) -
                                                  xCurrent->getValue(j)) +
                    F_ * (xParent0->getValue(j) - xParent1->getValue(j));

            if (value < xChild->getLowerBound(j))
            {
                value =  xChild->getLowerBound(j);
            }
            if (value > xChild->getUpperBound(j))
            {
                value = xChild->getUpperBound(j);
            }
            xChild->setValue(j, value) ;
        } // for

    }
    else if ((DE_Variant_.compare("current-to-rand/1/bin") == 0) ||
             (DE_Variant_.compare("current-to-best/1/bin") == 0))
    {

        for (int j=0; j < numberOfVariables; j++)
        {
            if (PseudoRandom::randDouble(0, 1) < CR_ || j == jrand)
            {
                double value ;
                value = xCurrent->getValue(j) + K_ * (xParent2->getValue(j) -
                                                      xCurrent->getValue(j)) +
                        F_ * (xParent0->getValue(j) - xParent1->getValue(j));

                if (value < xChild->getLowerBound(j))
                {
                    value =  xChild->getLowerBound(j);
                }
                if (value > xChild->getUpperBound(j))
                {
                    value = xChild->getUpperBound(j);
                }

                xChild->setValue(j, value);
            }
            else
            {
                double value;
                value = xCurrent->getValue(j);
                xChild->setValue(j, value);
            } // else
        } // for

    }
    else if ((DE_Variant_.compare("current-to-rand/1/exp") == 0) ||
             (DE_Variant_.compare("current-to-best/1/exp") == 0))
    {

        for (int j=0; j < numberOfVariables; j++)
        {
            if (PseudoRandom::randDouble(0, 1) < CR_ || j == jrand)
            {
                double value ;
                value = xCurrent->getValue(j) + K_ * (xParent2->getValue(j) -
                                                      xCurrent->getValue(j)) +
                        F_ * (xParent0->getValue(j) - xParent1->getValue(j));

                if (value < xChild->getLowerBound(j))
                {
                    value =  xChild->getLowerBound(j);
                }
                if (value > xChild->getUpperBound(j))
                {
                    value = xChild->getUpperBound(j);
                }

                xChild->setValue(j, value);
            }
            else
            {
                CR_ = 0.0;
                double value;
                value = xCurrent->getValue(j);
                xChild->setValue(j, value);
            } // else
        } // for

    }
    else
    {

        cerr << "DifferentialEvolutionCrossover.execute: " <<
             " unknown DE variant (" << DE_Variant_ << ")" << std::endl;
        cerr << "Exception in DifferentialEvolutionCrossover.execute()" << std::endl;
        exit(-1);

    } // if

    delete xParent0;
    delete xParent1;
    delete xParent2;
    delete xCurrent;
    delete xChild;

    return child;

} // execute
