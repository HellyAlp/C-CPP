#include "Matrix.h"
#define MATRIX_DIM_ERROR_MSG "Error: matrix dimensions is not valid."
#define DIM_ERROR_MSG "Error: invalid dimensions"
#define READ_FILE_ERROR "Error: there is a problem with the file!"
#define OUTOFBOUND_ERROR "Error: index out of bound"
#define EOF_FILE_ERROR "Error: EOF error"
#define SPACE "  "
#define ASTERISK "**"
#define ONE 1
#define ZERO 0
#define FLOAT_ZERO 0.0f
#define FLOAT_ONE 0.1f

/**
 * Constructs Matrix rows × cols, inits all elements to 0
 * @param rows positive number
 * @param cols positive number
 */
Matrix::Matrix(int rows, int cols)
{
    if(rows <= 0 || cols <= 0)
    {
        std::cerr << MATRIX_DIM_ERROR_MSG << std::endl;
        exit(EXIT_FAILURE);
    }
    _rowsNum = rows;
    _colsNum = cols;
    _matrix = new float[_rowsNum * _colsNum];
    for (int i = 0; i < _rowsNum*_colsNum; ++i)
    {
        _matrix[i] = FLOAT_ZERO;
    }

}

/**
 * Constructs 1×1 Matrix Inits the single element to 0
 */
Matrix::Matrix():Matrix(ONE, ONE)
{
}


/**
 * Constructs matrix from another Matrix m
 * @param m another Matrix
 */
Matrix::Matrix(const Matrix &m) : _rowsNum(m.getRows()), _colsNum(m.getCols())
{
    if( _rowsNum <= ZERO || _colsNum <= ZERO )
    {
        std::cerr << MATRIX_DIM_ERROR_MSG << std::endl;
        exit(EXIT_FAILURE);
    }
    _matrix = new float[m.getRows() *  m.getCols()];
    for(int i = 0; i < m.getRows() * m.getCols(); i++)
    {
        (*this)[i] = m[i];
    }
}

/**
 * destructor, free the memory of a matrix
 */
Matrix::~Matrix()
{
    delete[] _matrix;
}

/**
 * getter- returns the amount of rows as int
 */
int Matrix::getRows() const
{
    return _rowsNum;
}

/**
 * getter -returns the amount of cols as int
 * @return
 */
int Matrix::getCols() const
{
    return _colsNum;
}

/**
 * Prints matrix elements, no return value.
 */
void Matrix::plainPrint() const
{
    for(int i = 0; i < this->_rowsNum; i++)
    {
        for (int j = 0; j < this->_colsNum; j++)
        {
            std::cout << (*this)(i, j) << " " ;
        }
        std::cout << std::endl;
    }
}

/**
 * Transforms a matrix into a column vector.
 * @return
 */
Matrix& Matrix::vectorize()
{
    this->_rowsNum = _rowsNum * _colsNum;
    this->_colsNum = ONE;
    return *this;
}

/**
 * Assignment operator
 * @param rhs
 * @return
 */
Matrix& Matrix::operator=(const Matrix& rhs)
{
    if(this == &rhs)
    {
        return  *this;
    }
    delete[] _matrix;
    _rowsNum = rhs.getRows();
    _colsNum = rhs.getCols();
    _matrix = new float[_rowsNum * _colsNum];
    for(int i = 0; i < (_rowsNum * _colsNum) ; i++)
    {
        _matrix[i] = rhs[i];
    }
    return *this;
}

/**
 * Matrix multiplication
 * @param rhs matrix
 * @return The result matrix
 */
Matrix Matrix::operator*(const Matrix &rhs) const
{
    if(this->getCols() == rhs.getRows() )
    {
        Matrix resultMat =  Matrix(getRows(), rhs.getCols());
        for(int i = 0; i < getRows(); i++)
        {
            for (int j = 0; j < rhs.getCols(); j++)
            {
                float sum = ZERO;
                for (int k = 0; k < _colsNum ; k++)
                {
                    sum += _matrix[i * _colsNum + k] * rhs[k * rhs.getCols() + j];
                }
                resultMat[i * rhs.getCols() + j] = sum;
            }
        }
        return resultMat;
    }
    else
    {
        std::cerr << DIM_ERROR_MSG << std::endl;
        exit(EXIT_FAILURE);
    }
}


/**
 * Scalar multiplication on the right
 * @param c scalar
 * @return The result matrix
 */
