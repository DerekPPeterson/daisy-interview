#include <stdlib.h>
#include <stdio.h>


float sum_lines_in_file(const char *filename)
{
    FILE *file;
    float sum = 0;
    float cur_num;

    file = fopen(filename, "r");

    while (EOF != fscanf(file, "%f", &cur_num)) {
        sum += cur_num;
    }
    return sum;
}

int main(int argc, char **argv)
{
    const char *filename = argv[1];
    printf(filename);
    float sum = sum_lines_in_file(filename); 
    printf("sum: %f\n", sum);
}
