#include "kvs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  kvs_t* kvs = open();
  if (!kvs) {
    printf("Failed to open kvs\n");
    return -1;
  }

  FILE *file = fopen("query.dat", "r");
  if (!file) {
    printf("Failed to open query.dat\n");
    close(kvs);
    return -1;
  }

  FILE *answer_file = fopen("answer.dat", "w");
  if (!answer_file) {
    printf("Failed to open answer.dat\n");
    fclose(file);
    close(kvs);
    return -1;
  }

  char line[256];
  while (fgets(line, sizeof(line), file)) {
    char query[10], key[100], value[100] = "";

    int num_tokens = sscanf(line, "%9[^,],%99[^,],%99[^\n]", query, key, value);

    if (num_tokens >= 2 && strcmp(query, "set") == 0) {
      put(kvs, key, value);
    } else if (num_tokens >= 2 && strcmp(query, "get") == 0) {
      char* result = get(kvs, key);
      fprintf(answer_file, "%s\n", result ? result : "-1");
    }
  }

  fclose(file);
  fclose(answer_file);
  close(kvs);

  return 0;
}