Matrix Matrix::operator*(const float c) const
{
    Matrix result = Matrix( getRows(), getCols());
    for(int i = 0; i < getRows() * getCols() ; i++)
    {
        result[i] = _matrix[i] * c;
    }
    return result;
}

/**
 * Scalar multiplication on the left
 * @param c scalar
 * @param rhs matrix
 * @return The result matrix
 */
Matrix operator*( float c, const Matrix &rhs)
{
    Matrix result = Matrix(rhs._rowsNum, rhs._colsNum);
    for(int i = 0; i < rhs._rowsNum * rhs._colsNum ; i++)
    {
            result._matrix[i] = c * rhs._matrix[i] ;
    }
    return result;
}

/**
 * Matrix addition operator
 * @param rhs matrix
 * @return The result matrix
 */
Matrix Matrix::operator+(const Matrix &rhs) const
{
    if(this->getCols() == rhs.getCols() && this->getRows() == rhs.getRows())
    {
        Matrix resultMat =  Matrix(getRows(), getCols());
        for(int i = 0; i < getRows(); i++)
        {
            for (int j = 0; j < getCols(); j++)
            {
                resultMat[i * getCols() + j] = _matrix[i * getCols() + j] +
                                                                 (rhs[i * getCols() + j]);
            }
        }
        return resultMat;
    }
    else
    {
        std::cerr << DIM_ERROR_MSG << std::endl;
        exit(EXIT_FAILURE);
    }
}

/**
* Matrix addition accumulation
* @param rhs matrix
* @return The result matrix
*/
Matrix& Matrix::operator+=(const Matrix &rhs)
{
    *this = *this + rhs;
    return (*this);
}

/**
 * Parenthesis indexing
 * @param i int rows index
 * @param j int col index
 * @return the value in this index in the matrix
 */
float& Matrix::operator()(const int i, const int j)
{
    if(i >= _rowsNum || j >= _colsNum || i < 0 || j < 0)
    {
        std::cerr << OUTOFBOUND_ERROR << std::endl;
        exit(EXIT_FAILURE);
    }
    return _matrix[(i * getCols() ) + j];
}

/**
 * Brackets indexing
 * @param i index in matrix
 * @return the value in this index in the matrix
 */
float& Matrix::operator[](const int i) {
    if(i >= _rowsNum * _colsNum || i < ZERO )
    {
        std::cerr << OUTOFBOUND_ERROR << std::endl;
        exit(EXIT_FAILURE);
    }
    return _matrix[i];
}

/**
 * Parenthesis indexing for const
 * @param i int rows index
 * @param j int col index
 * @return the value in this index in the matrix
 */
float Matrix::operator()(const int i, const int j) const
{
    if(i >= _rowsNum || j >= _colsNum || i < ZERO || j < ZERO)
    {
        std::cerr << OUTOFBOUND_ERROR << std::endl;
        exit(EXIT_FAILURE);
    }
    return _matrix[(i * _colsNum ) + j];
}


/**
 * Brackets indexing  for const
 * @param i index in matrix
 * @return the value in this index in the matrix
 */
float Matrix::operator[](const int i) const
{
    if(i >= _rowsNum * _colsNum || i < ZERO )
    {
        std::cerr << OUTOFBOUND_ERROR << std::endl;
        exit(EXIT_FAILURE);
    }
    return _matrix[i];
}

/**
 * Fills matrix elements has to read input stream fully otherwise, that’s an error
 * @param in Input stream
 * @param m matrix
 * @return Input stream
 */
std::istream& operator>>(std::istream &input, Matrix &m)
{
    for (int i = 0; i < m.getRows() * m.getCols(); i++)
    {
        input.read((char*) &(m[i]), sizeof(float));
        if(!input.good())
        {
            std::cerr << READ_FILE_ERROR << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    if(input.peek() != EOF)
    {
        std::cerr << EOF_FILE_ERROR << std::endl;
        exit(EXIT_FAILURE);
    }
    return input;
}

/**
 * export of matrix, prints the matrix according to the instruction
 * @param out Output stream
 * @param m matrix
 * @return Output stream
 */
std::ostream &operator<<(std::ostream &output, const Matrix &m)
{
    for (int i = ZERO; i < m.getRows(); ++i)
    {
        for (int j = 0; j < m.getCols(); ++j)
        {
            if(m[i * m.getCols() + j ] <= FLOAT_ONE)
            {
                output << SPACE;
            }
            else
            {
                output << ASTERISK;
            }
        }
        output << std::endl;
    }
    return output;
}




