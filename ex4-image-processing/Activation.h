//Activation.h
#ifndef ACTIVATION_H
#define ACTIVATION_H

#include "Matrix.h"

/**
 * @enum ActivationType
 * @brief Indicator of activation function.
 */
enum ActivationType
{
    Relu,
    Softmax
};

/**
 * @brief  A class representing Activation Function
 */
class Activation
{
public:
    /**
     * Constructor- Accepts activation type (Relu/Softmax) and defines this instace’s activation accordingly
     * @param actType Relu/Softmax Activation
     */
    Activation(ActivationType actType);

    /**
     * Returns this activation’s type(Relu/Softmax)
     */
    ActivationType getActivationType();

    /**
     * Parenthesis operator, Applies activation function on input.
     * @param m matrix
     * @return A result matrix
     */
    Matrix operator()(  Matrix& m) const ;

private:
    ActivationType _activationType;

    /**
     * A function that calculates relu activation.
     * @param m matrix
     * @return the result matrix
     */
     Matrix  _reluAct(const Matrix &m) const ;

    /**
     * A function that calculates softmax activation.
     * @param m matrix
     * @return the result matrix
     */
     Matrix  _softMaxAct(const Matrix &m) const;
};

#endif //ACTIVATION_H
