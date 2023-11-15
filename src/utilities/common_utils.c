#include "common_utils.h"
#include <stdio.h>
#include <stdlib.h>

void ValidateArgs(char *file, int params_expected, int params_received) {
    if (params_expected != params_received) {
        printf("Error: Invalid number of arguments.\n");
        if (params_expected == 2) {
            printf("Usage: %s port_no\n", file);
        } else if (params_expected == 3) {
            printf("Usage: %s hostname port_no\n", file);
        }
        exit(EXIT_FAILURE);
    }
}