#include "kvs.h"
#include <stdio.h>

int do_snapshot_baseline(kvs_t *kvs, const char *file_path) {
    FILE *file = fopen(file_path, "w");
    if (!file) {
        printf("Failed to open %s for writing\n", file_path);
        return -1;
    }

    node_t *current = kvs->header->forward[0];
    while (current != NULL) {
        fprintf(file, "%s,%s\n", current->key, current->value);
        current = current->forward[0];
    }

    fclose(file);
    return 0;
}
