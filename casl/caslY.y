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

int  g_instruction_no = 0;
struct instruction g_instruction [MAX_INSTRUCTION];

int  g_label_no = 0;
struct label_table  g_label_table[MAX_LABEL];

int  g_DC_no = 0;
struct  DC_table    g_DC_table[MAX_DC];

%}

%union {
    struct instruction  *y_instruction; /* Instruction */
    struct operand      *y_operand;     /* Operand */
    int      y_code;         /* instruction code */
    int      y_gr;           /* # of general register */
    int      y_adr;          /* address */
    int      y_label;        /* idx of label */
    int      y_literal;      /* idx of literal table */
}

/*
 *  terminal symbols
 */
%token  <y_code>    Inst_code
%token  <y_gr>      Gr
%token  <y_adr>     Address
%token  <y_label>   Label
%token  <y_literal> Literal

%token  NL
%token  END_OF_FILE

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
        instructions END_OF_FILE {/* end_program(); */}

instructions
    : instruction 
        { g_instruction[g_instruction_no++] = *$1; }
    | instructions NL instruction  
        { g_instruction[g_instruction_no++] = *$3; }
    | error
    | instructions error

instruction
    : Label Inst_code 
            {   $$ = instruction_create($1, $2, NULL); }
    | Label Inst_code operand   
            {   $$ = instruction_create($1, $2, $3); }
    | Label error 
        { printf("Label error\n"); yyerrok; }
    | Inst_code operand   {
        $$ = instruction_create(-1, $1, $2); }
    | Inst_code  {   $$ = instruction_create(-1, $1, NULL); }
    | Inst_code error { printf(" Inst_code error\n"); }

operand
    : Gr ',' Gr 
        { $$ = operand_create($1, $3, NULL); }
    | Gr ',' Address
        {   $$ = operand_create($1, -1, address_create(ADDRESS, $3)); }
    | Gr ',' Address ',' Gr
         {   $$ = operand_create($1, $5, address_create(ADDRESS, $3));}
    | Address 
         {  $$ = operand_create(-1, -1, address_create(ADDRESS, $1)); }
    | Address ',' Gr
         {  $$ = operand_create(-1, $3, address_create(ADDRESS, $1)); }
    | Gr ',' Literal
        {  $$ = operand_create($1, -1, address_create(LITERAL, $3)); }
    | Gr ',' Label
        {  $$ = operand_create($1, -1, address_create(LABEL, $3)); }
    | Label 
        {  $$ = operand_create(-1, -1, address_create(LABEL, $1)); }
