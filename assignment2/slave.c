#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/file.h>


/* sum_lines_in_file
 * args: 
 *      filename: Name of a file (without a .dat extension) that contains a list
 *      of floating point numbers, one per line
 * Waits for filename to exist if it does not exist
 * Deletes file after reading
 * Returns the sum of all the numbers in filename
 */
float sum_lines_in_file(char *filename)
{
    FILE *file = NULL;
    float sum = 0;
    float cur_num;
    char new_filename[256] = "";

    strcat(new_filename, filename);
    strcat(new_filename, ".dat");

    while (file == NULL) {
        file = fopen(new_filename, "r");
        //printf("Could not open %s for reading: %s\n", new_filename, strerror(errno));
        usleep(1);
    }
    flock(fileno(file), LOCK_EX);

    while (EOF != fscanf(file, "%f", &cur_num)) {
        sum += cur_num;
    }

    flock(fileno(file), LOCK_UN);
    fclose(file);
    remove(new_filename);

    return sum;
}

/* create_results_file
 * args: 
 *      filename: Name of a file (without the ending _results.dat) that needs to
 *      be written to
 *      result: number to write to the file
 */
void create_results_file(char *filename, float result) 
{
    FILE *file;
   
    // create results file
    char new_filename[256] = "";
    char temp_filename[256] = "";

    strcat(new_filename, filename);
    strcat(new_filename, "_results.dat");
    strcat(temp_filename, "_temp_results.dat");

    file = fopen(temp_filename, "w");
    flock(fileno(file), LOCK_EX);
    if (file == NULL) {
        printf("Could not open %s for writing: %s", new_filename, strerror(errno));
        exit(-1);
    }

    fprintf(file, "%f", result);
    flock(fileno(file), LOCK_UN);
    fclose(file);
    rename(temp_filename, new_filename);
}

int main(int argc, char **argv)
{
    char *filename = argv[1];
    printf("Slave %s started\n", filename);
    fflush(stdout);

    while (1) {
        float sum = sum_lines_in_file(filename); 
        create_results_file(filename, sum);
    }
}
