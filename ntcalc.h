/* ntcalc.h */

#ifndef _NTCALC_H
#define _NTCALC_H

#include <fcntl.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>


/*
 * scan.c
 */

/*
# Scanner EBNF
tokenlist  ::= (token)*
token      ::= intlit | hexlit | binlit | symbol
symbol     ::= '+' | '-' | '*' | '/' | '>>' | '<<' | '~' | '&' | '|' | '^' | '>-'
intlit     ::= digit (digit)*
hexlit     ::= '0x' | hexdigit (hexdigit)*
binlit     ::= '0b' ['0', '1'] (['0', '1'])*
hexdigit   ::= 'a' | ... | 'f' | 'A' | ... | 'F' | digit
digit      ::= '0' | ... | '9'
# Ignore
whitespace ::= ' ' | '\t' (' ' | '\t')*
*/

#define SCAN_TOKEN_LEN 32
#define SCAN_TOKEN_TABLE_LEN 1024
#define SCAN_INPUT_LEN 2048

enum scan_token_enum {
    TK_INTLIT, /* -123, 415 */
    TK_HEXLIT,
    TK_BINLIT, /* 0b1010, 0b00001111 */
    TK_LPAREN, /* ( */
    TK_RPAREN, /* ) */
    TK_PLUS,   /* + */
    TK_MINUS,  /* - */
    TK_MULT,   /* * */
    TK_DIV,    /* / */
    TK_NOT,    /* ~ */
    TK_LSL,    /* << */
    TK_LSR,     /* >> */
    TK_AND,     /* & */
    TK_OR,      /* | */
    TK_XOR,     /* ^ */
    TK_ASR,     /* >- */
    TK_EOT,    /* end of text */
    TK_ANY,    /* wildcard for parsing */
};

#define SCAN_TOKEN_STRINGS {"TK_INTLIT", "TK_HEXLIT", "TK_BINLIT", \
                            "TK_LPAREN", "TK_RPAREN", \
                            "TK_PLUS", "TK_MINUS", "TK_MULT", "TK_DIV", \
                            "TK_NOT", "TK_LSL", "TK_LSR", "TK_AND", "TK_OR", "TK_XOR", "TK_ASR", "TK_EOT", "TK_ANY"};

struct scan_token_st {
    enum scan_token_enum id;
    char value[SCAN_TOKEN_LEN];
};

struct scan_table_st {
    struct scan_token_st table[SCAN_TOKEN_TABLE_LEN];
    int len;
    int next;
};

void scan_table_init(struct scan_table_st *st);
void scan_table_scan(struct scan_table_st *st, char *input, int len);
void scan_table_print(struct scan_table_st *st);
struct scan_token_st * scan_table_get(struct scan_table_st *st, int i);
bool scan_table_accept(struct scan_table_st *st, enum scan_token_enum tk_expected);

/*
 * parse.c
 */

 /*
 A simple grammar for the ntcalc langauge
 # Parser
 program    ::= expression EOT
 
 expression ::= operand (operator operand)*
 operand    ::= intlit
            | hexlit
            | binlit
            | '-' operand
            | '~' operand
            | '(' expression ')'
*/

enum parse_expr_enum {EX_INTVAL, EX_OPER1, EX_OPER2};
enum parse_oper_enum {OP_PLUS, OP_MINUS, OP_MULT, OP_DIV,
                      OP_NOT, OP_LSL, OP_LSR, OP_AND, OP_OR, OP_XOR, OP_ASR};

struct parse_node_st {
    enum parse_expr_enum type;
    union {
        struct {uint32_t value;} intval;
        struct {enum parse_oper_enum oper;
                struct parse_node_st *operand;} oper1;
        struct {enum parse_oper_enum oper;
                struct parse_node_st *left;
                struct parse_node_st *right;} oper2;
    };                
};

#define PARSE_TABLE_LEN 1024

struct parse_table_st {
    struct parse_node_st table[PARSE_TABLE_LEN];
    int len;
};

void parse_table_init(struct parse_table_st *pt);
struct parse_node_st * parse_node_new(struct parse_table_st *pt);
void parse_tree_print(struct parse_node_st *np);
struct parse_node_st * parse_program(struct parse_table_st *pt, 
                                     struct scan_table_st *st);

/*
 * conv.c
 */

 uint32_t conv_binstr_to_uint32(char *binstr);
 void conv_uint32_to_binstr(uint32_t v, char *binstr, int width);
 uint32_t conv_hexstr_to_uint32(char *hexstr);
 void conv_uint32_to_hexstr(uint32_t v, char *hexstr, int width);
 uint32_t conv_decstr_to_uint32(char *decstr);
 void conv_uint32_to_decstr(uint32_t v, char *decstr, bool u, int width);

/*
 * eval.c
 */

 uint32_t eval_tree(struct parse_node_st *np);


#endif /* _NTCALC_H */