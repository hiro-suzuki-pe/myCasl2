%{
    /*
     *  sample C -- lexical analysis
     */

#ifdef DEBUG       /* debugging version - if assert ok */

#include <assert.h>


main()
{
    char *p;

    assert(sizeof(int) >= sizeof(char *));
    while(p = (char *)yylex())
    printf("%-10.10s is \"%s\"\n", p, yytext);
}

s_lookup()  {}
int yynerrs = 0;

#define token(x)    x

#else   !DEBUG      /* production version */

#include "casl.tab.h"
#define token(x)    x
int yynerrs = 0;

#endif  DEBUG

#define END(v)  (v-1 + sizeof v/sizeof v[0])
static int screen();
%}

letter      [a-zA-Z_]
digit       [0-9]
letter_or_digit [a-zA-Z_0-9]
white_space [ \t\n]
blank       [ \t]
other       .

%%

^"#"{blank}*{digit}+({blank}+.*)?\n     yymark();
">="        return token(GE);
"<="        return token(LE);
"=="        return token(EQ);
"!="        return token(NE);
"+="        return token(PE);
"-="        return token(ME);
"*="        return token(TE);
"/="        return token(DE);
"%="        return token(RE);
"++"        return token(PP);
"--"        return token(MM);

{letter}{letter_or_digit}*      return screen();

{digit}+    { s_lookup(token(Constant));
              return token(Constant);
            }

{white_space}+  ;
{other}     return token(yytext[0]);

%%
/*
 *  reserved word screener
 */
struct instable {       /* reserved word table */
    char *inst_name;    /* representation */
    int inst_yylex;     /* yylex() value(= token value) */
    inst code;          /* machine code */
 };
 static struct instable  instable[] = {            /* sorted */
    { "ADDA", token(ADDA), 4},
    { "ADDL", token(ADDL), 5},
    { "AND", token(AND), 8},
    { "CALL", token(CALL), 25},
    { "CPA", token(CPA), 11},
    { "CPL", token(CPL), 12},
    { "DC", token(DC), -1},
    { "DS", token(DS), -1},
    { "END", token(END), -1},
    { "IN", token(IN), -1},
    { "JMI", token(JMI), 18},
    { "JNE", token(JNE), 20},
    { "JNZ", token(JNZ), 19},
    { "JOV", token(JOV), 21},
    { "JPL", token(JPL), 17},
    { "JUMP", token(JUMP), 22},
    { "LAD", token(LAD), 3},
    { "LD", token(LD), 1},
    { "NOP", token(NOP), 0},
    { "OR", token(OR), 9},
    { "OUT", token(OUT), -1},
    { "POP", token(POP), 24},
    { "PUSH", token(PUSH), 23},
    { "RET", token(RET), 26},
    { "RPOP", token(RPOP), -1},
    { "RPUSH", token(RPUSH), -1},
    { "RSVD1", token(RSVD1), 28},
    { "RSVD2", token(RSVD2), 29},
    { "RSVD3", token(RSVD3), 30},
    { "RSVD4", token(RSVD4), 31},
    { "SLA", token(SLA), 13},
    { "SLL", token(SLL), 15},
    { "SRA", token(SRA), 14},
    { "SRL", token(SRL), 16},
    { "ST", token(ST), 2},
    { "START", token(START), -1},
    { "SUBA", token(SUBA), 6},
    { "SUBL", token(SUBL), 7},
    { "SVC", token(SVC), 27},
    { "XOR", token(XOR), 10}
 };

static int screen()
{
    struct instable *low = instable,
    *high = END(instable),
    *mid;
    int c;

    while(low <= high)
    {
        mid = low + (high-low)/2;
        if ((c = strcmp(mid->rw_name, yytext)) == 0)
            return mid->rw_yylex;
        else if (c < 0)
            low = mid + 1;
        else
            high = mid - 1;
    }
    s_lookup(token(Identifier));
    return token(Identifier);
}

#ifdef DEBUG

s_lookup(yylex)
int yylex;          /* Constant or Identifier */
{
//    extern char yytext[];
    extern char *yytext;

    switch(yylex){
        case Constant:
            yylval.y_str = strsave(yytext);
            break;
        case Identifier:
            if (yylval.y_sym = s_find(yytext))
               break;  
            yylval.y_sym = s_create(yytext);
            break;
        default:
            bug("s_lookup");
    }
}
#endif