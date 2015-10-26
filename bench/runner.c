#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "timer.h"
#include "generate.h"

static void print_help() {
    fprintf(stderr,"usage: bench [OPTIONS]");
    fprintf(stderr,"options:");
    fprintf(stderr, "-b 200\n");
    fprintf(stderr, "\tbench size in MBs\n");
    fprintf(stderr, "-c 6\n");
    fprintf(stderr, "\tcolumns to generate\n");
    fprintf(stderr, "-r 5\n");
    fprintf(stderr, "\tnumber of measure runs\n");
}

static void run(const char* restrict command, const char* restrict buffer, size_t buffer_size, unsigned int repeats, double* results) {
    for (unsigned int r = 0; r < repeats; r++) {
        FILE* target = popen(command, "w");
        if (!target) {
            fprintf(stderr, "Can't start \"%s\"\n", command);
            results[r] = -1;
        }
        double start = getRealTime();
        fwrite(buffer, sizeof(char), buffer_size, target);
        if (pclose(target) != 0) {
            fprintf(stderr, "\"%s\" had an error.\n", command);
            results[r] = -1;
        }
        double stop = getRealTime();
        results[r] = (stop - start);
    }
}

static void print_run(const char* restrict command, const char* restrict buffer, size_t buffer_size, unsigned int repeats) {
    double* results = calloc(repeats, sizeof(double));
    run(command, buffer, buffer_size, repeats, results);
    fprintf(stdout, "\"%s\"", command);
    for (unsigned int r = 0; r < repeats; r++) {
        fprintf(stdout, ",%fMB/s", ( buffer_size / results[r]) / (1024*1024) );
    }
    fprintf(stdout, "\n");
    free(results);
}

int main(int argc, char** argv) {
    size_t bench_size = 200 * 1024 * 1024; 
    unsigned int columns = 6;
    unsigned int repeats = 5;

    char c;
    while ((c = getopt (argc, argv, "b:c:r:h")) != -1) {
        switch (c) {
            case 'b':
                sscanf(optarg, "%zd", &bench_size);
                bench_size *= 1024 * 1024;
                break;
            case 'c':
                sscanf(optarg, "%u", &columns);
                break;
            case 'r':
                sscanf(optarg, "%u", &repeats);
                break;
            case '?':
            case 'h':
            default:
                print_help();
                exit(1);
                break;
        }
    }
    char* buffer = calloc(bench_size, sizeof(char));
    fprintf(stderr, "Preparing data (%zd bytes)\n",bench_size); 
    size_t data_filled = generate_csv(buffer, bench_size, 42, 11, columns);
    fprintf(stderr, "Data ready (%zd bytes)\n",data_filled); 
    print_run("wc -l > /dev/null", buffer, data_filled, repeats);
    print_run("wc -l > /dev/null", buffer, data_filled, repeats);
    
    return 0;
}
