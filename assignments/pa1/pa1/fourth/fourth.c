#include <stdio.h>
#include <stdlib.h>

int** multMatrix(int**, int**, int, int, int, int);
int printMatrix(int**, int, int);
int freeMatrix(int**, int);

int main(int argc, char** argv) {
    FILE* fp = fopen(argv[1], "r");

    if (fp != 0) {
        // char** temp;
        int** mat1, **mat2, **matProd;
        int rows1, rows2, cols1, cols2;

        fscanf(fp, "%d\t%d\n", &rows1, &cols1); // First matrix
      
        mat1 = calloc(rows1, sizeof(int*)); 
        for (int i = 0; i < rows1; i++) {
            mat1[i] = calloc(cols1, sizeof(int));
            
            for (int j = 0; j < cols1; j++) {
                if (j == cols2 - 1)
                    fscanf(fp, "%d\n", &(mat1[i][j]));
                else
                    fscanf(fp, "%d\t", &(mat1[i][j]));
            }
        }

        fscanf(fp, "%d\t%d\n", &rows2, &cols2); // Second matrix

        mat2 = calloc(rows2, sizeof(int*)); 
        for (int i = 0; i < rows2; i++) {
            mat2[i] = calloc(cols2, sizeof(int));
            
            for (int j = 0; j < cols2; j++) {
                if (j == cols2 - 1)
                    fscanf(fp, "%d\n", &(mat2[i][j]));
                else
                    fscanf(fp, "%d\t", &(mat2[i][j]));
            }
        }  

        matProd = multMatrix(mat1, mat2, rows1, rows2, cols1, cols2); // Multiply matrix
       
        if (matProd != 0) {
            printMatrix(matProd, rows1, cols2);
            freeMatrix(matProd, rows1);
        } else
            printf("%s\n", "bad-matrices");
    
        fclose(fp);
    
        freeMatrix(mat1, rows1);
        freeMatrix(mat2, rows2);
    }

    return 0;
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
