//  EpsilonDominanceComparator.cpp
//
//  Authors:
//       Esteban López-Camacho <esteban@lcc.uma.es>
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

#include <EpsilonDominanceComparator.h>

/**
 * This class implements a <code>Comparator</code> (a method for comparing
 * <code>Solution</code> objects) based on epsilon dominance.
 */

/**
 * Constructor.
 *  @param eta Value for epsilon-dominance.
 */
EpsilonDominanceComparator::EpsilonDominanceComparator(double eta) : Comparator()
{
    eta_ = eta;
    overallConstraintViolationComparator_ = snew OverallConstraintViolationComparator();
} // EpsilonDominanceComparator

/**
 * Destructor.
 */
EpsilonDominanceComparator::~EpsilonDominanceComparator()
{
    delete overallConstraintViolationComparator_;
} // ~EpsilonDominanceComparator

/**
 * Compares two solutions.
 * @param o1 Object representing the first <code>Solution</code>.
 * @param o2 Object representing the second <code>Solution</code>.
 * @return -1, or 0, or 1 if o1 dominates o2, both are
 * non-dominated, or o1 is dominated by o2, respectively.
 */
int EpsilonDominanceComparator::compare(ValuePtr o1, ValuePtr o2)
{

    if (o1==nullptr)
        return 1;
    else if (o2 == nullptr)
        return -1;

    Solution&  solution1 = CastValue(o1, Solution)->getData();
    Solution&  solution2 = CastValue(o2, Solution)->getData();

    int dominate1 ; // dominate1 indicates if some objective of solution1
    // dominates the same objective in solution2. dominate2
    int dominate2 ; // is the complementary of dominate1.

    dominate1 = 0 ;
    dominate2 = 0 ;

    int flag; //stores the result of the comparison

    flag = overallConstraintViolationComparator_->compare(o1, o2);

    if (flag != 0)
    {
        return flag;
    }

    // Equal number of violated constraints. Applying a dominance Test then
    double value1, value2;
    for (int i = 0; i < solution1.getNumberOfObjectives(); i++)
    {
        value1 = solution1.getObjective(i);
        value2 = solution2.getObjective(i);

        //Objetive implements comparable!!!
        if (value1/(1 + eta_) < value2)
        {
            flag = -1;
        }
        else if (value1/(1 + eta_) > value2)
        {
            flag = 1;
        }
        else
        {
            flag =  0;
        }

        if (flag == -1)
        {
            dominate1 = 1;
        }

        if (flag == 1)
        {
            dominate2 = 1;
        }
    }

    if (dominate1 == dominate2)
    {
        return 0; //No one dominates the other
    }
    if (dominate1 == 1)
    {
        return -1; // solution1 dominates
    }
    return 1;    // solution2 dominates

} // compare
