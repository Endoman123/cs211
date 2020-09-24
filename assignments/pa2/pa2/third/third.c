#include <stdlib.h>
#include <stdio.h>

double **multMatrix(double **, double **, int, int, int, int);
double **transpose(double **, int, int);
double **invert(double **, int, int);

void scaleRow(double **, int, double, int);
void addRows(double **, int, int, double, int);

double **calcWeights(double **, double **, int, int);

int printMatrix(double **, int, int);
int freeMatrix(double **, int);

int main(int argc, char **argv) { 
    FILE *fp1 = fopen(argv[1], "r"), *fp2 = fopen(argv[2], "r");

    if (fp1 != 0 && fp2 != 0) {
        // NOTES:
        // Training Data: Nx(K + 1)
        // Output: Nx1
        // Weights: (K+1)x1
        int i, j, nRows, nCols;
        double **x = calloc(nRows, sizeof(double *)), **y = calloc(nRows, sizeof(double *)), **weights, **test, **result;
        
        // Step 1: calculate weights
        fscanf(fp1, "%d\n%d\n", &nCols, &nRows);
        nCols += 1;

        for (i = 0; i < nRows; i++) {
            x[i] = calloc(nCols, sizeof(double));
            y[i] = malloc(sizeof(double));

            x[i][0] = 1;
            
            for (j = 1; j < nCols; j++)
                fscanf(fp1, "%lf,", x[i] + j);

            fscanf(fp1, "%lf\n", y[i]);
        }
        
        weights = calcWeights(x, y, nRows, nCols);

        // Step 2: apply ML to test values
        fscanf(fp2, "%d\n", &nRows);
        nCols -= 1;
        test = calloc(nRows, sizeof(double *));
        
        for (i = 0; i < nRows; i++) {
            test[i] = calloc(nCols, sizeof(double));
            
            for (j = 0; j < nCols; j++)
                fscanf(fp2, j < nCols - 1 ? "%lf," : "%lf\n", test[i] + j); 
        }
        
        result = multMatrix(test, weights, nRows, nCols, nCols, 1);
        printMatrix(result, nRows, 1);

    }   

    return 0;
}

/**
 * Multiplies two matrices together
 * 
 * Returns the matrix product, or null if the matrices are not compatible
 */
double **multMatrix(double **mat1, double **mat2, int rows1, int cols1, int rows2, int cols2) {
    double** ret = NULL;

    // Begin multiplying only if matrices are valid
    if (cols1 == rows2) {
        ret = calloc(rows1, sizeof(double *));

        for (int i = 0; i < rows1; i++) {
            ret[i] = calloc(cols2, sizeof(double));
            
            for (int j = 0; j < cols2; j++) { // Loop for every element in the product matrix
                for (int r = 0; r < cols1; r++) // Get the product for the element
                    ret[i][j] += mat1[i][r] * mat2[r][j];
            }
        } 
    }

    return ret;   
}

/**
 * Transposes a matrix
 *
 * Does not modify the source matrix.
 */
double **transpose(double **src, int nRows, int nCols) {
    double **ret = calloc(nCols, sizeof(double *));
    int i, j;

    for (i = 0; i < nCols; i++) {
        ret[i] = calloc(nRows, sizeof(double));

        for (j = 0; j < nRows; j++)
            ret[i][j] = src[j][i];
    }

    return ret;
}

/**
 * Inverts the matrix, if possible.
 * 
 * Does not modify the source matrix.
 */
double **invert(double **src, int nRows, int nCols) {
    double **ret = NULL;

    if (nRows == nCols) {
        double **srcCopy = calloc(nRows, sizeof(double *));
        ret = calloc(nRows, sizeof(double *));
        int i, j;

        // Initialize both srcCopy and ret
        for (i = 0; i < nRows; i++) {
            srcCopy[i] = calloc(nCols, sizeof(double));
            ret[i] = calloc(nCols, sizeof(double));
            
            for (j = 0; j < nCols; j++) {
                srcCopy[i][j] = src[i][j];
                ret[i][j] = i == j;
            }
        }

        // Reduce to Row Echelon form
        for (i = 0; i < nRows; i++) { 
            // Scale row to make pivot 1 
            double scale = srcCopy[i][i];

            scaleRow(srcCopy, i, 1.0 / scale, nCols);
            scaleRow(ret, i, 1.0 / scale, nCols);

            for (j = i + 1; j < nRows; j++) {
                scale = srcCopy[j][i] / srcCopy[i][i];
                
                addRows(srcCopy, i, j, -scale, nCols);
                addRows(ret, i, j, -scale, nCols);
            }
        }


        // Reduced Row Echelon form
        for (i = nRows - 1; i >= 0; --i) {
            // Make values above pivots 0
            for (j = i - 1; j >= 0; --j) {
                double scale = srcCopy[j][i];

                addRows(srcCopy, i, j, -scale, nCols); 
                addRows(ret, i, j, -scale, nCols);
            }
        }
    }

    return ret;
}

/**
 * Scales a row by a value
 *
 * Modifies the source
 */
void scaleRow(double **src, int row, double scale, int nCols) {
    int i = 0;

    for (i = 0; i < nCols; i++)
        src[row][i] *= scale;

    return; 
}

/**
 * Adds one row to another row 
 *
 * Modifies the source.
 */
void addRows(double **src, int srcRow, int destRow, double scale, int nCols) {
    int i = 0;

    for (i = 0; i < nCols; i++)
        src[destRow][i] += scale * src[srcRow][i];

    return;
}

/**
 * Calculate the weights for one-shot learning
 */
double **calcWeights(double **x, double **y, int nRows, int nCols) {
    double **ret = calloc(nRows, sizeof(double *)), **xTrans = transpose(x, nRows, nCols), **xSrc, **xDest;

    // X^T . X;
    // (N x M) . (M x N) = (N x N)
    xDest = multMatrix(xTrans, x, nCols, nRows, nRows, nCols);

    // (X^T . X)^-1
    // N x N
    xSrc = xDest;
    xDest = invert(xSrc, nCols, nCols);
    freeMatrix(xSrc, nCols);

    printMatrix(xDest, nCols, nCols);

    // (X^T . X)^-1 . X^T
    // (N x N) . (N x M) = (N x M)
    xSrc = xDest;
    xDest = multMatrix(xSrc, xTrans, nCols, nCols, nCols, nRows);
    freeMatrix(xSrc, nCols);

    // (X^T . X)^-1 . X^T . Y
    // (N x M) . (M x 1) = (N x 1)
    xSrc = xDest;
    ret = multMatrix(xSrc, y, nCols, nRows, nRows, 1);
    freeMatrix(xSrc, nCols);

    freeMatrix(xTrans, nCols);

    return ret;
}

/**
 * Prints the matrix
 */
int printMatrix(double** matrix, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) // Loop for every element in the product matrix
            printf("%0.0lf\t", matrix[i][j]); 

        printf("\n");
    }

    return 0;
}

/**
 * Frees the matrix from memory
 */
int freeMatrix(double** matrix, int rows) {
    for (int i = 0; i < rows; ++i)
        free(matrix[i]);    
   
    free(matrix); 

    return 0;
}
