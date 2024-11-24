#ifndef KVS_H
#define KVS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEVEL 10 // Skip List의 최대 레벨

typedef struct node {
  char key[100];
  char *value;
  struct node **forward;
} node_t;

typedef struct kvs {
  node_t *header;
  int level;
  int items;
} kvs_t;

int do_recovery_custom(kvs_t *kvs, const char *file_path);
int do_snapshot_custom(kvs_t *kvs, const char *file_path);
int do_recovery_baseline(kvs_t *kvs, const char *file_path);
int do_snapshot_baseline(kvs_t *kvs, const char *file_path);
kvs_t* kvs_open();
int kvs_close(kvs_t* kvs);
int put(kvs_t* kvs, const char* key, const char* value);
char* get(kvs_t* kvs, const char* key);

#endif // KVS_H
