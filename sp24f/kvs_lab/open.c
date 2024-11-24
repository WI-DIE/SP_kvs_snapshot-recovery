#include "kvs.h"

kvs_t* kvs_open() {
  kvs_t *kvs = (kvs_t*)malloc(sizeof(kvs_t));
  if (!kvs) return NULL;

  kvs->level = 1;
  kvs->items = 0;

  kvs->header = (node_t*)malloc(sizeof(node_t));
  kvs->header->forward = (node_t**)malloc(sizeof(node_t*) * MAX_LEVEL);
  for (int i = 0; i < MAX_LEVEL; i++) {
    kvs->header->forward[i] = NULL;
  }
  return kvs;
}
