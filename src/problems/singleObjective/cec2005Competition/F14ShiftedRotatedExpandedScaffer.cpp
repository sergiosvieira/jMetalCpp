//  F14ShiftedRotatedExpandedScaffer.cpp
//
//  Authors:
//       Esteban López-Camacho <esteban@lcc.uma.es>
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


#include <F14ShiftedRotatedExpandedScaffer.h>
#include "JMetalHeader.h"

// Fixed (class) parameters
const std::string F14ShiftedRotatedExpandedScaffer::FUNCTION_NAME = "Shifted Rotated Expanded Scaffer's F6 Function";
// TODO: Cambiar ruta
const std::string F14ShiftedRotatedExpandedScaffer::DEFAULT_FILE_DATA = "../../data/cec2005CompetitionResources/supportData/E_ScafferF6_func_data.txt";
const std::string F14ShiftedRotatedExpandedScaffer::DEFAULT_FILE_MX_PREFIX = "../../data/cec2005CompetitionResources/supportData/E_ScafferF6_M_D";
const std::string F14ShiftedRotatedExpandedScaffer::DEFAULT_FILE_MX_SUFFIX = ".txt";


/**
 * Constructor.
 */
F14ShiftedRotatedExpandedScaffer::F14ShiftedRotatedExpandedScaffer(int dimension, double bias)
    : F14ShiftedRotatedExpandedScaffer(dimension, bias, DEFAULT_FILE_DATA, getFileMxName(DEFAULT_FILE_MX_PREFIX, dimension, DEFAULT_FILE_MX_SUFFIX))
{
} // F14ShiftedRotatedExpandedScaffer


/**
 * Constructor
 */
F14ShiftedRotatedExpandedScaffer::F14ShiftedRotatedExpandedScaffer(int dimension, double bias, std::string file_data, std::string file_m)
    : TestFunc(dimension, bias, FUNCTION_NAME)
{

    // Note: dimension starts from 0
    m_o = snew double[m_dimension];
    m_matrix = snew double*[m_dimension];
    for (int i=0; i<m_dimension; i++)
    {
        m_matrix[i] = snew double[m_dimension];
    }
    m_z = snew double[m_dimension];
    m_zM = snew double[m_dimension];

    // Load the shifted global optimum
    Benchmark::loadRowVectorFromFile(file_data, m_dimension, m_o);
    // Load the matrix
    Benchmark::loadMatrixFromFile(file_m, m_dimension, m_dimension, m_matrix);

} // F14ShiftedRotatedExpandedScaffer


/**
 * Destructor
 */
F14ShiftedRotatedExpandedScaffer::~F14ShiftedRotatedExpandedScaffer()
{
    delete [] m_o;
    for (int i=0; i<m_dimension; i++)
    {
        delete [] m_matrix[i];
    }
    delete [] m_matrix;
    delete [] m_z;
    delete [] m_zM;
} // ~F14ShiftedRotatedExpandedScaffer


/**
 * Function body
 */
double F14ShiftedRotatedExpandedScaffer::f(double * x)
{
    double result = 0.0;

    Benchmark::shift(m_z, x, m_o, m_dimension);
    Benchmark::rotate(m_zM, m_z, m_matrix, m_dimension);

    result = Benchmark::EScafferF6(m_zM, m_dimension);

    result += m_bias;

    return result;
}


std::string F14ShiftedRotatedExpandedScaffer::getFileMxName(std::string prefix, int dimension, std::string suffix)
{
    std::stringstream sstm;
    sstm << prefix << dimension << suffix;
    return sstm.str();
}
