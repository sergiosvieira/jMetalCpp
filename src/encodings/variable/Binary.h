//  Binary.h
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


#ifndef Binary_h
#define Binary_h

#include "JMetalHeader.h"
#include <Variable.h>
#include <bitset>
#include <PseudoRandom.h>
#include <stdlib.h>

using namespace JMetal;

/**
 * This class implements a generic binary std::string variable.It can be used as
 * a base class other binary std::string based classes (e.g., binary coded integer
 * or real variables).
 */
class Binary : public Variable
{

public:

    Binary();
    Binary(int numberOfBits);
    Binary(Binary * variable);
    ~Binary();

    void decode();
    Variable * deepCopy();
    int getNumberOfBits();
    bool getIth(int bit);
    void setIth(int bit, bool value);
    void flip(int bit) ;
    int hammingDistance(Binary * other);
    std::string toString();
    int cardinality() ;

    double getValue();
    void setValue(double value);
    double getLowerBound();
    double getUpperBound();

protected:
    VectorOfBoolean * bits_ = nullptr;
    int numberOfBits_;
};

#endif
