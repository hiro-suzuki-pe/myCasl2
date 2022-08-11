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


struct instruction {
    char    name[MAX_LENGTH];
    char    label[MAX_LENGTH];
    ushort  code;
    ushort   r;              /* r or r1 if adr==0xffff, no r if r==0xffff */
    ushort   x;              /* x or r2 if adr==0xffff, no x if x==0xffff */
    ushort   adr;            /* address, r -> r1 & x -> r2 if adr=0 */
};
extern ushort  g_instruction_no;
extern struct instruction g_instruction [MAX_INSTRUCTION];

struct  label_table {
    char    label[MAX_LENGTH];     /* label */
    ushort  adr;        /* address */
};
extern ushort  g_label_no;
extern struct label_table  g_label_table[];

struct  DC_table {
    char    label[MAX_LENGTH];
    ushort  val;
};
extern ushort  g_DC_no;
extern struct  DC_table    g_DC_table[];


void    print_instruction();
void    print_label_table();
void    print_DC_table();
