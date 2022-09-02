/*
 *  revcasl -- symbol table definition and manipulation
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "casl.h"


int    LD_header_show(int fd);

int main(int argc, char **argv)
{
    int fd;

    if (argc != 2){
        fprintf(stderr, "ERROR: %s takes just 1 parameter (CASL file)\n", argv[0]);
        exit(1);
    }
    else
        printf("$ %s %s\n", argv[0], argv[1]);

    fd = open(argv[1], O_RDONLY);
    if (fd == -1){
        fprintf(stderr, "open error (errno: %d)\n", errno);
        exit(2);
    }

    LD_header_show(fd);

    printf("[LD header]\n");
    printf("[Machine code]\n");
    printf("[Data Constant]\n");
    printf("[Bss]\n");
    printf("[Stack]\n");
    
    close(fd);    
}

/*
 *  LD_header_show(fd) shows contents of .csx file (load module)
 */
int    LD_header_show(int fd)
{
    unsigned short bf[BLOCK_HALF];
    unsigned char cbf[BLOCK_SIZE];

    int n = read(fd, cbf, BLOCK_SIZE);
    if (n != BLOCK_SIZE) return -1;
    snapshot("LD_header", cbf, sizeof cbf);

    for (int i = 0; i < BLOCK_HALF; i++)
        bf[i] = (cbf[2*i] << 8) & cbf[2*i+1];

    printf("No of Sections: %d\n", bf[0]);
    printf("\t%-8s%04X  (%4d words)\n", "text:", bf[2], bf[3]);
    printf("\t%-8s%04X  (%4d words)\n", "data:", bf[4], bf[5]);
    printf("\t%-8s%04X  (%4d words)\n", "bss:", bf[6], bf[7]);
    printf("\t%-8s%04X  (%4d words)\n", "stack:", bf[8], bf[9]);

    return 0;
}
