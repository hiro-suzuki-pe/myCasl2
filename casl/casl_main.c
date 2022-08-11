/*
 *  casl -- symbol table definition and manipulation
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "casl.h"
#include "casl.tab.h"

/* 
 *  main function
 */
int main(int argc, char **argv)
{
    extern int yylex();
    extern int yyparse();
    extern char *yytext;
    extern FILE *yyin;
    char *p;

    if (argc != 2){
        fprintf(stderr, "ERROR: %s takes just 1 parameter (CASL file)\n", argv[0]);
        exit(1);
    }
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL){
        fprintf(stderr, "ERROR: cannot open %s\n", argv[1]);
        exit(1);
    } else{
        printf("%s: input file %s\n", argv[0], argv[1]);
    }
    yyin = fp;
    yyparse();
    fclose(fp);

    print_instruction();
    print_label_table();
    print_DC_table();
}

/*
 *  initialize symbol and block table
 */
init()
{
    g_label_no = 0;
    g_DC_no = 0;
}

void label_lookup(char *str)
{
    int i;

    for (i = 0; i < g_label_no; i++){
        if (strcmp(str, g_label_table[i].label) == 0)
            break;
    }
    if (i == g_label_no){
        strcpy(g_label_table[i].label, str);
        g_label_table[i].adr = 0;
        g_label_no++;
    }
    return;
}

void DC_lookup(char *str)
{
    int i;

    for (i = 0; i < g_DC_no; i++){
        if (strcmp(str, g_DC_table[i].label) == 0)
            break;
    }
    if (i == g_DC_no){
        strcpy(g_DC_table[i].label, str);
        if (str[1] == '#')
            g_DC_table[i].val = strtol(&str[2], NULL, 16);
        else
            g_DC_table[i].val = atoi(&str[1]);
        g_DC_no++;
    }
    return;
}

void    print_instruction()
{
    struct instruction inst;

    printf("\n\n<<instruction table (%d entry)>>\n", 
        g_instruction_no);
    for (int i = 0; i < g_instruction_no; i++){
        inst = g_instruction[i];
        printf("%-8s %-8s %02d %2d %2d %04x\n",
            inst.label, inst.name, inst.code, 
            inst.r, inst.x, inst.adr);
    }
}

void    print_label_table()
{
    struct label_table lbl;

    printf("\n\n<<label table (%d entry)>>\n", g_label_no);
    for (int i = 0; i < g_label_no; i++){
        lbl = g_label_table[i];
        printf("%-8s  %04x\n", lbl.label,lbl.adr);
    }
}


void    print_DC_table()
{
    struct DC_table dc;

    printf("\n\n<<DC table (%d entry)>>\n", g_DC_no);
    for (int i = 0; i < g_DC_no; i++){
        dc = g_DC_table[i];
        printf("%-8s  %04x\n", dc.label, dc.val);
    }
}

