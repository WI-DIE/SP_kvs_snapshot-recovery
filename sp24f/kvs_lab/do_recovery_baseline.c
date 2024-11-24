#include "kvs.h"
#include <stdio.h>

int do_recovery_baseline(kvs_t *kvs, const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (!file) {
        printf("Failed to open %s for reading\n", file_path);
        return -1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char key[100], value[100];
        sscanf(line, "%99[^,],%99[^\n]", key, value);
        put(kvs, key, value);
    }

    fclose(file);
    return 0;
}
