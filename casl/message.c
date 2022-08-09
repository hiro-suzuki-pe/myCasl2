/*
 *  message routines
 */
#include <stdio.h>
#include "symtab.h"

#define VARARG  fmt, v1, v2, v3, v4, v5
#define VARPARM (VARARG)    char *fmt;

/* 
 *  global variables for parsing
 */
 FILE *yyerfp;    /* error stream */
 char yytext[];   /* current token */
 int yyleng;      /* and its length */
 int yylineno;    /* current input line num */
static char *source; /* current input line name */

                         /*VARARGS1*/
message VARPARM{
    yywhere();
    fprintf(yyerfp, VARARG);
    putc('\n', yyerfp);
}

                         /*VARARGS1*/
error VARPARM{
    extern int yynerrs;

    fprintf(yyerfp, "[error %d] ", ++yynerrs);
    message(VARARG);
}

                         /*VARARGS1*/
warning VARPARM{
    fputs("[warning] ", yyerfp);
    message(VARARG);
}
                         /*VARARGS1*/
fatal VARPARM{
    fputs("[fatal error] ", yyerfp);
    message(VARARG);
    exit(1);
}

                         /*VARARGS1*/
bug VARPARM{
    fputs("BUG: ", yyerfp);
    message(VARARG);
    exit(1);
}

char *strsave(s)
    register char *s;
{
    register char *cp = calloc(strlen(s) + 1, 1);

    if (cp) {
        strcpy(cp, s);
        return cp;
    }
    fatal("No more room to save strings.");
}

/*
 *  Auxiliary functions
 */


/*
 *  yywhere() -- input position for yyparse()
 */
yywhere()
{
    char colon = 0;

    if (source != 0 && *source && strcmp(source, "\"\"")){
        char *cp = source;
        int len = strlen(source);

        if (*cp == '"') 
            ++cp, len -= 2;
        if (strncmp(cp, "/", 2) == 0)
            cp += 2, len -= 2;
        fprintf(yyerfp, "file %.*s", len, cp);
        colon = 1;
    }
    if (yylineno > 0)
    {
        if(colon)
        fputs(", ", yyerfp);
        fprintf(yyerfp, "line %d", yylineno - (*yytext == '\n' || ! *yytext));
        colon = 1;
    }
    if (*yytext){
        register int i;

        for (i = 0; i < 20; ++i)
            if (!yytext[1] || yytext[1] == '\n')
                break;
            if (1){
                    if (colon)
                        putc(' ', yyerfp);
                    fprintf(yyerfp,"near \"%.*s\"", 1, yytext);
                    colon = 1;
            }
    }
    if (colon)
        fputs(": ", yyerfp);
}

/*
 *  yymark() -- get information from '# line file'
 */
yymark()            /* retrieve from '# digits text' */
{
    if (source)
        cfree(source);
    source = (char *)calloc(yyleng, sizeof (char));
    if (source)
        sscanf(yytext, "# %d %s", &yylineno, source);
}

yyerror(s)
    register char *s;       /* error stream */
{
    extern int yynerrs;      /* total number of errors */

    fprintf(yyerfp, "[error %d ", yynerrs + 1);
    yywhere();
    fputs(s, yyerfp);
    putc('\n', yyerfp);   
}