//  SolutionSet.cpp
//
//  Author:
//       Esteban López-Camacho <esteban@lcc.uma.es>
//       Antonio J. Nebro <antonio@lcc.uma.es>
//       Juan J. Durillo <durillo@lcc.uma.es>
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


#include <SolutionSet.h>


/**
 * Constructor.
 * Creates an unbounded solution set.
 */
SolutionSet::SolutionSet ()
{
    capacity_ = 0;
} // SolutionSet


/**
 * Creates a empty solutionSet with a maximum capacity.
 * @param maximumSize Maximum size.
 */
SolutionSet::SolutionSet (int maximumSize)
{
    capacity_ = maximumSize;
} // SolutionSet


/**
 * Destructor
 */
SolutionSet::~SolutionSet()
{
} // ~SolutionSet


/**
 * Inserts a new solution into the SolutionSet.
 * @param solution The <code>Solution</code> to store
 * @return True If the <code>Solution</code> has been inserted, false
 * otherwise.
 */
bool SolutionSet::add(ValuePtr solution)
{
    if (solutionsList_.size() >= capacity_)
    {
        std::cout << "Error in class SolutionSet the maximum capacity of the set has been reached" << std::endl;
        exit(-1);
    }
    solutionsList_.push_back(CastValue(solution, Solution));
    return true;
} // add


/**
 * Inserts a new solution into the SolutionSet in a specific position.
 * @param index The position where to store the solution
 * @param solution The <code>Solution</code> to store
 * @return True If the <code>Solution</code> has been inserted, false
 * otherwise.
 */
bool SolutionSet::add(int index, ValuePtr solution)
{
    solutionsList_.insert(solutionsList_.begin()+index, CastValue(solution, Solution));
    return true;
}

/**
 * Returns the ith solution in the set.
 * @param i Position of the solution to obtain.
 * @return The <code>Solution</code> at the position i.
 * @throws IndexOutOfBoundsException.
 */
ValuePtr SolutionSet::get(int i)
{
    if (i < 0 || i >= solutionsList_.size())
    {
        std::cout << "Error in class SolutionSet trying to access to an element out of range" << std::endl;
        exit(-1);
    }
    return solutionsList_[i];
} // get


/**
 * Returns the maximum capacity of the solution set
 * @return The maximum capacity of the solution set
 */
int SolutionSet::getMaxSize()
{
    return capacity_;
} // getMaxSize


/**
 * Sorts a SolutionSet using a <code>Comparator</code>.
 * @param comparator <code>Comparator</code> used to sort.
 */
void SolutionSet::sort(SharedComparator comparator)
{
    if (comparator == nullptr)
    {
        std::cout << "Error. No criterium for compare exist" << std::endl;
        exit(-1);
    } // if
    for (int i = 0; i < solutionsList_.size(); i++)
    {
        for (int j = i+1; j < solutionsList_.size(); j++)
        {
            if ((comparator->compare(solutionsList_[i],solutionsList_[j]))==1)
            {
                ValuePtr tmp = solutionsList_[i];
                solutionsList_[i] = solutionsList_[j];
                solutionsList_[j] = tmp;
            }
        }
    } // At this point the solution set will be sorted
} // sort


/**
 * Returns the index of the best Solution using a <code>Comparator</code>.
 * If there are more than one occurrences, only the index of the first one is returned
 * @param comparator <code>Comparator</code> used to compare solutions.
 * @return The index of the best Solution attending to the comparator or
 * <code>-1<code> if the SolutionSet is empty
 */
int SolutionSet::indexBest(SharedComparator comparator)
{
    if (solutionsList_.empty())
    {
        return -1;
    }
    int index = 0;
    ValuePtr bestKnown = solutionsList_[0];
    ValuePtr candidateSolution;
    int flag;
    for (int i = 1; i < solutionsList_.size(); i++)
    {
        candidateSolution = solutionsList_[i];
        flag = comparator->compare(bestKnown, candidateSolution);
        //std::cout << "flag: " << flag << std::endl;
        if (flag == 1)
        {
            index = i;
            bestKnown = candidateSolution;
        }
    }
    return index;
} // indexBest


/**
 * Returns the best Solution using a <code>Comparator</code>.
 * If there are more than one occurrences, only the first one is returned
 * @param comparator <code>Comparator</code> used to compare solutions.
 * @return The best Solution attending to the comparator or <code>null<code>
 * if the SolutionSet is empty
 */
ValuePtr SolutionSet::best(SharedComparator comparator)
{
    int indexBest = this->indexBest(comparator);
    if (indexBest < 0)
    {
        return nullptr;
    }
    else
    {
        return solutionsList_[indexBest];
    }
} // best


/**
 * Returns the index of the worst Solution using a <code>Comparator</code>.
 * If there are more than one occurrences, only the index of the first one is returned
 * @param comparator <code>Comparator</code> used to compare solutions.
 * @return The index of the worst Solution attending to the comparator or
 * <code>-1<code> if the SolutionSet is empty
 */
int SolutionSet::indexWorst(SharedComparator comparator)
{
    if (solutionsList_.empty())
    {
        return -1;
    }
    int index = 0;
    ValuePtr worstKnown = solutionsList_[0];
    ValuePtr candidateSolution;
    int flag;
    for (int i = 1; i < solutionsList_.size(); i++)
    {
        candidateSolution = solutionsList_[i];
        flag = comparator->compare(worstKnown, candidateSolution);
        if (flag == -1)
        {
            index = i;
            worstKnown = candidateSolution;
        }
    }
    return index;
} // indexWorst


/**
 * Returns the worst Solution using a <code>Comparator</code>.
 * If there are more than one occurrences, only the first one is returned
 * @param comparator <code>Comparator</code> used to compare solutions.
 * @return The worst Solution attending to the comparator or <code>null<code>
 * if the SolutionSet is empty
 */
