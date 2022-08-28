/*
 *  casl -- symbol table definition and manipulation
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "casl.h"
#include "casl.tab.h"

struct LD_header g_LD_header;
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

    instruction_set_label_adr(BLOCK_SIZE);
    DC_set_adr(g_LD_header.data_top);

    instruction_print();
    label_table_print();
    DC_table_print();
    LD_header_print();

    instruction_resolve_address();
    instruction_print();

    char csx_file[255];
    int i;
    if (argc == 2){
        for (i = strlen(argv[1]); i > 0; i--){
            if (argv[1][i-1] == '.') break;
        }
        if (i > 0)
            strcpy(csx_file, argv[1]);
            strcpy(&csx_file[i], "csx");
    }
    else
        strcpy(csx_file, "temp.csx");

    csx_write_file(csx_file);
    exit(0);
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

int DC_lookup(char *str)
{
    int i;
    int idx;

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
        g_DC_table[i].adr = 0;
        g_DC_no++;
    }
    return i;
}

void DC_set_adr(int data_top)
{
    if (g_DC_no == 0)
        g_LD_header.bss_top = data_top;
    else{
        for (int i = 0; i < g_DC_no; i++)
            g_DC_table[i].adr = data_top + i;    
        g_LD_header.bss_top = ((data_top + g_DC_no -1)/BLOCK_SIZE + 1) * BLOCK_SIZE;
    }
    g_LD_header.bss_size += g_DC_no;
    
    return;
}

void    instruction_print()
{
    struct instruction inst;
    struct operand  ope;
    struct address  adr;

    printf("\n<<instruction table (%d entry)>>\n", 
        g_instruction_no);
    printf("%-8s %-8s %-2s %-2s %-2s %-2s %-s\n",
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
            printf("A  %04X", adr.value&0x0ffff);
        else if (adr.type == LABEL)
            printf("L  %04X", adr.value);
        else if (adr.type == LITERAL)
            printf("N  %04X", adr.value);
        printf("\n");
    }
}

void    instruction_set_label_adr(int pc)
{
    g_LD_header.text_top = pc;

    for (int i = 0; i < g_instruction_no; i++){
        struct instruction inst = g_instruction[i];

        if (strcmp(inst.label, "") != 0){
            int idx = label_lookup(inst.label);
            g_label_table[idx].adr = pc;
        } 
        
        if (inst.code >= 0 && inst.code <= 31){
            if (inst.ope.adr.type == UNKNOWN)
                pc += 1;    /* 1 word inst */
            else        
                pc += 2;    /* 2 word inst */                           
        }
    }
    g_LD_header.text_size = pc - g_LD_header.text_top;

    g_LD_header.data_top = g_LD_header.text_top;
    for (int i = g_LD_header.text_size; i > 0; i -= BLOCK_SIZE)
        g_LD_header.data_top += BLOCK_SIZE;

    return;
}

void    instruction_resolve_address(void)
{
    for (int i = 0; i < g_instruction_no; i++){
        struct instruction *inst = &g_instruction[i];

        if (inst->ope.adr.type == LABEL){
            inst->ope.adr.type = ADDRESS;
            inst->ope.adr.value = g_label_table[inst->ope.adr.value].adr;
        } else if (inst->ope.adr.type == LITERAL){
            inst->ope.adr.type = ADDRESS;
            inst->ope.adr.value = g_DC_table[inst->ope.adr.value].adr;
        } 
    }
    return;
}

