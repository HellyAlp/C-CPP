//MlpNetwork.h

#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Matrix.h"
#include "Digit.h"
#include "Dense.h"

#define MLP_SIZE 4

const MatrixDims imgDims = {28, 28};
const MatrixDims weightsDims[] = {{128, 784}, {64, 128}, {20, 64}, {10, 20}};
const MatrixDims biasDims[]    = {{128, 1}, {64, 1}, {20, 1},  {10, 1}};

/**
 * A class representing the mlp network
 */
class MlpNetwork
{
public:
    /**
     * Constructor - Accepts 2 arrays, size 4 each. one for weights and one for biases. constructs the network described
     * @param weights matrix array of size 4
     * @param biases matrix array of size 4
     */
    MlpNetwork(Matrix weights[MLP_SIZE], Matrix biases[MLP_SIZE]);

    /**
     * Parenthesis operator - Applies the entire network on input
     * @param img - matrix
     * @return digit struct
     */
    Digit operator()(const Matrix& img) const;



private:
    Matrix* _weights;
    Matrix* _biases;
    Dense _firstDense;
    Dense _secondDense;
    Dense _thirdDense;
    Dense _fourthDense;
};

#endif // MLPNETWORK_H
