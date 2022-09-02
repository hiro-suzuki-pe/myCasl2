#include <stdio.h>
#include <errno.h>
#include <string.h>

#define CHARS_IN_LINE   16
#define top(x)      ((x/CHARS_IN_LINE) * CHARS_IN_LINE)

int snapshot(char *memory_name, unsigned char *m, int n)
{
    if (memory_name == NULL || strlen(memory_name) == 0)
        printf("top address: %04X\n", m);
    else
        printf("\"%s\" address: %04X\n", memory_name, m);
    
    int pos = 0;
    int i;
//    int n;
    int skip = 0;
    char xbf[3*CHARS_IN_LINE+1];
    char xbf_prev[3*CHARS_IN_LINE+1];
    char cbf[CHARS_IN_LINE+1];

    strcpy(xbf_prev, "");
    while((n - pos) >= CHARS_IN_LINE){
        for (i = 0; i < CHARS_IN_LINE; i++){
            sprintf(&xbf[3*i], "%02X ", m[pos + i]);
            sprintf(&cbf[i], "%c", isprint(m[pos + i])? m[pos + i]: '.');
        }
        if (strcmp(xbf, xbf_prev) != 0){
            printf("%04X:  %-48s    %-16s\n", top(pos), xbf, cbf);
            strcpy(xbf_prev, xbf);
            skip = 0;
        }
        else if (skip == 0){
            printf("       *\n");
            skip = 1;
        }
        pos += CHARS_IN_LINE;
    }

    if (skip == 1) n = CHARS_IN_LINE;
    if (n > 0){
        for (i = 0; i < n; i++){
            sprintf(&xbf[3*i+1], "%02X ", m[pos + i]);
            sprintf(&cbf[i], "%c", isprint(m[pos + i])? m[pos + i]: '.');
        }
        for (; i < CHARS_IN_LINE; i++){
            sprintf(&xbf[3*i+1], "   ");
            sprintf(&cbf[i], " ");
        }
        printf("%04X:  %-48s    %-16s\n", top(pos), xbf, cbf);
    }
    return 0;
}
