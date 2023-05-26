%{
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <algorithm>
#include <memory>
#include <cstring>
#include <cdk/compiler.h>
#include <cdk/types/types.h>
#include ".auto/all_nodes.h"
#define LINE                         compiler->scanner()->lineno()
#define yylex()                      compiler->scanner()->scan()
#define yyerror(compiler, s)         compiler->scanner()->error(s)
//-- don't change *any* of these --- END!
%}

%parse-param {std::shared_ptr<cdk::compiler> compiler}

%union {
  //--- don't change *any* of these: if you do, you'll break the compiler.
  YYSTYPE() : type(cdk::primitive_type::create(0, cdk::TYPE_VOID)) {}
  ~YYSTYPE() {}
  YYSTYPE(const YYSTYPE &other) { *this = other; }
  YYSTYPE& operator=(const YYSTYPE &other) { type = other.type; return *this; }

  std::shared_ptr<cdk::basic_type> type;        /* expression type */
  //-- don't change *any* of these --- END!

  int                   i;	/* integer value */
  double                d;
  std::string           *s;	/* symbol name or string literal */
  cdk::basic_node       *node;	/* node pointer */
  cdk::sequence_node    *sequence;
  cdk::expression_node  *expression; /* expression nodes */
  cdk::lvalue_node      *lvalue;
  mml::block_node       *block;
};

%token tTYPE_INT tTYPE_DOUBLE tTYPE_STRING tTYPE_VOID
%token tFOREIGN tFORWARD tPUBLIC tAUTO tPRIVATE
%token tIF tELIF tELSE tWHILE tSTOP tNEXT tRETURN
%token tINPUT tNULL tSIZEOF
%token tBEGIN tEND
%token tGE tLE tEQ tNE tAND tOR
%token tPRINT tPRINTLN

%token <i> tINTEGER
%token <r> tREAL
%token <d> tDOUBLE
%token <s> tIDENTIFIER tSTRING

%nonassoc tELIF tELSE
%nonassoc   '(' ')' '[' ']'

%right '='
%left tOR
%left tAND
%nonassoc '~'
%left tNE tEQ
%left '<' tLE tGE '>' 
%left '+' '-'
%left '*' '/' '%'
%nonassoc tUNARY


%type <node> program declaration instr 
%type <sequence> declarations instrs exprs
%type <expression> expr funccall
%type <lvalue> lval
%type <type> type functype
%type <block> block
%type <s> string

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%

file           : declarations program                       { compiler -> ast($1);}
               | program                                    { compiler -> ast($1);}
               | declarations                               { compiler -> ast($1);}
               |                                            { compiler -> ast(new cdk::sequence_node(LINE));}
               ;

declarations   : declaration                                { $$ = new cdk::sequence_node(LINE, $1);}
               | declarations declaration                   { $$ = new cdk::sequence_node(LINE, $2, $1);}
               ;

