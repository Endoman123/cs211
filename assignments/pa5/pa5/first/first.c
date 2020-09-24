#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef enum _op {
    AND,
    NOT,
    OR,
    NOR,
    XOR,
    XNOR,
    NAND,
    MUX,
    DECODE
} Operation;

typedef struct _var {
    char *name;
    int value;
    struct _var *next;
} Variable;

typedef struct _gate {
    Operation op;
    int nInputs, nAux, nOutputs; 
    char **input, aux;
    char **output;
    struct _gate *next;
} Gate;

Variable *inVar, *outVar, *tempVar;
Gate *circuit;
unsigned int *gcode, *output;
int nInputs, nOutputs, nRows;

void process();
void populateInputs(unsigned int);

int get(char *);
void *set(char *, int val);

int getNumBits(unsigned int);

void zeroList(Variable *);

void freeVarLL(Variable *);
void freeGateLL(Gate *);

int main(int argc, char **argv) {
    FILE *fp = fopen(argv[1], "r");

    if (fp != NULL) {
        int i;
        char *name = malloc(99 * sizeof(char));

        // Init Variable LL
        fscanf(fp, "INPUTVAR %d", &nInputs);

        Variable *ptr;
        for (i = 0; i < nInputs; ++i) {
            if (inVar == NULL) {
                inVar = malloc(sizeof(Variable));
                ptr = inVar;
            } else {
                ptr->next = malloc(sizeof(Variable));
                ptr = ptr->next;
            }

            if (fscanf(fp, "%s", name) > 0)
                ptr->name = name;
        }
        
        fscanf(fp, "\n");
        fscanf(fp, "OUTPUTVAR %d", &nOutputs);
      
        for (i = 0; i < nOutputs; ++i) {
            if (outVar == NULL) {
                outVar = malloc(sizeof(Variable));
                ptr = outVar;
            } else {
                ptr->next = malloc(sizeof(Variable));
                ptr = ptr->next;
            }

            if (fscanf(fp, "%s", name) > 0)
                ptr->name = name;            
        }
    
        fscanf(fp, "\n");

        // Init circuit
        Gate *gptr;
        while (fscanf(fp, "%s", name) > 0) {
            if (circuit == NULL) {
                circuit = malloc(sizeof(Gate));
                gptr = circuit;
            } else {
                gptr->next = malloc(sizeof(Gate));
                gptr = gptr->next;
            }

            if (!strcmp(name, "MULTIPLEXER")) {
                int nSwitches = 0;
                
                fscanf(fp, "%d", &nSwitches);

                gptr->op = MUX;
                gptr->nInputs = 1 << nSwitches; 
                gptr->nAux = nSwitches;
                gptr->nOutputs = 1;
            } else if (!strcmp(name, "DECODER")) {
                int nInputs = 0;
                
                fscanf(fp, "%d", &nInputs);
                
                gptr->op = DECODE;
                gptr->nInputs = nInputs;
                gptr->nAux = 0;
                gptr->nOutputs = 1 << nInputs;
            } else if (!strcmp(name, "NOT")) {
                gptr->op = NOT;
                gptr->nInputs = 1;
                gptr->nAux = 0;
                gptr->nOutputs = 1;
            } else {
                gptr->nInputs = 2;
                gptr->nAux = 0;
                gptr->nOutputs = 1;

                if (!strcmp(name, "AND"))
                    gptr->op = AND;
                else if (!strcmp(name, "OR"))
                    gptr->op = OR;
                else if (!strcmp(name, "NOR"))
                    gptr->op = NOR;
                else if (!strcmp(name, "XOR"))
                    gptr->op = XOR;
                else if (!strcmp(name, "XNOR"))
                    gptr->op = XNOR;
                else if (!strcmp(name, "NAND"))
                    gptr->op = NAND;
            }
            fscanf(fp, "\n");
        }

        // Start processing
        process();
    }
}

void process() {
    int i, nCodes = 1 << ((nInputs + 1) - 1);
    
    for (i = 0; i < nCodes; ++i) {
        populateInputs(i); 
        
        Variable *ptr = inVar;
        
        for (ptr = inVar; ptr != NULL; ptr = ptr->next)
            printf("%d ", ptr->value);

        for (ptr = outVar; ptr != NULL; ptr = ptr->next) {
            printf("%d ", ptr->value);
            printf(ptr->next == NULL ? "\n" : " ");
        }
    } 
}

/**
 * Populates the input variables with proper gray code
 */
void populateInputs(unsigned int n) {
    int i, gcode;
    Variable *ptr = inVar;

    // Step 1: Get gray code for number
    gcode = n ^ (n >> 1);

    // Step 2: Assign each to its proper input
    for (i = nInputs - 1; i >= 0; --i) {
        ptr->value = (gcode >> i) & 1;
        ptr = ptr->next; 
    }
}

/**
 * Gets the number of bits
 */
int getNumBits(unsigned int n) {
    int i = 1;

    for (i = 1; n > 1; ++i)
        n = n >> 1;

    return i;
}
