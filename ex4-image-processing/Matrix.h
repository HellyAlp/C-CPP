// Matrix.h

#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
/**
 * @struct MatrixDims
 * @brief Matrix dimensions container
 */
typedef struct MatrixDims
{
    int rows, cols;
} MatrixDims;

/**
 * A class repersenting a matrix
 */
class Matrix
{
public:
    /**
     * Constructs Matrix rows × cols, inits all elements to 0
     * @param rows positive number
     * @param cols positive number
     */
    Matrix(int rows, int cols);

    /**
     * Constructs 1×1 Matrix Inits the single element to 0
     */
    Matrix();

    /**
     * Constructs matrix from another Matrix m
     * @param m another Matrix
     */
    Matrix(const Matrix &m);

    /**
     * destructor, free the memory of a matrix
     */
    ~Matrix();

    /**
     * getter- returns the amount of rows as int
     */
    int getCols() const ;

    /**
     * getter -returns the amount of cols as int
     * @return
     */
    int getRows() const;

    /**
     * Prints matrix elements, no return value.
     */
    void plainPrint() const ;

    /**
     * Transforms a matrix into a column vector.
     * @return
     */
    Matrix& vectorize();

    /**
     * Assignment operator
     * @param rhs
     * @return
     */
    Matrix& operator=(const Matrix &rhs);

    /**
     * Matrix multiplication
     * @param rhs matrix
     * @return The result matrix
     */
    Matrix operator*(const Matrix &rhs) const ;

    /**
     * Scalar multiplication on the right
     * @param c scalar
     * @return The result matrix
     */
    Matrix operator*( float c) const ;

    /**
     * Scalar multiplication on the left
     * @param c scalar
     * @param rhs matrix
     * @return The result matrix
     */
    friend Matrix operator*( float c, const Matrix &rhs);

    /**
     * Matrix addition operator
     * @param rhs matrix
     * @return The result matrix
     */
    Matrix operator+(const Matrix &rhs) const;

    /**
     * Matrix addition accumulation
     * @param rhs matrix
     * @return The result matrix
     */
    Matrix& operator+=(const Matrix &rhs);

    /**
     * Parenthesis indexing
     * @param i int rows index
     * @param j int col index
     * @return the value in this index in the matrix
     */
    float& operator()( const int i, const  int j) ;

    /**
     * Parenthesis indexing for const
     * @param i int rows index
     * @param j int col index
     * @return the value in this index in the matrix
     */
    float operator()( const int i, const  int j)  const ;

    /**
     * Brackets indexing
     * @param i index in matrix
     * @return the value in this index in the matrix
     */
    float& operator[](const int i) ;

    /**
     * Brackets indexing  for const
     * @param i index in matrix
     * @return the value in this index in the matrix
     */
    float operator[]( const int i) const;

    /**
     * Fills matrix elements has to read input stream fully otherwise, that’s an error
     * @param in Input stream
     * @param m matrix
     * @return Input stream
     */
    friend std::istream &operator>>(std::istream &in, Matrix& m);

    /**
     * export of matrix, prints the matrix according to the instruction
     * @param out Output stream
     * @param m matrix
     * @return Output stream
     */
    friend std::ostream&operator<<(std::ostream &out, const Matrix& m);


private:
    int _rowsNum;
    int _colsNum;
    float* _matrix;

};

#endif //MATRIX_H
