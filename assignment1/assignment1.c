#include <stdio.h>
#include <stdlib.h>

/* read_input_file
 * Arguments:
 */
int ** read_input_file(char *filename, float ***data)
{
    FILE * data_file;
    data_file = fopen(filename, "r");
  
    int i, j, k;
    float x, y, z;

    while (! fscanf(data_file, "%d,%d,%d,%f,%f,%f", &i, &j, &k, &x, &y, &z)) {
        data[i][k][k] = 
    }
}

int main(int argc, char **argv)
{

    // Read in integer from command line
    int N;

    if (argc != 2) {
        printf("Please enter an integer\n");
        exit(-1);
    }
    N = atoi(argv[1]);

    // Allocate memory for data

    // Execute the computation

    // Free the memory
}
