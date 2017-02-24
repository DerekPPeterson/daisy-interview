#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INPUT_FILENAME "input.csv"

typedef struct row
{
    int i, j, k;
    float x, y, z;
} row_t;

/* read_input_file
 * Arguments: 
 *      filename - The name of the file to read the data in from
 *      N - The number of rows to read from the file (should be equal to the
 *      number of lines in the file
 *      data - a pointer to memory allocated to store the data
 * Returns 0 on success, -1 otherwise
 */
int read_input_file(char *filename, int N, row_t *data)
{
    FILE * data_file;
    data_file = fopen(filename, "r");
    if (data_file == NULL) {
        return -1;
    }
  
    int i, j, k;
    float x, y, z;

    int count;
    for (count = 0; count < N; count++) {
        if (fscanf(data_file, "%d,%d,%d,%f,%f,%f", &i, &j, &k, &x, &y, &z) != 6) {
            return -1;
        }
        data[count].i = i;
        data[count].j = j;
        data[count].k = k;
        data[count].x = x;
        data[count].y = y;
        data[count].z = z;
   }
   return 0;
}

/* compute_result
 * Arguments:
 *      data - array of data over which to perform the computation
 *      N - Number of elements in data
 * Returns the result of the computation
 */
float compute_result(row_t *data, int N)
{
    float result = 0;
    int i;

    for (i = 0; i < N; i++) {
        result += sqrt(data[i].x) * pow(data[i].y, 2) + data[i].z;
    }
    return result;
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

    // Allocate memory
    row_t *data;
    data = malloc(sizeof(row_t) * N);

    // Read in the data
    if (read_input_file(INPUT_FILENAME, N, data)) {
        printf("Error reading input file\n");
        return -1;
    }

    float result = compute_result(data, N);
    printf("The result of the computation is: %f\n", result);

    free(data);

    return 0;
}
