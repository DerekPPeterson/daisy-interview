#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>

#define N_DATA_LINES 100
#define MAX_DATA_VAL 100

/* create_results_file
 * args:
 *      filename: filename to which N_DATA_LINES of random floating point 
 *      numbers between 0 and MAX_DATA_VAL will be written
 *      sem: semaphore to wait on before opening file for write
 */
void create_data_file(char *filename, sem_t *sem) 
{
    FILE *file;
    
    sem_wait(sem);
    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Could not open %s for writing: %s", filename, strerror(errno));
        exit(-1);
    }

    int i;
    for (i = 0; i < N_DATA_LINES; i++) {
        float random = (float) rand() / (float) (RAND_MAX/MAX_DATA_VAL);
        fprintf(file, "%f\n", random);
    }
    fclose(file);
    sem_post(sem);
}

/* read_results_file
 * args:
 *      filename: Name of results file to read. Should contain a single floating
 *      point number
 *      sem: semaphore to wait on after we discover file exists, but before 
 *      reading to make sure that the slave is finished writing
 * Waits for the file to exist if it does not already
 * Also removes the results file
 * Returns the floating point number in the file
 */
float read_results_file(char * filename, sem_t *sem)
{
    FILE * file = NULL;
    float result;

    while (file == NULL) {
        file = fopen(filename, "r");
    }
    sem_wait(sem);
    fscanf(file, "%f", &result);

    fclose(file);
    remove(filename);
    sem_post(sem);

    return result;
}

/* create_slave_process
 * args:
 *      First argument that will be passed to the slave process
 * Fork/execs to create a slave process
 */
void create_slave_process(char *arg) {

    char *argv[] = {"./slave", arg, NULL};
    if (fork() == 0) {
        execv("./slave", argv);
    }
}


int main(int argc, char **argv)
{
    // Loop counter
    int i = 0;

    // Remove data files before starting
    remove("A.dat");
    remove("B.dat");
    remove("A_results.dat");
    remove("B_results.dat");

    // Reinitialize semaphores used to communicate with slave processes
    sem_unlink("/slaveA");
    sem_unlink("/slaveB");
    sem_t *semA = sem_open("/slaveA", O_CREAT, 0644, 1);
    sem_t *semB = sem_open("/slaveB", O_CREAT, 0644, 1);
    if (semA == SEM_FAILED || semB == SEM_FAILED) {
        printf("Failed to open semaphore\n");
        exit(-1);
    }

    create_slave_process("A");
    create_slave_process("B");

    // Main data procesing loop
    while(1) {
        create_data_file("A.dat", semA);
        create_data_file("B.dat", semB);

        float A_result = read_results_file("A_results.dat", semA);
        float B_result = read_results_file("B_results.dat", semB);

        printf("%d\t%f\t%f\n", i, A_result, B_result);
        i++;
   }
}
