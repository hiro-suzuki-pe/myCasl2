%{
    /*
     *  CASL2 -- lexical analysis
     */
#include <assert.h>
#include    <string.h>
#include "casl.tab.h"
#define token(x)    x
#define yywrap()    1

int yynerrs = 0;

#define END(v)  (v-1 + sizeof v/sizeof v[0])
static int screen();


%}

letter      [A-Z_]
digit       [0-9]
hdigit      [A-F0-9]
letter_or_digit [A-Z_0-9]
white_space [ \t\n]
blank       [ \t]
other       .

%%

^"#"{blank}*{digit}+({blank}+.*)?\n     yymark();

{letter}{letter_or_digit}*   { printf("letter: %s\n", yytext); return screen(); }

{digit}+    { /* s_lookup(token(Constant)  ); */
              return token(Address);
            }
={digit}+ { printf("literal(D): %s\n", yytext); }
#{hdigit}+  { printf("Hex: %s\n", yytext); }
=#{hdigit}+ { printf("literal(H): %s\n", yytext); }


{white_space}+  ;
;.*\n           ;
{other}     return token(yytext[0]);

%%
/*
 *  reserved word screener
 */

 static struct inst_table  inst_table[] = {            /* sorted */
    { "ADDA", 4},
    { "ADDL", 5},
    { "AND", 8},
    { "CALL", 25},
    { "CPA", 11},
    { "CPL", 12},
    { "DC", -1},
    { "DS", -1},
    { "END", -1},
    { "IN", -1},
    { "JMI", 18},
    { "JNE", 20},
    { "JNZ", 19},
    { "JOV", 21},
    { "JPL", 17},
    { "JUMP", 22},
    { "LAD", 3},
    { "LD", 1},
    { "NOP", 0},
    { "OR", 9},
    { "OUT", -1},
    { "POP", 24},
    { "PUSH", 23},
    { "RET", 26},
    { "RPOP", -1},
    { "RPUSH", -1},
    { "RSVD1", 28},
    { "RSVD2", 29},
    { "RSVD3",  30},
    { "RSVD4", 31},
    { "SLA", 13},
    { "SLL", 15},
    { "SRA", 14},
    { "SRL", 16},
    { "ST", 2},
    { "START", -1},
    { "SUBA", 6},
    { "SUBL", 7},
    { "SVC", 27},
    { "XOR", 10}
 };

static int screen()
{
    char *s_reg[] = {
        "GR0", "GR1", "GR2", "GR3", "GR4", "GR5", "GR6", "GR7"};

    int n = sizeof (inst_table)/ sizeof (inst_table[0]);
    for (int i = 0; i < n; i++){
        if (strcmp(yytext, inst_table[i].inst_name))
            return token(Inst_code);
    }
    for (int i = 0; i < 8; i++){
        if (strcmp(yytext, s_reg[i]))
            return token(Gr);
    }
    s_lookup(yytext);
    return token(Label);
}

