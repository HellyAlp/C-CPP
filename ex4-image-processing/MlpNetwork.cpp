#include "MlpNetwork.h"
#define ONE 1
#define ZERO 0
#define TWO 2
#define THREE 3
/**
 *
 * @param weights
 * @param biases
 */
MlpNetwork::MlpNetwork(Matrix weights[MLP_SIZE], Matrix biases[MLP_SIZE]): _weights(weights), _biases(biases),
                                                            _firstDense(Dense(_weights[ZERO], _biases[ZERO],
                                                                    Relu)),
                                                            _secondDense(Dense(_weights[ONE], _biases[ONE],
                                                                    Relu)),
                                                            _thirdDense(Dense(_weights[TWO], _biases[TWO],
                                                                    Relu)),
                                                            _fourthDense( Dense(_weights[THREE], _biases[THREE],
                                                                    Softmax))

{
}

/**
 * Parenthesis operator - Applies the entire network on input
 * @param img - matrix
 * @return digit struct
 */
Digit MlpNetwork::operator()(const Matrix &img) const
{
    Matrix activateFirstDense =  _firstDense(img);
    Matrix activateSecondDense = _secondDense(activateFirstDense);
    Matrix activateThirdDense = _thirdDense(activateSecondDense);
    Matrix result = _fourthDense(activateThirdDense);
    unsigned int index = ZERO;
    float max = ZERO;
    for (int i = 0; i < result.getRows(); i++)
    {
            if(max < result[i])
            {
                max = result[i];
                index = i;
            }
    }
    Digit digit{index, max};
    return digit;
}



