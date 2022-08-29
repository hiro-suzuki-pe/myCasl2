#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#define BLOCK_SIZE      1024
#define CHARS_IN_LINE   16
#define top(x)      ((x/CHARS_IN_LINE) * CHARS_IN_LINE)

void xcd_text(FILE *fp);    /* dump in hexa and char (text mode) */
void xcd(int fd);           /* dump in hexa and char (binary mode) */

int main(int argc, char **argv)
{
    int i;

    if (argc == 1)          /* in text mode */
        xcd_text(NULL);
    else{                   /* in binary mode */
        for (i = 1; i < argc; i++){
            printf("xcd(%s)\n", argv[i]);

            int fd = open(argv[i], O_RDONLY);
            if (fd == -1){
                fprintf(stderr, "%s open error (errno=)\n", argv[i], errno);
                exit(-1);
            }
            xcd(fd);
            close(fd);
        }
    }
    return 0;

err:if (argc == 1)
        fprintf(stderr, "xcd(%s) error (text mode)\n", "");
    else
        fprintf(stderr, "xcd(%s) error (binary mode)\n", argv[i]);
    return -1;
}

/* 
 * xcd_text --- hexa + char dump
 */
void xcd_text(FILE *fp)
{
    int pos = 0;
    int n = 0;
    char c;
    char xbf[3*CHARS_IN_LINE+1];
    char cbf[CHARS_IN_LINE+1];

    while((c = fgetc(fp)) != EOF){
        if (n == 0){
            memset(xbf, 0x20, sizeof(xbf));     /* バッファをスペースで初期化 */
            memset(cbf, 0x20, sizeof(cbf));
        }
        sprintf(&xbf[3*n+1], "%02X ", c);
        sprintf(&cbf[n], "%c", isprint(c)? c: '.');

        n++;
        if (n >= CHARS_IN_LINE){
//            printf("%04X  %-48s    %-16s\n", top(pos), xbf, cbf);
            printf("%04X[%2d]  %-48s    %-16s\n", top(pos), n, xbf, cbf);
            n = 0;
        }
        pos++;
    }
    if (n != 0)
//        printf("%04X  %-48s    %-16s\n", top(pos), xbf, cbf);
        printf("%04X[%2d]  %-48s    %-16s\n", top(pos), n, xbf, cbf);
    return;
}


/* 
 * xcd --- hexa + char dump (file descriptor version/binary mode)
 */
void xcd(int fd)
{
    int pos = 0;
    int i;
    int n;
    char rbf[CHARS_IN_LINE];
    char xbf[3*CHARS_IN_LINE+1];
    char cbf[CHARS_IN_LINE+1];

    n = read(fd, rbf, CHARS_IN_LINE);
    while(n == CHARS_IN_LINE){
        for (i = 0; i < CHARS_IN_LINE; i++){
            sprintf(&xbf[3*i+1], "%02X", rbf[i]);
            sprintf(&cbf[i], "%c", isprint(rbf[i])? rbf[i]: '.');
        }
        printf("%04X[%2d]  %-48s    %-16s\n", top(pos), n, xbf, cbf);

        pos += CHARS_IN_LINE;

        n = read(fd, rbf, CHARS_IN_LINE);
    }

    if (n > 0)
        for (i = 0; i < n; i++){
            sprintf(&xbf[3*i+1], "%02X ", rbf[i]);
            sprintf(&cbf[i], "%c", isprint(rbf[i])? rbf[i]: '.');
        }
        for (; i < CHARS_IN_LINE; i++){
            sprintf(&xbf[3*i+1], "   ");
            sprintf(&cbf[i], " ");
        }

        printf("%04X[%2d]  %-48s    %-16s\n", top(pos), n, xbf, cbf);
    return;
}
