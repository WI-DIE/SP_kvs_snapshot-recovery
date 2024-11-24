#include "kvs.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int do_recovery_custom(kvs_t *kvs, const char *file_path) {
    int fd = open(file_path, O_RDONLY);
    if (fd < 0) {
        printf("Failed to open %s for reading\n", file_path);
        return -1;
    }

    char buffer[4096];
    char line[256];
    size_t line_offset = 0;
    ssize_t bytes_read;

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        size_t buffer_offset = 0;
        while (buffer_offset < bytes_read) {
            if (line_offset >= sizeof(line) - 1) {
                printf("Line buffer overflow\n");
                return -1;
            }

            char c = buffer[buffer_offset++];
            if (c == '\n') {
                line[line_offset] = '\0';
                char key[100], value[100];
                sscanf(line, "%99[^,],%99[^\n]", key, value);
                put(kvs, key, value);
                line_offset = 0;
            } else {
                line[line_offset++] = c;
            }
        }
    }

    if (line_offset > 0) {
        line[line_offset] = '\0';
        char key[100], value[100];
        sscanf(line, "%99[^,],%99[^\n]", key, value);
        put(kvs, key, value);
    }

    close(fd);
    return 0;
}
