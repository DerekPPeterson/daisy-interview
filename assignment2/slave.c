#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/file.h>
#include <semaphore.h>


/* sum_lines_in_file
 * args: 
 *      filename: Name of a file (without a .dat extension) that contains a list
 *      of floating point numbers, one per line
 * Waits for filename to exist if it does not exist
 * Deletes file after reading
 * Returns the sum of all the numbers in filename
 */
float sum_lines_in_file(char *filename, sem_t *sem)
{
    FILE *file = NULL;
    float sum = 0;
    float cur_num;
    char new_filename[256] = "";

    strcat(new_filename, filename);
    strcat(new_filename, ".dat");

    while (file == NULL) {
        file = fopen(new_filename, "r");
    }

    sem_wait(sem);
    while (EOF != fscanf(file, "%f", &cur_num)) {
        sum += cur_num;
    }

    fclose(file);
    remove(new_filename);
    sem_post(sem);


    return sum;
}

/* create_results_file
 * args: 
 *      filename: Name of a file (without the ending _results.dat) that needs to
 *      be written to
 *      result: number to write to the file
 */
void create_results_file(char *filename, float result, sem_t *sem) 
{
    FILE *file;
   
    // create results file
    char new_filename[256] = "";

    strcat(new_filename, filename);
    strcat(new_filename, "_results.dat");

    sem_wait(sem);
    file = fopen(new_filename, "w");
    if (file == NULL) {
        printf("Could not open %s for writing: %s", new_filename, strerror(errno));
        exit(-1);
    }

    fprintf(file, "%f\n", result);
    fclose(file);
    sem_post(sem);
}

int main(int argc, char **argv)
{
    char *filename = argv[1];
    printf("Slave %s started\n", filename);


    char sem_name[255] = "/slave";
    strcat(sem_name, filename);
    
    sem_t *sem = sem_open(sem_name, 0);

    while (1) {
        float sum = sum_lines_in_file(filename, sem); 
        create_results_file(filename, sum, sem);
    }
}
