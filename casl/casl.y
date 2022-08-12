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

struct operand {
    ushort   r;              /* r or r1 if adr==0xffff, no r if r==0xffff */
    ushort   x;              /* x or r2 if adr==0xffff, no x if x==0xffff */
    ushort   adr;            /* address, r -> r1 & x -> r2 if adr=0 */
};

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
            {  printf("Label Inst_code : %s\t%d\n", 
                    g_label_table[$1].label,
                    $2);}
    | Label Inst_code operand   
        {  printf("Label Inst_code operand\n");
        printf("$1:%d, $2:%d", $$, $1, $2); }
    | Label error 
        { printf("Label error\n"); yyerrok; }
    | Inst_code operand  {  printf("Inst_code operand\n");  }
    | Inst_code  {  printf("Inst_code\n");  }
    | Inst_code error { printf(" Inst_code error\n"); }

operand
    : Gr ',' Gr {printf("Gr , Gr\n"); }
        { ;}
    | Gr ',' Address
        { }
    | Gr ',' Address ',' Gr
        {  }
    | Address 
        {  }
    | Address ',' Gr
        {  }
