#include "Dense.h"

/**
 * constractor - Inits a new layer with given parameters
 * @param w matrix
 * @param bias matrix
 * @param activationType (Relu/Softmax)
 */
Dense::Dense(const Matrix w, const Matrix bias,  ActivationType activationType): _w(w), _bias(bias),
            _activationType(activationType)
{
}



/**
 * getter - Returns the weights of this layer
 */
Matrix Dense::getWeights() const
{
    return _w;
}

/**
 * getter- Returns the bias of this layer
 * @return
 */
Matrix Dense::getBias() const
{
    return _bias;
}


/**
 * getter - Returns the activation function of this layer forbids modification
 * @return Activaation class object
 */
Activation Dense::getActivation() const
{
    return Activation(_activationType);
}


/**
 * Parenthesis operator - Applies the layer on input and returns output matrix Layers operate
 * @param m matrix
 * @return result matrix
 */
Matrix Dense::operator()(const Matrix &m) const
{
    Activation activationFun = getActivation();
    Matrix matrix = getWeights() * m + getBias();
    return activationFun(matrix);
}
