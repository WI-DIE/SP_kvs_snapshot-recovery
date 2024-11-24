#include "kvs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int set(kvs_t* kvs, const char* key, const char* value)
{
    printf("set: %s, %s\n", key, value);

    // 새로운 노드 생성 및 초기화
    node_t* new_node = (node_t*)malloc(sizeof(node_t));
    if (!new_node) {
        fprintf(stderr, "Failed to allocate memory for node\n");
        return -1;
    }

    strcpy(new_node->key, key);
    new_node->value = strdup(value); // 값을 동적으로 할당하고 복제
    new_node->next = NULL;

    // 데이터베이스가 비어 있는 경우
    if (kvs->db == NULL) {
        kvs->db = new_node;
    } else {
        // 데이터베이스가 비어 있지 않은 경우
        // 주어진 key를 찾아서 해당하는 노드의 value를 변경
        node_t* current = kvs->db;
        while (current->next != NULL) {
            if (strcmp(current->key, key) == 0) {
                free(current->value); // 기존 값 메모리 해제
                current->value = strdup(value); // 새로운 값으로 복제
                free(new_node); // 새로 생성한 노드 메모리 해제
                return 0;
            }
            current = current->next;
        }

        // 주어진 key를 찾지 못한 경우, 새로운 노드를 데이터베이스에 추가
        current->next = new_node;
    }

    return 0;
}