declaration    : tFOREIGN type tIDENTIFIER ';'              { $$ = new mml::variable_decl_node(LINE, tFOREIGN, $2, false, *$3, nullptr);}
               | tFORWARD type tIDENTIFIER ';'              { $$ = new mml::variable_decl_node(LINE, tFORWARD, $2, false, *$3, nullptr);}
               | tPUBLIC type tIDENTIFIER ';'               { $$ = new mml::variable_decl_node(LINE, tPUBLIC, $2, false, *$3, nullptr);}
               | tFOREIGN type tIDENTIFIER '=' expr ';'     { $$ = new mml::variable_decl_node(LINE, tFOREIGN, $2, false, *$3, $5);}
               | tFORWARD type tIDENTIFIER '=' expr ';'     { $$ = new mml::variable_decl_node(LINE, tFORWARD, $2, false, *$3, $5);}
               | tPUBLIC type tIDENTIFIER '=' expr ';'      { $$ = new mml::variable_decl_node(LINE, tPUBLIC, $2, false, *$3, $5);}
               | type tIDENTIFIER ';'                       { $$ = new mml::variable_decl_node(LINE, tPRIVATE, $1, false, *$2, nullptr);}
               | type tIDENTIFIER '=' expr ';'              { $$ = new mml::variable_decl_node(LINE, tPRIVATE, $1, false, *$2, $4);}
               | tFOREIGN tAUTO tIDENTIFIER '=' expr ';'    { $$ = new mml::variable_decl_node(LINE, tFOREIGN, true, *$3, $5);}
               | tFORWARD tAUTO tIDENTIFIER '=' expr ';'    { $$ = new mml::variable_decl_node(LINE, tFORWARD, true, *$3, $5);}
               | tPUBLIC tAUTO tIDENTIFIER '=' expr ';'     { $$ = new mml::variable_decl_node(LINE, tPUBLIC, true, *$3, $5);}
               | tAUTO tIDENTIFIER '=' expr ';'             { $$ = new mml::variable_decl_node(LINE, tPRIVATE, true, *$2, $4);}
               | tFOREIGN tIDENTIFIER '=' expr ';'          { $$ = new mml::variable_decl_node(LINE, tFOREIGN, false, *$2, $4);}
               | tFORWARD tIDENTIFIER '=' expr ';'          { $$ = new mml::variable_decl_node(LINE, tFORWARD, false, *$2, $4);}
               | tPUBLIC tIDENTIFIER '=' expr ';'           { $$ = new mml::variable_decl_node(LINE, tPUBLIC, false, *$2, $4);}
               | tIDENTIFIER '=' expr ';'                   { $$ = new mml::variable_decl_node(LINE, tPRIVATE, false, *$1, $3);}
               ;

type           : tTYPE_INT                                  { $$ = cdk::primitive_type::create(4, cdk::TYPE_INT); }
               | tTYPE_DOUBLE                               { $$ = cdk::primitive_type::create(8, cdk::TYPE_DOUBLE); }
               | tTYPE_STRING                               { $$ = cdk::primitive_type::create(4, cdk::TYPE_STRING); }
               | '[' type ']'                               { $$ = cdk::reference_type::create(4, $2); }
               | functype                                   { $$ = $1;}
               ;

functype       : type '<' type '>'                          { $$ = $1;}
               | type '<' '>'                               { $$ = $1;}
               ;

program	     : tBEGIN declarations instrs tEND            { $$ = (new mml::program_node(LINE, $2, $3)); }
               | tBEGIN declarations tEND                   { $$ = (new mml::program_node(LINE, $2, new cdk::sequence_node(LINE))); }
               | tBEGIN instrs tEND                         { $$ = (new mml::program_node(LINE, new cdk::sequence_node(LINE), $2)); }
               | tBEGIN tEND                                { $$ = (new mml::program_node(LINE, new cdk::sequence_node(LINE), new cdk::sequence_node(LINE))); }
	          ;

instrs         : instr                                      { $$ = new cdk::sequence_node(LINE, $1);}
               | instrs instr                               { $$ = new cdk::sequence_node(LINE, $2, $1);}
               ;

instr          : expr ';'                                   { $$ = new mml::evaluation_node(LINE, $1);}
               | exprs tPRINT                               { $$ = new mml::print_node(LINE, $1, false);}
               | exprs tPRINTLN                             { $$ = new mml::print_node(LINE, $1, true);}
               | tSTOP tINTEGER ';'                         { $$ = new mml::stop_node(LINE, $2);}
               | tSTOP ';'                                  { $$ = new mml::stop_node(LINE, 1);}
               | tNEXT tINTEGER ';'                         { $$ = new mml::next_node(LINE, $2);}
               | tNEXT ';'                                  { $$ = new mml::next_node(LINE, 1);}
               | tRETURN expr ';'                           { $$ = new mml::return_node(LINE, $2);}
               | tRETURN ';'                                { $$ = new mml::return_node(LINE, nullptr);}
               | tIF '(' expr ')' instr %prec tELIF         { $$ = new mml::if_node(LINE, $3, $5); }
//--               | tIF '(' expr ')' instr tELSE instr         { $$ = new mml::if_else_node(LINE, $3, $5, $7); }
               | tWHILE '(' expr ')' instr                  { $$ = new mml::while_node(LINE, $3, $5);}
               | block                                      { $$ = $1;}
               ;

