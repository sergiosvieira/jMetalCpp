//  FastSMSEMOA.h
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


#ifndef _FAST_SMSEMOA_H_
#define _FAST_SMSEMOA_H_

#include "JMetalHeader.h"
#include <Algorithm.h>
#include <Problem.h>
#include <SolutionSet.h>
#include <QualityIndicator.h>
#include <MetricsUtil.h>
#include <FastHypervolume.h>
#include <Ranking.h>
#include <CrowdingDistanceComparator.h>

using namespace JMetal;

class FastSMSEMOA : public Algorithm
{
protected:
    MetricsUtil * utils_ = nullptr;
    Hypervolume * hv_ = nullptr;
    VectorOfDouble hvContributions(MatrixOfDouble front);
public:
    FastSMSEMOA(Problem * problem);
    ~FastSMSEMOA();
    SolutionSet * execute();
};


#endif /* _FAST_SMSEMOA_H_ */
