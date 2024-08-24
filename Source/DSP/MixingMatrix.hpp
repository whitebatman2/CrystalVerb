//
//  MixingMatrix.hpp
//  Miłosz Kolenderski
//

#ifndef MixingMatrix_hpp
#define MixingMatrix_hpp

#include <cmath>


namespace MixingMatrix
{
    enum class MatrixType {
        Hadamard,
        Householder
    };

    inline void hadamardInPlace(double *data, int matrixOrder)
    {
        double a, b;
        int halfN = matrixOrder / 2;
        int N = matrixOrder;

        while (halfN >= 1)
        {
            for (int offset = 0; offset < matrixOrder; offset += N)
            {
                for (int i = offset; i < offset + halfN; ++i)
                {
                    // Butterfly
                    a = data[i];
                    b = data[i + halfN];

                    data[i] = a + b;
                    data[i + halfN] = a - b;
                }
            }

            N = halfN;
            halfN /= 2;
        }

        double normFactor = 1 / std::sqrt(matrixOrder);
        for (int i = 0; i < matrixOrder; ++i)
        {
            data[i] *= normFactor;
        }
    }

    inline void householderInPlace(double *data, int matrixOrder)
    {
        double sum = 0.f;
        for (int i = 0; i < matrixOrder; ++i)
        {
            sum += data[i];
        }

        sum *= 2. / matrixOrder;

        for (int i = 0; i < matrixOrder; ++i)
        {
            data[i] -= sum;
        }
    }
};

#endif /* MixingMatrix_hpp */
