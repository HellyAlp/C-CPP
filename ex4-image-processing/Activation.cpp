#include "Activation.h"
#include <cmath>
#define ACTIVATION_ERROR "ERROR: invalid activation type"
#define ZERO 0
#define ONE 1

/**
 * Constructor- Accepts activation type (Relu/Softmax) and defines this instace’s activation accordingly
 * @param actType Relu/Softmax Activation
 */
Activation::Activation(ActivationType actType): _activationType(actType)
{
}

/**
 * Returns this activation’s type(Relu/Softmax)
 */
ActivationType Activation::getActivationType()
{
    return _activationType;
}

/**
 * Parenthesis operator, Applies activation function on input.
 * @param m matrix
 * @return A result matrix
 */
Matrix Activation::operator()( Matrix &m) const
{
    if(_activationType == Relu)
    {
        return _reluAct(m);
    }
    else if (_activationType == Softmax)
    {
        return _softMaxAct(m);
    }
    else
    {
        std::cerr << ACTIVATION_ERROR << std::endl;
        exit(EXIT_FAILURE);
    }
}

/**
 * A function that calculates relu activation.
 * @param m matrix
 * @return the result matrix
 */
Matrix Activation::_reluAct(const Matrix &m) const
{
    Matrix resultMat = Matrix(m);
    for (int i = 0; i < m.getRows() * m.getCols(); ++i)
    {
        if(m[i] < ZERO )
        {
            resultMat[i] = ZERO;
        }
    }
    return resultMat;
}

/**
 * A function that calculates softmax activation.
 * @param m matrix
 * @return the result matrix
 */
Matrix Activation::_softMaxAct(const Matrix& m) const
{
    Matrix resultMat = Matrix(m);
    float sumOfExp  = ZERO;
    for (int i = 0; i < m.getCols() * m.getRows() ; i++)
    {
        resultMat[i] = std::exp(m[i]);
        sumOfExp += resultMat[i];
    }
    return   (ONE / sumOfExp)  *  resultMat; ;
}




