#include "kvs.h"

char* get(kvs_t *kvs, const char *key) {
  node_t *current = kvs->header;
  for (int i = kvs->level - 1; i >= 0; i--) {
    while (current->forward[i] != NULL && strcmp(current->forward[i]->key, key) < 0) {
      current = current->forward[i];
    }
  }
  current = current->forward[0];

  if (current != NULL && strcmp(current->key, key) == 0) {
    return current->value;
  }
  return NULL;
}
