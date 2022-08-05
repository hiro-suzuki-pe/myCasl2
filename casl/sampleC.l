%{
    /*
     *  sample C -- lexical analysis
     */

#ifdef DEBUG       /* debugging version - if assert ok */

#include <assert.h>


main()
{
    char *p;

    assert(sizeof(int) >= sizeof(char *));
    while(p = (char *)yylex())
    printf("%-10.10s is \"%s\"\n", p, yytext);
}

s_lookup()  {}
int yynerrs = 0;

//#define token(x)    (int) "x"
#define token(x)    x

#else   !DEBUG      /* production version */

#include "sampleC.tab.h"
#define token(x)    x
int yynerrs = 0;

#endif  DEBUG

#define END(v)  (v-1 + sizeof v/sizeof v[0])
static int screen();
%}

letter      [a-zA-Z_]
digit       [0-9]
letter_or_digit [a-zA-Z_0-9]
white_space [ \t\n]
blank       [ \t]
other       .

%%

^"#"{blank}*{digit}+({blank}+.*)?\n     yymark();
">="        return token(GE);
"<="        return token(LE);
"=="        return token(EQ);
"!="        return token(NE);
"+="        return token(PE);
"-="        return token(ME);
"*="        return token(TE);
"/="        return token(DE);
"%="        return token(RE);
"++"        return token(PP);
"--"        return token(MM);

{letter}{letter_or_digit}*      return screen();

{digit}+    { s_lookup(token(Constant));
              return token(Constant);
            }

{white_space}+  ;
{other}     return token(yytext[0]);

%%
/*
 *  reserved word screener
 */
struct rwtable {     /* reserved word table */
    char *rw_name;          /* representation */
    int rw_yylex;           /* yylex() value */
 };
 static struct rwtable  rwtable[] = {            /* sorted */
    "break",    token(BREAK),
    "continue", token(CONTINUE),
    "else",     token(ELSE),
    "if",       token(IF),
    "int",      token(INT),
    "return",   token(RETURN),
    "while",    token(WHILE),
 };

static int screen()
{
    struct rwtable *low = rwtable,
    *high = END(rwtable),
    *mid;
    int c;

    while(low <= high)
    {
        mid = low + (high-low)/2;
        if ((c = strcmp(mid->rw_name, yytext)) == 0)
            return mid->rw_yylex;
        else if (c < 0)
            low = mid + 1;
        else
            high = mid - 1;
    }
    s_lookup(token(Identifier));
    return token(Identifier);
}

#ifdef DEBUG

s_lookup(yylex)
int yylex;          /* Constant or Identifier */
{
//    extern char yytext[];
    extern char *yytext;

    switch(yylex){
        case Constant:
            yylval.y_str = strsave(yytext);
            break;
        case Identifier:
            if (yylval.y_sym = s_find(yytext))
               break;  
            yylval.y_sym = s_create(yytext);
            break;
        default:
            bug("s_lookup");
    }
}
#endif