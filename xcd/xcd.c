#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#define BLOCK_SIZE      1024
#define CHARS_IN_LINE   16
#define top(x)      ((x/CHARS_IN_LINE) * CHARS_IN_LINE)
void xcd(FILE *fp);

int main(int argc, char **argv)
{
    int i;

    if (argc == 1) 
        xcd(stdin);
    else{
        for (i = 1; i < argc; i++){
            printf("xcd(%s)\n", argv[i]);

            FILE *fp = fopen(argv[i], "r");
            if (fp == NULL){
                fprintf(stderr, "%s open error (errno=)\n", argv[i], errno);
                exit(-1);
            }
            xcd(fp);
            fclose(fp);
        }
    }
    return 0;

err:if (argc == 1)
        fprintf(stderr, "xcd(%s) error\n", "");
    else
        fprintf(stderr, "xcd(%s) error\n", argv[i]);
    return -1;
}

/* 
 * xcd --- hexa + char dump
 *      01234567890123456789012345678901234567890123456789012345678901234567890123456789
 *      xxxx  00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F    abcdefghijklmnop
 */
void xcd(FILE *fp)
{
    int pos = 0;
    int n = 0;
    int skip = 0;
    char c;
    char abf[8];    
    char xbf[3*CHARS_IN_LINE+1];
    char xbf_prev[3*CHARS_IN_LINE+1];
    char cbf[CHARS_IN_LINE+1];

    memset(xbf_prev, 0, sizeof(xbf_prev));

    while((c = fgetc(fp)) != EOF){
        if (n == 0){
            memset(xbf, 0x20, sizeof(xbf));     /* バッファをスペースで初期化 */
            memset(cbf, 0x20, sizeof(cbf));
        }
        sprintf(&xbf[3*n], "%02x ", c);
        sprintf(&cbf[n], "%c", isprint(c)? c: '.');

        n++;
        if (n >= CHARS_IN_LINE){
            if (strcmp(xbf, xbf_prev) != 0){
                printf("%04X:  %-48s    %-16s\n", top(pos), xbf, cbf);
                memcpy(xbf_prev, xbf, sizeof(xbf));
                skip = 0;
            }
            else if (strcmp(xbf, xbf_prev) == 0 && skip == 0){
                printf("\n%-7s* same value *\n\n", "");
                skip = 1;
            }
            n = 0;
        }
        pos++;
    }
    if (n != 0 || skip == 1)
        printf("%04X:  %-48s    %-16s\n", top(pos), xbf, cbf);
    return;
}

