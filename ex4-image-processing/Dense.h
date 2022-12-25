#ifndef EX4_DENSE_H
#define EX4_DENSE_H

#include "Matrix.h"
#include "Activation.h"

/**
 * A class representing a dense in the network
 */
class Dense
{
public:
    /**
     * constractor - Inits a new layer with given parameters
     * @param w matrix
     * @param bias matrix
     * @param activationType (Relu/Softmax)
     */
    Dense(const Matrix w, const Matrix bias, ActivationType activationType);

    /**
     * getter - Returns the weights of this layer
     */
    Matrix getWeights() const;

    /**
     * getter- Returns the bias of this layer
     * @return
     */
    Matrix getBias() const ;


    /**
     * getter - Returns the activation function of this layer forbids modification
     * @return Activaation class object
     */
    Activation getActivation() const;

    /**
     * Parenthesis operator - Applies the layer on input and returns output matrix Layers operate
     * @param m matrix
     * @return result matrix
     */
    Matrix operator()(const Matrix& m) const;


private:
    Matrix _w;
    Matrix _bias;
    ActivationType _activationType;

};

#endif //EX4_DENSE_H
