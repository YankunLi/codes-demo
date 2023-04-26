
#include <stdio.h>
#include <string.h>
#include <sys/timeb.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    struct timeb startTime , endTime;
    ftime(&startTime);
    if (strcmp(argv[4], "create") == 0) {
        char *name = (char *) malloc(125);
        char *prefix =  "testfilename-";
        int idx = 0;
        FILE *fp;
        for (idx = 0; idx < 10000; idx++) {
            sprintf(name, "%s%d", prefix, idx);
            fp = fopen(name, "w");
            if(fp == NULL) {
                printf("fail to open the file! \n");
                exit(-1);
            }else{
                fclose(fp);
            }
        }
        ftime(&endTime);
        free(name);
        fprintf(stdout, "create 10000 file escaped: %ld ms\n", (endTime.time-startTime.time)*1000 + (endTime.millitm - startTime.millitm));
        exit(0);
    }

    ftime(&startTime);
    if (strcmp(argv[4], "del") == 0) {
        char *name = (char *) malloc(125);
        char *prefix =  "testfilename-";
        int idx = 0;
        FILE *fp;
        int rc ;
        for (idx = 0; idx < 10000; idx++) {
            sprintf(name, "%s%d", prefix, idx);
            rc = remove(name);
            if(rc != 0) {
                printf("fail to open the file! \n");
                exit(-1);
            }
        }
        ftime(&endTime);
        free(name);
        fprintf(stdout, "del 10000 file escaped: %ld ms\n", (endTime.time-startTime.time)*1000 + (endTime.millitm - startTime.millitm));
        exit(0);
    }

    return 0;
}
