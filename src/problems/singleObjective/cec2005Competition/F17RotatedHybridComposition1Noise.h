//  F17RotatedHybridComposition1Noise.h
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

#ifndef __F17_ROTATED_HYBRID_COMPOSITION_1_NOISE__
#define __F17_ROTATED_HYBRID_COMPOSITION_1_NOISE__

#include <Benchmark.h>
#include <HCJob.h>
#include <TestFunc.h>

class F17RotatedHybridComposition1Noise : public TestFunc
{

private:

    // Fixed (class) parameters
    static const std::string FUNCTION_NAME;
    static const std::string DEFAULT_FILE_DATA;
    static const std::string DEFAULT_FILE_MX_PREFIX;
    static const std::string DEFAULT_FILE_MX_SUFFIX;

    // Number of functions
    static const int NUM_FUNC = 10;

    // Shifted global optimum
    double ** m_o;
    double *** m_M;
    static const double m_sigma[NUM_FUNC];
    static const double m_lambda[NUM_FUNC];
    static const double m_func_biases[NUM_FUNC];
    double * m_testPoint;
    double * m_testPointM;
    double * m_fmax;

    // In order to avoid excessive memory allocation,
    // a fixed memory buffer is allocated for each function object.
    double * m_w;
    double ** m_z;
    double ** m_zM;

    class MyHCJob : public HCJob
    {
    public:
        MyHCJob(int numFunc);
        double basic_func(int func_no, double* x, int length);
    };

    MyHCJob * theJob;

    std::string getFileMxName(std::string prefix, int dimension, std::string suffix);

public:

    F17RotatedHybridComposition1Noise(int dimension, double bias);
    F17RotatedHybridComposition1Noise(int dimension, double bias, std::string file_data, std::string file_m);
    ~F17RotatedHybridComposition1Noise();

    double f (double * x);

}; // F17RotatedHybridComposition1Noise

#endif /* __F17_ROTATED_HYBRID_COMPOSITION_1_NOISE__ */
