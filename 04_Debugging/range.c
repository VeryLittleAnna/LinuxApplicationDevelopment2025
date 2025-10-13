#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("Usage: range [M] [N] [S]\n\
No arguments - show help\n\
One argument N - print [0, 1, ... N-1]\n\
Two arguments M, N - print [M, M+1, ... N-1]\n\
Three arguments M, N, S - print [M, M+S, M+2S, ... N-1]\n");
        return 0;
    }
    int start = 0, end = 0, step = 1;
    if (argc == 2) {
        end = atoi(argv[1]);
    } else if (argc == 3) {
        start = atoi(argv[1]);
        end = atoi(argv[2]);
    } else {
        start = atoi(argv[1]);
        end = atoi(argv[2]);
        step = atoi(argv[3]);
    }
    if (step == 0) {
        return 1;
    }
    if (step > 0) {
        for (int i = start; i < end; i += step) {
            printf("%d\n", i);
        }
    } else {
        for (int i = start; i > end; i += step) {
            printf("%d\n", i);
        }
    }
    return 0;
}