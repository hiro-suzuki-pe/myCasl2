/*
 *  casl -- header file for symbol table
 */
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH  8       /* max length of strings */

#define ushort  unsigned short
#define cfree(x)    free(x)


#define MAX_INSTRUCTION   1024
#define MAX_LABEL   1024
#define MAX_DC      1024

struct inst_table {
    char    name[MAX_LENGTH];   /* Instruction code */
    ushort  code;               /* machine instruction code */
};
extern struct inst_table  inst_table[];

enum adr_type { ADDRESS, LABEL, LITERAL };
struct address {
    enum adr_type   type;
    ushort value;
};

struct operand {
    ushort  r;              /* r or r1 if adr==0xffff, no r if r==0xffff */
    ushort  x;              /* x or r2 if adr==0xffff, no x if x==0xffff */
    struct address  adr;     /* address, r -> r1 & x -> r2 if adr=0 */
};
struct operand *operand_create(ushort r, ushort x, struct address *adr);

struct instruction {
    char    name[MAX_LENGTH];
    char    label[MAX_LENGTH];
    ushort  code;
    struct operand ope;
};
extern ushort  g_instruction_no;
extern struct instruction g_instruction [MAX_INSTRUCTION];
struct instruction *instruction_create(char *label, ushort code, struct operand *ope);
void    instruction_print();

struct  label_table {
    char    label[MAX_LENGTH];     /* label */
    ushort  adr;        /* address */
};
extern ushort  g_label_no;
extern struct label_table  g_label_table[];
void    label_table_print();

struct  DC_table {
    char    label[MAX_LENGTH];
    ushort  val;
};
extern ushort  g_DC_no;
extern struct  DC_table    g_DC_table[];
void    DC_table_print();