void  csx_write_file(char *csx_file)
{
    printf("csx_write_file(%s)\n", csx_file);

    /* set header buffer */
    int hsize = BLOCK_SIZE;
    unsigned short *hbuf = (unsigned short *)malloc(hsize);
    memset(hbuf, 0, hsize);
    hbuf[0] = (unsigned short)g_LD_header.no_section;
    hbuf[2] = (unsigned short)g_LD_header.text_top;
    hbuf[3] = (unsigned short)g_LD_header.text_size;
    hbuf[4] = (unsigned short)g_LD_header.data_top;
    hbuf[5] = (unsigned short)g_LD_header.data_size;
    hbuf[6] = (unsigned short)g_LD_header.bss_top;
    hbuf[7] = (unsigned short)g_LD_header.bss_size;
    hbuf[8] = (unsigned short)g_LD_header.stack_top;
    hbuf[9] = (unsigned short)g_LD_header.stack_size;

    /* set machine instruction buffer */
    int msize = ((g_LD_header.text_size * 2 -1)/BLOCK_SIZE + 1) * BLOCK_SIZE;
    unsigned short *mbuf = (unsigned short *)malloc(msize);
    memset(hbuf, 0, msize);
    int j = 0;
    for (int i = 0; i < g_instruction_no; i++){
        struct instruction inst = g_instruction[i];

        if (inst.code > 0 && inst.code > 31) continue;

        int hw = ((0x003f & inst.code) << 10) |
                 ((0x0f & inst.ope.r) << 4) | (0x0f & inst.ope.x);
        if (inst.ope.adr.type == ADDRESS){
            hw |= LONG_INSTRUCTION;
            mbuf[j++] = hw;
            mbuf[j++] = (unsigned short)(0x0ffff & inst.ope.adr.value);
        }
        else
            mbuf[j++] = hw;
    }

    /* set data buffer */
    int dsize = ((2 * g_LD_header.data_size -1)/BLOCK_SIZE + 1) * BLOCK_SIZE;
    unsigned short *dbuf = (unsigned short *)malloc(dsize);
    memset(dbuf, 0, dsize);
    for (int i = 0; i < g_DC_no; i++)
        hbuf[i] = (unsigned short)(0x0ffff & g_DC_table[i].val);

    int fd = open(csx_file, O_WRONLY|O_CREAT, S_IRWXU|S_IRWXU|S_IRWXO);

    /* write buffer in csx file */
    int n = write(fd, hbuf, hsize);
    if (n != hsize){
        fprintf(stderr, "Header write error:%d of %d words written in %s\n",
            n, hsize, csx_file);
        exit(2);
    }
    n = write(fd, mbuf, msize);
    if (n != msize){
        fprintf(stderr, "Machine instruction write error:%d of %d words written in %s\n",
            n, msize, csx_file);
        exit(2);
    }
    n = write(fd, dbuf, dsize);
    if (n != dsize){
        fprintf(stderr, "data write error:%d of %d words written in %s\n",
            n, dsize, csx_file);
        exit(2);
    }
    close(fd);
    return j;
}

void    label_table_print(void)
{
    struct label_table lbl;

    printf("<<label table (%d entry)>>\n", g_label_no);
    for (int i = 0; i < g_label_no; i++){
        lbl = g_label_table[i];
        printf("%-8s  %04X\n", lbl.label, lbl.adr);
    }
}

void    DC_table_print()
{
    struct DC_table dc;

    printf("<<DC table (%d entry)>>\n", g_DC_no);
    for (int i = 0; i < g_DC_no; i++){
        dc = g_DC_table[i];
        printf("%-8s  %04X %04X\n", dc.label, dc.val, dc.adr);
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

/* address_create()
 *  adr_type: 
 *  value:  = address if adr_type = ADDRESS 
 *          = label idx if adr_type = LABEL
 *          = DC idx if adr_type = LITERAL
 */
struct address *address_create(enum adr_type type, int value)
{
    struct address *paddr = (struct address *)malloc(sizeof (struct address));

    paddr->type = type;
    paddr->value = value;      

    return paddr;
}

void    LD_header_print()
{
    printf("No of section: %d\n", g_LD_header.no_section);
    printf("text: %04x, %4d\n", g_LD_header.text_top,  g_LD_header.text_size);
    printf("data: %04x, %4d\n", g_LD_header.data_top,  g_LD_header.data_size);
    printf("bss : %04x, %4d\n", g_LD_header.bss_top,  g_LD_header.bss_size);
}
