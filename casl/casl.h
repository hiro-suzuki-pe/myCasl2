/*
 *  casl -- header file for symbol table
 */
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH  8       /* max length of strings */
#define cfree(x)    free(x)


#define MAX_INSTRUCTION   1024
#define MAX_LABEL   1024
#define MAX_DC      1024

#define BLOCK_SIZE  1024    /* block size of execution file */
#define BLOCK_HALF  512     /* block size in word (2 bytes) */
#define LONG_INSTRUCTION    0x0100  /* flag of machine word (set if 2 word inst) */
struct inst_table {
    char    name[MAX_LENGTH];   /* Instruction code */
    int  code;               /* machine instruction code */
};
extern struct inst_table  inst_table[];

enum adr_type { ADDRESS, LABEL, LITERAL, UNKNOWN };
struct address {
    enum adr_type   type;
    int value;
};
struct address *address_create(enum adr_type type, int value);

struct operand {
    int  r;              /* r or r1 if adr==-1, no r if r==-1 */
    int  x;              /* x or r2 if adr==-1, no x if x==-1 */
    struct address  adr;     /* address, r -> r1 & x -> r2 if adr=0 */
};
struct operand *operand_create(int r, int x, struct address *adr);

struct instruction {
    char    name[MAX_LENGTH];
    char    label[MAX_LENGTH];
    int  code;
    struct operand ope;
};
extern int  g_instruction_no;
extern struct instruction g_instruction [MAX_INSTRUCTION];
struct instruction *instruction_create(int label_idx, int code, struct operand *ope);
void    instruction_print(void);
void    instruction_set_label_adr(int text_adr);
void    instruction_resolve_address(void);


struct  label_table {
    char    label[MAX_LENGTH];     /* label */
    int  adr;        /* address */
};
extern int  g_label_no;
extern struct label_table  g_label_table[];
void    label_table_print(void);

struct  DC_table {
    char    label[MAX_LENGTH];
    int  val;       /* constant value */
    int  adr;       /* address */
};
extern int  g_DC_no;
extern struct  DC_table    g_DC_table[];
void    DC_table_print();
void    DC_set_adr(int data_adr);

struct LD_header {
    int no_section;
    int ;            /* reserved word */
    int text_top;
    int text_size;   /* word count of text */
    int data_top;
    int data_size;
    int bss_top;
    int bss_size;
    int stack_top;
    int stack_size;
};
extern struct LD_header g_LD_header;
void LD_header_print(void);

void csx_write_file(char *csx_file);


int instruction_word_count(void);
