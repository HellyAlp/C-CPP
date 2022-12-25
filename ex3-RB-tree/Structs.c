#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "Structs.h"
#define ACTION_SUCCEEDED 1
#define GREATER 1
#define SMALLER -1
#define EQUALS 0
#define ACTION_NOT_SUCCEEDED 0
#define POW 2

/**
 * CompFunc for strings (assumes strings end with "\0")
 * @param a - char* pointer
 * @param b - char* pointer
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a. (lexicographic
 * order)
 */
int stringCompare(const void *a, const void *b)
{
    int compResult = strcmp(a, b);
    return compResult;
}

/**
 * ForEach function that concatenates the given word and \n to pConcatenated. pConcatenated is
 * already allocated with enough space.
 * @param word - char* to add to pConcatenated
 * @param pConcatenated - char*
 * @return 0 on failure, other on success
 */
int concatenate(const void *word, void *pConcatenated)
{

    if(strcat(pConcatenated, word) == NULL)
    {
        return ACTION_NOT_SUCCEEDED;
    }

    if(strcat(pConcatenated, "\n") == NULL)
    {
        return ACTION_NOT_SUCCEEDED;
    }
    return ACTION_SUCCEEDED;
}

/**
 * FreeFunc for strings
 */
void freeString(void *s)
{
    free(s);
}

/**
 * CompFunc for Vectors, compares element by element, the vector that has the first larger
 * element is considered larger. If vectors are of different lengths and identify for the length
 * of the shorter vector, the shorter vector is considered smaller.
 * @param a - first vector
 * @param b - second vector
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a.
 */
int vectorCompare1By1(const void *a, const void *b)
{
    const Vector* vectorA = a;
    const Vector* vectorB = b;
    if(vectorA == NULL)
    {
        if(vectorB == NULL)
        {
            return EQUALS;
        }
        return SMALLER;
    }
    if(vectorB == NULL)
    {
        return GREATER;
    }
    int i = 0;
    while(i < vectorA->len && i < vectorB->len)
    {
        if(vectorA->vector[i] > vectorB->vector[i])
        {
            return GREATER;
        }
        if(vectorA->vector[i] < vectorB->vector[i])
        {
            return SMALLER;
        }
        i++;
    }
    if(vectorA->len > vectorB->len)
    {
        return GREATER;
    }
    else if (vectorA->len < vectorB->len )
    {
        return SMALLER;
    }
    else
    {
        return EQUALS;
    }

}

/**
 * FreeFunc for vectors
 */
void freeVector(void *pVector)
{
    Vector* vector = (Vector*) pVector;
    free(vector->vector);
    free(vector);
}

/**
 * A method that clculates a norm of a vector
 * @param vectorA
 * @return the norm
 */
double calculateNorm(const Vector* vectorA)
{
    double sumOfSquares = 0;
    int i = 0;
    while (i < vectorA->len)
    {
        sumOfSquares += pow(vectorA->vector[i], POW);
        i++;
    }
    return sqrt(sumOfSquares);

}

/**
 * copy pVector to pMaxVector if : 1. The norm of pVector is greater then the norm of pMaxVector.
 * 								   2. pMaxVector->vector == NULL.
 * @param pVector pointer to Vector
 * @param pMaxVector pointer to Vector that will hold a copy of the data of pVector.
 * @return 1 on success, 0 on failure (if pVector == NULL || pMaxVector==NULL: failure).
 */
int copyIfNormIsLarger(const void *pVector, void *pMaxVector)
{
    const Vector* vector1 = pVector;
    Vector* vector2 = pMaxVector;

    if(vector1 == NULL || vector2 == NULL)
    {
        return ACTION_NOT_SUCCEEDED;
    }
    double norm1 = calculateNorm(vector1);
    double norm2 = calculateNorm(vector2);
    if(vector2->vector == NULL || norm1 > norm2)
    {
        vector2->vector = realloc(vector2->vector, vector1->len*sizeof(double));
        memcpy(vector2->vector, vector1->vector, (vector1->len)*sizeof(double));
        vector2->len = vector1->len;
    }
    return  ACTION_SUCCEEDED;
}

/**
 * This function allocates memory it does not free.
 * @param tree a pointer to a tree of Vectors
 * @return pointer to a *copy* of the vector that has the largest norm (L2 Norm), NULL on failure.
 */
Vector *findMaxNormVectorInTree(RBTree *tree)
{
    Vector * vector = (Vector*) malloc(sizeof(Vector));
    vector->len = 0;
    vector->vector = NULL;
    forEachRBTree(tree, copyIfNormIsLarger, vector);
    return vector;
}


