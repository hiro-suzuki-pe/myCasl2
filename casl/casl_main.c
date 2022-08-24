/*
 *  casl -- symbol table definition and manipulation
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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

    instruction_print();
    label_table_print();
    DC_table_print();

}

/*
 *  initialize symbol and block table
 */
init()
{
    g_label_no = 0;
    g_DC_no = 0;
}

int label_lookup(char *str)
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
    return i;
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
        else if (str[1] == '\'')
            g_DC_table[i].val = str[2];
        else
            g_DC_table[i].val = atoi(&str[1]);
        g_DC_no++;
    }
    return;
}

void    instruction_print()
{
    struct instruction inst;
    struct operand  ope;
    struct address  adr;

    printf("\n<<instruction table (%d entry)>>\n", 
        g_instruction_no);
    printf("%-8s %-8s %-2s %-2s %-2s %-2s %-sx\n",
        "label", "inst", "code", "r", "x", "type", "address");
    for (int i = 0; i < g_instruction_no; i++){
        inst = g_instruction[i];
        ope = inst.ope;
        adr = ope.adr;
        printf("%-8s %-8s %04d ", inst.label, inst.name, inst.code); 
        if (ope.r == -1)
            printf("   ");
        else
            printf("%2d ", ope.r); 

        if (ope.x == -1)
            printf("   ");
        else
            printf("%2d ", ope.x); 

        if (adr.type == ADDRESS)
            printf("A  %04x", adr.value&0x0ffff);
        else if (adr.type == LABEL)
            printf("L  %04x", adr.value);
        else if (adr.type == LITERAL)
            printf("N  %04x", adr.value);
        printf("\n");
    }
}

void    label_table_print()
{
    struct label_table lbl;

    printf("<<label table (%d entry)>>\n", g_label_no);
    for (int i = 0; i < g_label_no; i++){
        lbl = g_label_table[i];
        printf("%-8s  %04x\n", lbl.label,lbl.adr);
    }
}

void    DC_table_print()
{
    struct DC_table dc;

    printf("<<DC table (%d entry)>>\n", g_DC_no);
    for (int i = 0; i < g_DC_no; i++){
        dc = g_DC_table[i];
        printf("%-8s  %04x\n", dc.label, dc.val);
    }
}

struct instruction *instruction_create(int label_idx,
                 int code, struct operand *ope)
{
    struct instruction *pinst = 
        (struct instruction *)malloc(sizeof (struct instruction));

    strcpy(pinst->name, inst_table[code].name);
    if (g_label_no < 0)
        strcpy(pinst->label, "");
    else
        strcpy(pinst->label, g_label_table[label_idx].label);
    pinst->code = code;
    if (ope == NULL){
        pinst->ope.r = -1;
        pinst->ope.x = -1;
        pinst->ope.adr = *address_create(UNKNOWN, 0);   
    }
    else
        pinst->ope = *ope;

    return pinst;
}

struct operand *operand_create(int r, int x, struct address *adr)
{
    struct operand *pope = (struct operand *)malloc(sizeof (struct operand));

    pope->r = r;
    pope->x = x;
    if (adr == NULL){
        pope->adr.type = UNKNOWN;
        pope->adr.value = 0;
    }
    else
        pope->adr = *adr;
    
    return pope;
}

struct address *address_create(enum adr_type type, int value)
{
    struct address *paddr = (struct address *)malloc(sizeof (struct address));

    paddr->type = type;
    paddr->value = value;

    return paddr;
}
