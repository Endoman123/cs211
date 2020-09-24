#include <stdio.h>
#include <stdlib.h>

typedef struct _Fox {
    char name[20];
} Fox;

typedef struct _Chicken {
    int ids[3];
} Chicken;

typedef struct _Hay {
    int numHay;
} Hay;

typedef struct _Farmer {
    Fox* fox;
    Chicken* chicken;
    Hay* hay;
} Farmer;

int main (int argc, char** argv) {	 
    FILE* fp = fopen(argv[1], "r");
    char name[20];
    int numHay = 0;
    Farmer* farmer = (Farmer*)malloc(sizeof(Farmer));

    fscanf(fp, "%s\n", &name);

    int num = 0;
    while (fscanf(fp, "%d\n", &num))
        numHay += num;


    fscanf(fp, "%d\n", &name);

    farmer->fox->name = name;
    farmer->hay->numHay = numHay;

    return 0;
}
