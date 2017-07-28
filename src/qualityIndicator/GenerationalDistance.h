//  GenerationalDistance.h
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

#ifndef __GENERATIONALDISTANCE__
#define __GENERATIONALDISTANCE__

#include <MetricsUtil.h>
#include <math.h>
#include <vector>

/**
 * This class implements the generational distance indicator. It can be used also
 * as a command line by typing:
 * "java jmetal.qualityIndicator.GenerationalDistance <solutionFrontFile>
 * <trueFrontFile> <numberOfObjectives>"
 * Reference: Van Veldhuizen, D.A., Lamont, G.B.: Multiobjective Evolutionary
 *            Algorithm Research: A History and Analysis.
 *            Technical Report TR-98-03, Dept. Elec. Comput. Eng., Air Force
 *            Inst. Technol. (1998)
 */

class GenerationalDistance
{

private:
    static const double pow_;

public:

    MetricsUtil * utils_;

    GenerationalDistance();
    ~GenerationalDistance();
    double generationalDistance(std::vector< std::vector<double> > front,
                                std::vector< std::vector<double> > trueParetoFront, int numberOfObjectives);

};

#endif /* __GENERATIONALDISTANCE__ */
