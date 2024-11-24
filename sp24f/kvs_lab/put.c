#include "kvs.h"

int put(kvs_t *kvs, const char *key, const char *value) {
  node_t *update[MAX_LEVEL];
  node_t *current = kvs->header;

  for (int i = kvs->level - 1; i >= 0; i--) {
    while (current->forward[i] != NULL && strcmp(current->forward[i]->key, key) < 0) {
      current = current->forward[i];
    }
    update[i] = current;
  }
  current = current->forward[0];

  if (current != NULL && strcmp(current->key, key) == 0) {
    free(current->value);
    current->value = strdup(value);
    return 0;
  }

  int new_level = (rand() % MAX_LEVEL) + 1;
  if (new_level > kvs->level) {
    for (int i = kvs->level; i < new_level; i++) {
      update[i] = kvs->header;
    }
    kvs->level = new_level;
  }

  node_t *new_node = (node_t*)malloc(sizeof(node_t));
  strcpy(new_node->key, key);
  new_node->value = strdup(value);
  new_node->forward = (node_t**)malloc(sizeof(node_t*) * new_level);

  for (int i = 0; i < new_level; i++) {
    new_node->forward[i] = update[i]->forward[i];
    update[i]->forward[i] = new_node;
  }
  kvs->items++;
  return 0;
}