ValuePtr SolutionSet::worst(SharedComparator comparator)
{
    int index = indexWorst(comparator);
    if (index < 0)
    {
        return nullptr;
    }
    else
    {
        return solutionsList_[index];
    }
} // worst


/**
 * Returns the number of solutions in the SolutionSet.
 * @return The size of the SolutionSet.
 */
int SolutionSet::size()
{
    return solutionsList_.size();
} // size


/**
 * Writes the objective function values of the <code>Solution</code>
 * objects into the set in a file.
 * @param path The output file name
 */
void SolutionSet::printObjectivesToFile(std::string file)
{
    std::ofstream out(file.c_str());
    std::cout.setf(ios::fixed);
    for (int i = 0; i < solutionsList_.size(); i++)
    {
		Solution& solution = CastValue(solutionsList_[i], Solution)->getData();
        int nObj = solution.getNumberOfObjectives();
        for (int obj = 0; obj < nObj; obj++)
        {
            out << solution.getObjective(obj) << " ";
            //std::cout << setprecision(15) << solutionsList_[i]->getObjective(obj) << " ";
        }
        out << std::endl;
        //std::cout << std::endl;
    }
    out.close();
} // printObjectivesToFile


/**
 * Writes the objective function values of the <code>Solution</code>
 * objects into the set in a file.
 * @param path The output file name
 * @param append If the objectives will be appended in case a file
 * already exists
 */
void SolutionSet::printObjectivesToFile(std::string file, bool append)
{
    if (append)
    {
        std::fstream out(file.c_str(), std::ios::out | std::ios::app);
        std::cout.setf(ios::fixed);
        for (int i = 0; i < solutionsList_.size(); i++)
        {
			Solution& solution = CastValue(solutionsList_[i], Solution)->getData();
            int nObj = solution.getNumberOfObjectives();
            for (int obj = 0; obj < nObj; obj++)
            {
                out << solution.getObjective(obj) << " ";
            }
            out << std::endl;
        }
        out.close();
    }
    else
    {
        this->printObjectivesToFile(file);
    }
} // printObjectivesToFile


/**
 * Writes the decision variable values of the <code>Solution</code>
 * solutions objects into the set in a file.
 * @param path The output file name
 */
void SolutionSet::printVariablesToFile(std::string file)
{
    std::ofstream out(file.c_str());
    for (int i = 0; i < solutionsList_.size(); i++)
    {
		Solution& solution = CastValue(solutionsList_[i], Solution)->getData();
        out << solution.toString() << std::endl ;
    }
    out.close();
} // printVariablesToFile


/**
 * Writes the decision variable values of the <code>Solution</code>
 * solutions objects into the set in a file.
 * @param path The output file name
 */
void SolutionSet::printVariablesToFile(std::string file, bool append)
{
    if (append)
    {
        std::fstream out(file.c_str(), std::ios::out | std::ios::app);
        for (int i = 0; i < solutionsList_.size(); i++)
        {
			Solution& solution = CastValue(solutionsList_[i], Solution)->getData();
            out << solution.toString() << std::endl ;
        }
        out.close();
    }
    else
    {
        this->printVariablesToFile(file);
    }
} // printVariablesToFile


/**
 * Empties the SolutionSet
 */
void SolutionSet::clear()
{
    solutionsList_.clear();
} // clear


/**
 * Deletes the <code>Solution</code> at position i in the set.
 * @param i The position of the solution to remove.
 */
void SolutionSet::remove(int i)
{
    if (i < 0 || i >= solutionsList_.size())
    {
        std::cout << "Error in class SolutionSet trying to access to an element out of range" << std::endl;
        exit(-1);
    }
    solutionsList_.erase(solutionsList_.begin() + i);
} // remove


/**
 * Returns a new <code>SolutionSet</code> which is the result of the union
 * between the current solution set and the one passed as a parameter.
 * @param solutionSet SolutionSet to join with the current solutionSet.
 * @return The result of the union operation.
 */
SolutionSet * SolutionSet::join(SolutionSet *another)
{
    SolutionSet *result =
        new SolutionSet(solutionsList_.size()+another->size());
    for (int i=0; i<solutionsList_.size(); i++)
    {
		result->add(MakeShared(Solution, this->get(i)));
    }
    for (int i=0; i<another->size(); i++)
    {
		result->add(MakeShared(Solution, another->get(i)));
    }
    return result;
} // join


/**
 * Replaces a solution by a new one
 * @param position The position of the solution to replace
 * @param solution The new solution
 */
void SolutionSet::replace(int position, ValuePtr solution)
{
    if (position < 0 || position >= solutionsList_.size())
    {
        std::cout << "Error in class SolutionSet trying to access to an element out of range" << std::endl;
        exit(-1);
    }
    solutionsList_[position] = solution;
} // replace


/**
 * Copies the objectives of the solution set to a matrix
 * @return A matrix containing the objectives
 */
MatrixOfDouble SolutionSet::writeObjectivesToMatrix()
{
    MatrixOfDouble objectives;
    //objectives = snew double[size()][get(0).numberOfObjectives()];
    for (int i = 0; i < size(); i++)
    {
        VectorOfDouble list;
		Solution& solution = CastValue(get(0), Solution)->getData();
        for (int j = 0; j < solution.getNumberOfObjectives(); j++)
        {
            list.push_back(solution.getObjective(j));
        }
        objectives.push_back(list);
    }
    return objectives;
} // writeObjectivesMatrix


void SolutionSet::printObjectives()
{
    for (int i = 0; i < solutionsList_.size(); i++)
    {
		Solution& solution = CastValue(get(i), Solution)->getData();
        std::cout << solution.toString() << std::endl;
    }
}