exprs          : expr                                       { $$ = new cdk::sequence_node(LINE, $1);}
               | exprs expr                                 { $$ = new cdk::sequence_node(LINE, $2, $1);}
               ;

block          : '{' declarations instrs '}'                { $$ = new mml::block_node(LINE, $2, $3);}
               | '{' declarations '}'                       { $$ = new mml::block_node(LINE, $2, new cdk::sequence_node(LINE));}
               | '{' instrs '}'                             { $$ = new mml::block_node(LINE, new cdk::sequence_node(LINE), $2);}
               | '{' '}'                                    { $$ = new mml::block_node(LINE, new cdk::sequence_node(LINE), new cdk::sequence_node(LINE));}
               ;

//--function       : '(' ')' '-' '>' type block                 { $$ = new mml::func_definition_node(LINE, new cdk::sequence_node(LINE), $5, $6);}
//--               | '(' declarations ')' '-' '>' type block    { $$ = new mml::func_definition_node(LINE, $2, $6, $7);}
 //--              ;

funccall       : tIDENTIFIER '('  ')'                       { $$ = new mml::func_call_node(LINE, *$1, new cdk::sequence_node(LINE));}
               | tIDENTIFIER '(' exprs ')'                  { $$ = new mml::func_call_node(LINE, *$1, $3);}
               ;

expr           : tINTEGER                                   { $$ = new cdk::integer_node(LINE, $1); }
               | tDOUBLE                                    { $$ = new cdk::double_node(LINE, $1); }
               | tNULL                                      { $$ = new mml::null_node(LINE); }
               | string                                     { $$ = new cdk::string_node(LINE, $1); }
               | funccall                                   { $$ = $1;}
//--               | function                                   { $$ = $1;}
               | '+' expr %prec tUNARY                      { $$ = $2; }
               | '-' expr %prec tUNARY                      { $$ = new cdk::neg_node(LINE, $2); }
               | '~' expr                                   { $$ = new cdk::not_node(LINE, $2); }
               | expr '+' expr	                         { $$ = new cdk::add_node(LINE, $1, $3); }
               | expr '-' expr	                         { $$ = new cdk::sub_node(LINE, $1, $3); }
               | expr '*' expr	                         { $$ = new cdk::mul_node(LINE, $1, $3); }
               | expr '/' expr	                         { $$ = new cdk::div_node(LINE, $1, $3); }
               | expr '%' expr	                         { $$ = new cdk::mod_node(LINE, $1, $3); }
               | expr '<' expr	                         { $$ = new cdk::lt_node(LINE, $1, $3); }
               | expr '>' expr	                         { $$ = new cdk::gt_node(LINE, $1, $3); }
               | expr tGE expr	                         { $$ = new cdk::ge_node(LINE, $1, $3); }
               | expr tLE expr                              { $$ = new cdk::le_node(LINE, $1, $3); }
               | expr tNE expr	                         { $$ = new cdk::ne_node(LINE, $1, $3); }
               | expr tEQ expr	                         { $$ = new cdk::eq_node(LINE, $1, $3); }
               | expr tAND expr                             { $$ = new cdk::and_node(LINE, $1, $3); }
               | expr tOR expr                              { $$ = new cdk::or_node (LINE, $1, $3); }
               | tINPUT                                     { $$ = new mml::input_node(LINE); }
               | tSIZEOF '(' expr ')'                       { $$ = new mml::sizeof_node(LINE, $3); }
               | '(' expr ')'                               { $$ = $2; }
               | '[' expr ']'                               { $$ = new mml::mem_alloc_node(LINE, $2);}
               | lval '?'                                   { $$ = new mml::address_node(LINE, $1);}
               | lval                                       { $$ = new cdk::rvalue_node(LINE, $1); }  //FIXME
               | lval '=' expr                              { $$ = new cdk::assignment_node(LINE, $1, $3); }
               ;

lval           : tIDENTIFIER                                { $$ = new cdk::variable_node(LINE, $1); }
               ;

string         : tSTRING                                    { $$ = $1; }
               | string tSTRING                             { $$ = $1; $$->append(*$2); delete $2; }
               ;


%%
