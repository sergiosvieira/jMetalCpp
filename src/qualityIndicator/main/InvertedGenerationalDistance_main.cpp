//  InvertedGenerationalDistance_main.cpp
//
//  Author:
//       Esteban López-Camacho <esteban@lcc.uma.es>
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

#include <vector>
#include <stdio.h>

#include <InvertedGenerationalDistance.h>



/**
 * This method can be invoked from the command line. Three params are required:
 * 1) the name of the file containing the front,
 * 2) the name of the file containing the true Pareto front
 * 3) the number of objectives
 */
int main(int argc, char ** argv)
{
    if (argc < 4)
    {
        cerr << "Error using delta. Type: \n InvertedGenerationalDistance " <<
             "<SolutionFrontFile> <TrueFrontFile> <numberOfObjectives>" << std::endl;
        exit(-1);
    }

    //Create a new instance of the metric
    InvertedGenerationalDistance * qualityIndicator =
        snew InvertedGenerationalDistance();
    //Read the front from the files
    MatrixOfDouble solutionFront =
        qualityIndicator->utils_->readFront(argv[1]);
    MatrixOfDouble trueFront =
        qualityIndicator->utils_->readFront(argv[2]);

    //Obtain delta value
    double value = qualityIndicator->invertedGenerationalDistance(
                       solutionFront, trueFront, atoi(argv[3]));

    delete qualityIndicator;

    std::cout << value << std::endl;
} // main
