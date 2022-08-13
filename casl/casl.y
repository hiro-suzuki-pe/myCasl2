/*
 *  casl parser
 *  syntax analysis with error recovery
 *  symbol table
 *  memeory allocation
 *  code generation
 *  (s/r conflicts: one on ELSE, one on error)
 */

%{

#include "casl.h"

ushort  g_instruction_no = 0;
struct instruction g_instruction [MAX_INSTRUCTION];

ushort  g_label_no = 0;
struct label_table  g_label_table[MAX_LABEL];

ushort  g_DC_no = 0;
struct  DC_table    g_DC_table[MAX_DC];

%}

%union {
    struct instruction  *y_instruction; /* Instruction */
    struct operand      *y_operand;     /* Operand */
    unsigned short      y_code;         /* instruction code */
    unsigned short      y_gr;           /* # of general register */
    unsigned short      y_adr;          /* address */
    unsigned short      y_label;        /* idx of label */
    unsigned short      y_literal;      /* idx of literal table */
}

/*
 *  terminal symbols
 */
%token  <y_code>    Inst_code
%token  <y_gr>      Gr
%token  <y_adr>     Address
%token  <y_label>   Label
%token  <y_literal> Literal

/*
 *  typed non-terminal symbols
 */
%type   <y_instruction> instruction 
%type   <y_operand>     operand

/*
 *  precedence table
 */
 %%

 program
    : {/* init(); */}
        instructions {/* end_program(); */}

instructions
    : instruction
    | instructions instruction    {yyerrok; }
    | error
    | instructions error

instruction
    : Label Inst_code 
            {   $$ = instruction_create($1, $2, NULL); 
                printf("%-08s\t%8s(%02d)\n", 
                    g_label_table[$1].label,
                    inst_table[$2].name, $2);}
    | Label Inst_code operand   
            {   $$ = instruction_create($1, $2, $3); 
                printf("%-08s\t%s(%02d)\n", 
                    g_label_table[$1].label,
                    inst_table[$2].name, $2);}
    | Label error 
        { printf("Label error\n"); yyerrok; }
    | Inst_code operand  {
        $$ = instruction_create(NULL, $1, $2); 
                  printf("\t%s(%02d)\n", inst_table[$1].name, $1);}
    | Inst_code  {   $$ = instruction_create(NULL, $1, NULL); 
                  printf("\t%s(%02d)\n", inst_table[$1].name, $1);}
    | Inst_code error { printf(" Inst_code error\n"); }

operand
    : Gr ',' Gr 
        { $$ = operand_create($1, $3, NULL);
          printf("GR%d,GR%d\n", $1, $3); }
    | Gr ',' Address
        {   $$ = operand_create($1, 0xffff, address_create(ADDRESS, $3));
            printf("GR%d,#%04\n", $1, $3); }
    | Gr ',' Address ',' Gr
         {   $$ = operand_create($1, $5, address_create(ADDRESS, $3));
             printf("GR%d,#%04x,GR%d\n", $1, $3, $5); }
    | Address 
         {  $$ = operand_create(0xffff, 0xffff, address_create(ADDRESS, $1));
           printf("#%04\n", $1); }
    | Address ',' Gr
         {  $$ = operand_create(0xffff, $3, address_create(ADDRESS, $1));
             printf("#%04,GR%d\n", $1, $3); }
    | Gr ',' Literal
        {  $$ = operand_create($1, 0xffff, address_create(LITERAL, $3));
           printf("GR%d,%s\n", $1, g_DC_table[$3].label); }
    | Label 
        {  $$ = operand_create(0xffff, 0xffff, address_create(LABEL, $1));
           printf("%s\n", g_label_table[$1].label); }
