%option c++ prefix="mml_scanner_" outfile="mml_scanner.cpp"
%option stack noyywrap yylineno 8bit debug
%{ 
// make relevant includes before including the parser's tab file
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>
#include "mml_parser.tab.h"

static std::ostringstream strlit;

// don't change this
#define yyerror LexerError
%}

SPACE         ([ \t\r]|\n)
B10_INT       [1-9][0-9]*|0  
B8_INT        0[0-7]+ 
DOUBLE        ([0-9]*\.[0-9]+|[0-9]+\.[0-9]*)([eE][-+]?[0-9]+)?

LETTER        [A-Za-z_]
IDENTIFIER    {LETTER}({LETTER}|[[:digit:]])*

%x X_COMMENT
%x X_STRING  X_BACKSLASH   X_CHAR    X_CHAR_END

%%
  yydebug=1; set_debug(1);

"int"                  return tTYPE_INT;
"double"               return tTYPE_DOUBLE;
"string"               return tTYPE_STRING;
"void"                 return tTYPE_VOID;

"foreign"              return tFOREIGN;
"forward"              return tFORWARD;
"public"               return tPUBLIC;
"auto"                 return tAUTO;

"if"                   return tIF; 
"elif"                 return tELIF;
"else"                 return tELSE;
"while"                return tWHILE; 
"stop"                 return tSTOP;
"next"                 return tNEXT;
"return"               return tRETURN; 

"input"                return tINPUT;
"null"                 return tNULL;
"sizeof"               return tSIZEOF;

"begin"                return tBEGIN; 
"end"                  return tEND; 


"<"                    return tLT;
">"                    return tGT;
">="                   return tGE; 
"<="                   return tLE; 
"=="                   return tEQ; 
"!="                   return tNE; 
"&&"                   return tAND;
"||"                   return tOR;

"!"                    return tPRINT;
"!!"				   			   return tPRINTLN;


[-%\]{}()<>!*/~+&|[;?,=.@]               return *yytext;

[A-Za-z][A-Za-z0-9_]*       yylval.s = new std::string(yytext); return tIDENTIFIER;



{B10_INT}                   yylval.i = strtol(yytext, NULL, 10); return tINTEGER;
{B8_INT}                    yylval.i = strtol(yytext, NULL, 8); return tINTEGER;


{DOUBLE}                    yylval.d = strtof(yytext, NULL); return tDOUBLE; 

"//".*$                /* match the whole line (it is ignored) */

"/*"                   yy_push_state(X_COMMENT);
<X_COMMENT>"*/"        yy_pop_state();
<X_COMMENT>.|"\n"      ;  /* ignore inside comments */



"\""                           yy_push_state(X_STRING);
<X_STRING>\\                   yy_push_state(X_BACKSLASH);
<X_STRING>"\""                 {
                                 yylval.s = new std::string(strlit.str());
                                 strlit.str("");
                                 yy_pop_state();
                                 return tSTRING;
                               }
<X_STRING>.                    strlit << *yytext;

<X_BACKSLASH>n                 strlit << '\n'; yy_pop_state();
<X_BACKSLASH>r                 strlit << '\r'; yy_pop_state();
<X_BACKSLASH>t                 strlit << '\t'; yy_pop_state();
<X_BACKSLASH>\\                strlit << '\\'; yy_pop_state();

<X_BACKSLASH>[[:xdigit:]]{1,3} {
                                 strlit << (char)(unsigned char)strtoul(yytext, NULL, 8);
                                 yy_pop_state();
                               }
<X_BACKSLASH>.                 strlit << *yytext; yy_pop_state();


[[:space:]]         /* ignore remaining white space inside the program */
.                      yyerror(yytext);    /* all other chars: error! */

%%
