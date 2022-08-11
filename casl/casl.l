%{
    /*
     *  CASL2 -- lexical analysis
     */
#include <assert.h>
#include <string.h>
#include "casl.h"
#include "casl.tab.h"
#define token(x)    x
#define yywrap()    1

int yynerrs = 0;
static int screen(void);


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

{letter}{letter_or_digit}*   { return screen(); }

{digit}+    { /* s_lookup(token(Constant)  ); */
              return token(Address);
            }
#{hdigit}+  { printf("Hex: %s\n", yytext); }
={digit}+   { DC_lookup(yytext); }
=#{hdigit}+ {  DC_lookup(yytext); }


{white_space}+  ;
;.*\n           ;
{other}     return token(yytext[0]);

%%
/*
 *  reserved word screener
 */
 
struct inst_table {
    char    name[MAX_LENGTH];   /* Instruction code */
    ushort  code;               /* machine instruction code */
};

 static struct inst_table  inst_table[] = {            /* sorted */
    { "DC", -1},
    { "DS", -1},
    { "END", -1},
    { "IN", -1},
    { "OUT", -1},
    { "RPOP", -1},
    { "RPUSH", -1},
    { "START", -1},
    { "NOP", 0},
    { "LD", 1},
    { "ST", 2},
    { "LAD", 3},
    { "ADDA", 4},
    { "ADDL", 5},
    { "SUBA", 6},
    { "SUBL", 7},
    { "AND", 8},
    { "OR", 9},
    { "XOR", 10},
    { "CPA", 11},
    { "CPL", 12},
    { "SLA", 13},
    { "SRA", 14},
    { "SLL", 15},
    { "SRL", 16},
    { "JPL", 17},
    { "JMI", 18},
    { "JNZ", 19},
    { "JNE", 20},
    { "JOV", 21},
    { "JUMP", 22},
    { "PUSH", 23},
    { "POP", 24},
    { "CALL", 25},
    { "RET", 26},
    { "SVC", 27},
    { "RSVD1", 28},
    { "RSVD2", 29},
    { "RSVD3", 30},
    { "RSVD4", 31}
 };

static int screen()
{
    struct instruction y_inst;
    char *s_reg[] = {
        "GR0", "GR1", "GR2", "GR3", "GR4", "GR5", "GR6", "GR7"};

    int n = sizeof (inst_table)/ sizeof (inst_table[0]);
    for (int i = 0; i < n; i++){
        if (strcmp(yytext, inst_table[i].name) == 0){
            strcpy(y_inst.name, inst_table[i].name);
            strcpy(y_inst.label, "");
            y_inst.code = inst_table[i].code;
            y_inst.r = 0xffff;
            y_inst.x = 0xffff;
            y_inst.adr = 0xffff;
            g_instruction[g_instruction_no++] = y_inst;
            return token(Inst_code);
        }
    }
    for (int i = 0; i < 8; i++){
        if (strcmp(yytext, s_reg[i]) == 0)
            return token(Gr);
    }
    label_lookup(yytext);
    return token(Label);
}

