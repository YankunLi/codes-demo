#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
    int64_t malloc_total_size = atoi(argv[1]) * 1024*1024*1024;
    int32_t chunk_size = 4*1024*1024;
    int32_t chunk_counts = (malloc_total_size + chunk_size-1)/chunk_size;
    int **list = (int**)malloc(sizeof(int) * chunk_counts);

    int i;
    int **it;
    int *temp;
    it = list;
    printf("to malloc memory: %lld\n", malloc_total_size);
    for(i = 0; i < chunk_counts; i++) {
        temp = (int *)malloc(chunk_size);
        if (temp == NULL) {
            printf("malloc fail\n");
            usleep(1000);
            i--;
            continue;
        }
        printf("malloc addr: %p\n", temp);
        *it = (int *)temp;
        it++;
    }
    usleep(1000*1000*1);
  //  it = list;
  //  printf("to free memory: %lld\n", malloc_total_size);
  //  for (i = 0; i < chunk_counts; i++) {
  //      printf("to free %p\n", *it);
  //      free((void*)*it);
  //      it++;
  //  }

    return 0;
}
