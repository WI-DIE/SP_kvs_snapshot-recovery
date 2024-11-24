#include "kvs.h"
#include <stdio.h>
#include <time.h>
#include <sys/resource.h>

// do_snapshot, do_recovery 함수 선언
int do_snapshot_baseline(kvs_t *kvs, const char *file_path);
int do_recovery_baseline(kvs_t *kvs, const char *file_path);

double measure_time(int (*func)(kvs_t *, const char *), kvs_t *kvs, const char *file_path);

void print_memory_usage() {
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        printf("Memory usage: %ld KB\n", usage.ru_maxrss);
    } else {
        perror("getrusage failed");
    }
}

double measure_time(int (*func)(kvs_t *, const char *), kvs_t *kvs, const char *file_path) {
    clock_t start, end;
    start = clock();
    int result = func(kvs, file_path);
    end = clock();
    if (result != 0) {
        printf("Function failed\n");
    }
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

int main() {
    // 메모리 사용량 초기 출력
    print_memory_usage();

    // KVS 열기
    kvs_t *kvs = kvs_open();
    if (!kvs) {
        printf("Failed to open KVS\n");
        return -1;
    }

    // 복구 실행 및 시간 측정
    double recovery_time = 0.0;
    recovery_time = measure_time(do_recovery_baseline, kvs, "kvs.img");
    printf("Baseline do_recovery time: %.6f seconds\n", recovery_time);
    print_memory_usage(); // 복구 후 메모리 사용량 출력

    // Workload 파일 읽기 및 데이터 로드
    FILE *fp1 = fopen("cluster004.trc", "r");
    FILE *fp2 = fopen("answer.dat", "w");
    if (!fp1 || !fp2) {
        printf("Failed to open workload or answer files\n");
        fclose(fp1);
        fclose(fp2);
        kvs_close(kvs);
        return -1;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp1) != NULL) {
        line[strcspn(line, "\n")] = '\0'; // 줄바꿈 제거
        char *command = strtok(line, ",");
        char *key = strtok(NULL, ",");
        char *value = strtok(NULL, ",");

        if (command != NULL && key != NULL) {
            if (strcmp(command, "get") == 0) {
                char *result = get(kvs, key);
                if (result != NULL) {
                    fprintf(fp2, "%s\n", result);
                } else {
                    fprintf(fp2, "%d\n", -1);
                }
            } else if (strcmp(command, "set") == 0 && value != NULL) {
                put(kvs, key, value);
            }
        }
    }

    // Snapshot 생성 및 시간 측정
    double snapshot_time = 0.0;
    snapshot_time = measure_time(do_snapshot_baseline, kvs, "kvs.img");
    printf("Baseline do_snapshot time: %.6f seconds\n", snapshot_time);
    print_memory_usage(); // 스냅샷 후 메모리 사용량 출력

    // 파일 닫기 및 KVS 종료
    fclose(fp1);
    fclose(fp2);
    kvs_close(kvs);

    return 0;
}
