//  Experiment.h
//
//  Author:
//       Esteban L�pez <esteban@lcc.uma.es>
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

#ifndef __EXPERIMENT__
#define __EXPERIMENT__

#include <string>
#include <vector>
#include <FileUtils.h>
#include <stdlib.h>

using namespace std;

template<typename T, size_t N>
T * end(T (&ra)[N]) {
    return ra + N;
}

/**
 * Abstract class representing jMetal experiments
 */

class Experiment {

public:

  string experimentName_;
  vector<string> algorithmNameList_; // List of the names of the algorithms to
                                     // be executed
  vector<string> problemList_; // List of problems to be solved
  string experimentBaseDirectory_; // Directory to store the results
  int independentRuns_; // Number of independent runs per algorithm

  Experiment();

  void checkExperimentDirectory();

};

#endif /* __EXPERIMENT__ */
