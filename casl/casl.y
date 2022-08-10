/*
 *  casl parser
 *  syntax analysis with error recovery
 *  symbol table
 *  memeory allocation
 *  code generation
 *  (s/r conflicts: one on ELSE, one on error)
 */

%{
//#include "symtab.h"     /* symbol table mnemonics */
//#include "sim.h"        /* code generation mnemonics */

//#define OFFSET(x)       (((struct symtab *)x)->s_offset)
//#define NAME(x)         (((struct symtab *)x)->s_name)

// extern int l_offset, l_max;

#define ushort  unsigned short
struct inst_tab {
    ushort   inst_code;      /* Instruction code */
    ushort   r;              /* r or r1 if adr==0xffff, no r if r==0xffff */
    ushort   x;              /* x or r2 if adr==0xffff, no x if x==0xffff */
    ushort   adr;            /* address, r -> r1 & x -> r2 if adr=0 */
};
%}

%union {
    struct inst_tab *y_inst;  /* Instruction */
    unsigned short y_gr;      /* # of general register */
    unsigned  y_adr;          /* address */
    unsigned  y_lab;          /* label */
}

/*
 *  terminal symbols
 */
%token  <y_inst>    Inst_code
%token  <y_gr>      Gr
%token  <y_adr>     Address
%token  <y_label>   Label

/*
%token	START
%token	END
%token	DS
%token	DC
%token	IN
%token	OUT
%token	RPUSH
%token	RPOP
%token	NOP
%token	LD
%token	ST
%token	LAD
%token	ADDA
%token	ADDL
%token	SUBA
%token	SUBL
%token	AND
%token	OR
%token	XOR
%token	CPA
%token	CPL
%token	SLA
%token	SRA
%token	SLL
%token	SRL
%token	JPL
%token	JMI
%token	JNZ
%token	JNE
%token	JOV
%token	JUMP
%token	PUSH
%token	POP
%token	CALL
%token	RET
%token	SVC
%token	RSVD1
%token	RSVD2
%token	RSVD3
%token	RSVD4
*/

/*
 *  typed non-terminal symbols
 */
/*
%type   <y_ope> operand
*/
%type   <y_inst> instruction operand

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
    : Label Inst_code operand   { $$ = $2; printf("Label Inst_code operand.\n"); }
    | Label error {yyerrok; }
    | Inst_code operand  { $$ = $2;  printf("Inst_code operand .\n"); }
    | Inst_code  { $$ = $1;  printf("Inst_code (s).\n"); }
    | Inst_code error

operand
    : Gr ',' Gr
        { $$->inst_code = 0xffff; $$->r = $1; $$->x = $3; $$->adr = 0; printf("operand.\n");}
    | Gr ',' Address
        { $$->inst_code = 0xffff; $$->r = $1; $$->x = 0xffff; $$->adr = $3; }
    | Gr ',' Address ',' Gr
        { $$->inst_code = 0xffff; $$->r = $1; $$->x = $5; $$->adr = $3; }
    | Address 
        { $$->inst_code =  $$->r = $$->x = 0xffff; $$->adr = $1; }
    | Address ',' Gr
        { $$->inst_code =  $$->r = 0xffff; $$->x = $3; $$->adr = $1; }
