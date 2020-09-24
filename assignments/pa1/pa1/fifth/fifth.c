#include <stdio.h>
#include <stdlib.h>

int** multMatrix(int**, int**, int, int, int, int);
int isMagic(int**, int);
int printMatrix(int**, int, int);
int freeMatrix(int**, int);

int main(int argc, char** argv) {
    FILE* fp = fopen(argv[1], "r");

    if (fp != 0) {
        int **mat;
        int dimSide;

        fscanf(fp, "%d\n", &dimSide); // First matrix
      
        mat = calloc(dimSide, sizeof(int*)); 
        for (int i = 0; i < dimSide; i++) {
            mat[i] = calloc(dimSide, sizeof(int));
            
            for (int j = 0; j < dimSide; j++) {
                if (j == dimSide - 1)
                    fscanf(fp, "%d\n", &(mat[i][j]));
                else
                    fscanf(fp, "%d\t", &(mat[i][j]));
            }
        }
    
        fclose(fp);

        printf("%s\n", isMagic(mat, dimSide) ? "magic" : "not-magic");
    }


    return 0;
}

/**
 * Gets whether or not the given matrix is a magic matrix;
 * that is, a square matrix whose rows, columns, and diagonals
 * all equal the same sum.
 */
int isMagic(int** mat, int dimSide) {
    // Start by checking if the matrix has distinct values
    for (int i = 0; i < dimSide * dimSide; i++) {
        for (int j = 0; j < dimSide * dimSide; j++) {
            if (i != j && mat[i / dimSide][i % dimSide] == mat[j / dimSide][j % dimSide])
                return 0;
        }
    }

    // Using matrix multiplication,
    // we can quickly sum all the columns and rows
    // by multiplying the matrix with the elementary
    // vectors
    int **eCol = NULL, **eRow = NULL, **sumCols = NULL, **sumRows = NULL;
    int sum = 0, sumD1 = 0, sumD2 = 0; // reference sum, diagonal sums 

    eCol = calloc(dimSide, sizeof(int*));
    eRow = malloc(sizeof(int*));
    eRow[0] = calloc(dimSide, sizeof(int));

    
    for (int i = 0; i < dimSide; ++i) {
        eCol[i] = malloc(sizeof(int));

        *eCol[i] = 1;
        eRow[0][i] = 1;
    }

    sumCols = multMatrix(eRow, mat, 1, dimSide, dimSide, dimSide); 
    sumRows = multMatrix(mat, eCol, dimSide, dimSide, dimSide, 1);

    free(eRow[0]);
    free(eRow);
    free(eCol); 

    sum = sumCols[0][0];

    for (int i = 0; i < dimSide; ++i) {
        if (sumCols[0][i] != sum || sumRows[i][0] != sum)
            return 0;
        
        sumD1 += mat[i][i];
        sumD2 += mat[i][dimSide - 1 - i];
    }

    // If the first diagonal's sum does not match the reference sum
    // or the first diagonal's sum does not match the second diagonal's sum,
    // no magic
    if (sumD1 != sum || sumD1 != sumD2)
        return 0;

    return 1;
}

/**
 * Multiplies two matrices together
 * 
 * Returns the matrix product, or null if the matrices are not compatible
 */
int** multMatrix(int** mat1, int** mat2, int rows1, int cols1, int rows2, int cols2) {
    int** ret = NULL;

    // Begin multiplying only if matrices are valid
    if (cols1 == rows2) {
        ret = calloc(rows1, sizeof(int*));

        for (int i = 0; i < rows1; i++) {
            ret[i] = calloc(cols2, sizeof(int));
            
            for (int j = 0; j < cols2; j++) { // Loop for every element in the product matrix
                for (int r = 0; r < cols1; r++) // Get the product for the element
                    ret[i][j] += mat1[i][r] * mat2[r][j];
            }
        } 
    }

    return ret;   
}

/**
 * Prints the matrix
 */
int printMatrix(int** matrix, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) { // Loop for every element in the product matrix
            printf("%d\t", matrix[i][j]); 
        }

        if (i < rows - 1)
            printf("\n");
    }

    return 0;
}

/**
 * Frees the matrix from memory
 */
int freeMatrix(int** matrix, int rows) {
    for (int i = 0; i < rows; ++i)
        free(matrix[i]);    
   
    free(matrix); 

    return 0;
}
