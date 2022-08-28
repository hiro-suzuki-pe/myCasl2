#include <stdio.h>
#include <errno.h>
#include <string.h>

/* 
 * xcd --- hexa and char dump
 */
int xcd(char *file_name)
{
    if (file_name == NULL)
        printf("xcd()\n");
    else{
        /*
                FILE *fp = fopen(file_name, "r");
                if (fp == NULL){
                    fprintf(stderr, "%s open error (errno=)\n", argv[i], errno);
                    exit(-1);
                }
        */
        printf("xcd(%s)\n", file_name);
//        fclose(fp);
    }
}

int main(int argc, char **argv)
{
    int i;

    if (argc == 1) 
        xcd(NULL);
    else{
        for (i = 1; i < argc; i++){
            xcd(argv[i]);
        }
    }
    return 0;

err:if (argc == 1)
        fprintf(stderr, "xcd(%s) error\n", "");
    else
        fprintf(stderr, "xcd(%s) error\n", argv[i]);
    return -1;
}