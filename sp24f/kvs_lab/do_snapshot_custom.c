#include "kvs.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int do_snapshot_custom(kvs_t *kvs, const char *file_path) {
    int fd = open(file_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        printf("Failed to open %s for writing\n", file_path);
        return -1;
    }

    node_t *current = kvs->header->forward[0];
    char buffer[4096]; // 더 큰 버퍼 사용
    size_t offset = 0; // 버퍼에 저장된 데이터 길이

    while (current != NULL) {
        int len = snprintf(buffer + offset, sizeof(buffer) - offset, "%s,%s\n", current->key, current->value);
        if (len < 0 || len >= sizeof(buffer) - offset) {
            write(fd, buffer, offset); // 버퍼가 꽉 차면 기록
            offset = 0;
            len = snprintf(buffer + offset, sizeof(buffer) - offset, "%s,%s\n", current->key, current->value);
        }
        offset += len;
        current = current->forward[0];
    }

    if (offset > 0) {
        write(fd, buffer, offset); // 남은 데이터 기록
    }

    fsync(fd);
    close(fd);
    return 0;
}
