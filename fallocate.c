#define _GNU_SOURCE
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdint.h>
#include <sys/stat.h>


#define MB 1024*1024
static int64_t BIGDATA = 32;
#define PREFIX "vol-"
#define VOLCNT 203

#define CREATEVOL 1
#define WRITEVOL 2

static void
create_vol(int32_t counts) {
    int idx = 0;
    char filename[128];
    int fd;
    int re;

    for (;idx < VOLCNT; idx++) {
        sprintf(filename, "%s%d", PREFIX, idx);
        printf("to create file %s size: %ld", filename, BIGDATA);
        fd = open(filename, O_CREAT|O_RDWR, 0660);
        if (fd == -1) {
            printf("create file %s fail", filename);
            return;
        }
        re = posix_fallocate(fd, 0, BIGDATA);
        if (re != 0) {
            printf("fallocate fail for file %s", filename);
            return;
        }
        close(fd);
    }

    return;
}

static void
write_vol(int32_t counts, int32_t chunk_size, int64_t total_size) {
    int idx = 0;
    int fd;
    srand((unsigned int)time(0));
    int *idx_arr = (int *)calloc(counts, sizeof(int));
    int i = 0, j = 0;
    int tmp = 0;
    printf("select volumes: ");
    for (;i < counts; i++) {
try_again:
        tmp = rand()%VOLCNT;
        for(j = 0; j < i; j++) {
            if (tmp == idx_arr[j]) {
                goto try_again;
            }
        }
        idx_arr[i] = tmp;
        printf("vol-%d ", tmp);
    }
    printf("\n");

    int *fds = (int *) calloc(counts, sizeof(int));
    char *chunk = (char *) malloc(chunk_size);
    idx = 0;
    char filename[128];

    for (; idx < counts; idx++) {
        sprintf(filename, "%s%d", PREFIX, idx_arr[idx]);
        fds[idx] = open(filename, O_RDWR, 0660);
        if (fds[idx] == -1) {
            printf("open file %s fail", filename);
            return;
        }
    }

    idx = 0;
    int re = 0;
    while (total_size > 0) {
        fd = fds[idx%counts];
        re = write(fd, chunk, chunk_size);
        if (re == -1) {
            printf("vol-%d write error\n", idx_arr[idx%counts]);
        }
        fdatasync(fd);
        idx++;
        total_size -= chunk_size;
    };

    for (idx = 0; idx < counts; idx++) {
        close(fds[idx]);
    }

    free(fds);
    free(chunk);
}

int
main(int argc, char *argv[]) {
    BIGDATA = BIGDATA * 1024*1024*1024;
    if (argc < 2 ) {
        return -1;
    }

    int32_t op = atoi(argv[1]);
    int64_t value1 = atol(argv[2]);
    int64_t value2;
    int64_t value3;

    switch(op) {
        case CREATEVOL:
            create_vol(value1);
            break;
        case WRITEVOL:
            value2 = atol(argv[3]);
            value3 = atol(argv[4]);
            printf("write count: %d chunk: %d total size: %ld\n", value1, value2, value3);
            write_vol(value1, value2, value3);
            break;
        default:
            printf("unknow cmd %d", op);
            return -1;
    }

    return 0;
}
