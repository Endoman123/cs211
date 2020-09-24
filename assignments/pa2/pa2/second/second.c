#include <stdlib.h>
#include <stdio.h>

int solveHexadoku(char [16][16]);
int convertToDecimal(char);
char convertToHex(int);

int main(int argc, char **argv) { 
    FILE *fp = fopen(argv[1], "r");
    char board[16][16];

    if (fp != 0) {
        for (int i = 0; i < 16; i++)
            fscanf(fp, "%c\t%c\t%c\t%c\t%c\t%c\t%c\t%c\t%c\t%c\t%c\t%c\t%c\t%c\t%c\t%c\n",
                    board[i],
                    board[i] + 1,
                    board[i] + 2,
                    board[i] + 3,
                    board[i] + 4,
                    board[i] + 5,
                    board[i] + 6,
                    board[i] + 7,
                    board[i] + 8,
                    board[i] + 9,
                    board[i] + 10,
                    board[i] + 11,
                    board[i] + 12,
                    board[i] + 13,
                    board[i] + 14,
                    board[i] + 15
            );

        if (solveHexadoku(board)) {
            for (int i = 0; i < 16; i++)
                printf("%c\t%c\t%c\t%c\t%c\t%c\t%c\t%c\t%c\t%c\t%c\t%c\t%c\t%c\t%c\t%c\n",
                        board[i][0],
                        board[i][1],
                        board[i][2],
                        board[i][3],
                        board[i][4],
                        board[i][5],
                        board[i][6],
                        board[i][7],
                        board[i][8],
                        board[i][9],
                        board[i][10],
                        board[i][11],
                        board[i][12],
                        board[i][13],
                        board[i][14],
                        board[i][15]
                );
        } else {
            printf("no-solution\n");
        }
    }
    
    return 0;
}

/**
 * If possible to solve, solves the board by filling in the missing pieces.
 * Returns 1 if successful, 0 otherwise
 */ 
int solveHexadoku(char board[16][16]) {
    int row = 0, col = 0;

    // Iterate through entire board
    // Look for missing numbers
    for (row = 0; row < 16; ++row) {
        for (col = 0; col < 16; ++col) {
            if (board[row][col] == '-') {
                char missing = '-';
                int i = 0, j = 0;
                
                // Figure out which number is missing
                for (i = 0; i < 16 && missing == '-'; i++) {
                    char curSearch = convertToHex(i);

                    for (j = 0; j < 16; j++) {
                        if (board[row][j] == curSearch)
                            break;
                    }

                    if (board[row][j] != curSearch)
                        missing = curSearch;
                }

                // With the missing piece, start checking if it fits
                for (i = 0; i < 16; i++) {
                    if (board[i][col] == missing || board[row / 4 * 4 + i / 4][col / 4 * 4 + i % 4] == missing)
                        return 0;
                } 

                // If it fits, place it
                board[row][col] = missing;
            }
        }
    }

    return 1;
}

/**
 * Converts the hex character to a decimal.
 * If specified character is not a hex digit,
 * returns -1.
 */
int convertToDecimal(char c) {
    char *hex = "0123456789ABCDEF";
    int i = 0;

    for (i = 0; i < 16; ++i) {
        if (hex[i] == c)
            return i;
    }

    return -1;
}

/**
 * Converts a decimal number to a hex char.
 * If number is not such that 0 <= x < 16,
 * returns NULL character
 */
char convertToHex(int n) {
    if (n < 0 || n > 15)
        return -1;

    return "0123456789ABCDEF"[n];
}
