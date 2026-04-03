/*
ASSIGNMENT - 4 :-
This assignment is to explore functions, recursive functions, passing values to functions and
function pointers.

P.S.- 3 : The power of any arbitrary square matrix A, denoted as A , for a nonnegative integer n,
is defined as the matrix product of n copies of A. This means A^n | A is
n = A × A ×... × A
multiplied n times. Note that, a matrix to the zeroth power is defined to be the identity
matrix of the same dimensions, i.e. A^0 = I. Given a square matrix A and a nonnegative
integer n, write a program that can recursively compute A^N involving minimal multiplications.

*/

// solution ---> 

#include <stdio.h>
#include <stdlib.h>

long long **allocateMatrix(int size)
{
    long long **matrix = (long long **)malloc(size * sizeof(long long *));
    if (matrix == NULL)
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    for (int i = 0; i < size; i++)
    {
        matrix[i] = (long long *)malloc(size * sizeof(long long));
        if (matrix[i] == NULL)
        {
            printf("memory allocation failed!\n");
            exit(1);
        }
    }
    return matrix;
}

void freeMatrix(long long **matrix, int size)
{
    for (int i = 0; i < size; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}

void printMatrix(long long **matrix, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("[   ");
        for (int j = 0; j < size; j++)
        {
            printf("%lld    ", matrix[i][j]);
        }
        printf("]\n");
    }
}

void multiply(long long **A, long long **B, long long **result, int size)
{

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            result[i][j] = 0;
            for (int k = 0; k < size; k++)
            {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

typedef void (*MultiplyFunc)(long long **, long long **, long long **, int);

void powerMatrix(long long **A, int n, long long **result, int size, MultiplyFunc multFunc)
{

    if (n == 0)
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                result[i][j] = (i == j) ? 1 : 0;
            }
        }
        return;
    }

    if (n == 1)
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                result[i][j] = A[i][j];
            }
        }
        return;
    }

    long long **temp = allocateMatrix(size);

    powerMatrix(A, n / 2, temp, size, multFunc);

    multFunc(temp, temp, result, size);

    if (n % 2 != 0)
    {
        long long **tempOdd = allocateMatrix(size);

        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                tempOdd[i][j] = result[i][j];
            }
        }

        multFunc(tempOdd, A, result, size);
        freeMatrix(tempOdd, size);
    }

    freeMatrix(temp, size);
}

int main()
{
    int size, n;

    printf("enter the dimension of the square matrix (e.g. 2 for 2x2): ");
    if (scanf("%d", &size) != 1 || size <= 0)
    {
        printf("invalid matrix size.\n");
        return 1;
    }

    printf("enter the non-negative integer power 'n' to raise the matrix to: ");
    if (scanf("%d", &n) != 1 || n < 0)
    {
        printf("invalid power. Must be a non-negative integer.\n");
        return 1;
    }

    long long **A = allocateMatrix(size);
    long long **result = allocateMatrix(size);

    printf("enter the elements of the %dx%d matrix row by row:\n", size, size);
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            scanf("%lld", &A[i][j]);
        }
    }

    printf("\noriginal Matrix A:\n");
    printMatrix(A, size);

    powerMatrix(A, n, result, size, multiply);

    printf("\nmatrix A raised to the power %d:\n", n);
    printMatrix(result, size);

    freeMatrix(A, size);
    freeMatrix(result, size);

    return 0;
}