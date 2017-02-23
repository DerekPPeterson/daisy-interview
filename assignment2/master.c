#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/file.h>

#define N_DATA_LINES 100
#define MAX_DATA_VAL 100

/* create_results_file
 * args:
 *      filename: filename to which N_DATA_LINES of random floating point 
 *      numbers between 0 and MAX_DATA_VAL will be written
 */
void create_data_file(char *filename) 
{
    FILE *file;
    
    file = fopen(filename, "w");
    flock(fileno(file), LOCK_EX);
    if (file == NULL) {
        printf("Could not open %s for writing: %s", filename, strerror(errno));
        exit(-1);
    }

    int i;
    for (i = 0; i < N_DATA_LINES; i++) {
        float random = (float) rand() / (float) (RAND_MAX/MAX_DATA_VAL);
        fprintf(file, "%f\n", random);
    }
    flock(fileno(file), LOCK_UN);
    fclose(file);
}

/* read_results_file
 * args:
 *      filename: Name of results file to read. Should contain a single floating
 *      point number
 * Waits for the file to exist if it does not already
 * Also removes the results file
 * Returns the floating point number in the file
 */
float read_results_file(char * filename)
{
    FILE * file = NULL;
    float result;

    while (file == NULL) {
        file = fopen(filename, "r");
    }
    flock(fileno(file), LOCK_EX);
    fscanf(file, "%f", &result);

    flock(fileno(file), LOCK_UN);
    fclose(file);
    return result;
}

void create_slave_process(char *arg) {
    char *argv[] = {"./slave", arg, NULL};
    if (fork() == 0) {
        execv("./slave", argv);
    }
}

int main(int argc, char **argv)
{
    int i = 0;

    create_slave_process("A");
    create_slave_process("B");

    remove("A.dat");
    remove("B.dat");
    remove("A_results.dat");
    remove("B_results.dat");

    while(1) {
        create_data_file("tempA.dat");
        create_data_file("tempB.dat");

        rename("tempA.dat", "A.dat");
        rename("tempB.dat", "B.dat");

        float A_result = read_results_file("A_results.dat");
        float B_result = read_results_file("B_results.dat");

        printf("%d\t%f\t%f\n", i, A_result, B_result);
        i++;
   }
}
