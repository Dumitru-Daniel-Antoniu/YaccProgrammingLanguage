%{
#include <stdio.h>
#include "y.tab.h"
%}
%option noyywrap
%%
"int"|"float"|"char"|"string" {yylval.characters=strdup(yytext); return TIP;}
"const int"|"const float"|"const char"|"const string" {yylval.characters=strdup(yytext); return CTIP;}
"Eval" {yylval.characters=strdup(yytext); return EVAL;}
"Typeof" {yylval.characters=strdup(yytext); return TYPE_OF;}

"class" {return CLASS;}
"private" {return PRIVATE;} 
"public" {return PUBLIC;}
"start" {return BGIN;}
"end" {return END;}
"if" {return IF;}
"else" {return ELSE;}
"for" {return FOR;}
"while" {return WHILE;}
[a-zA-Z][a-zA-Z0-9]* {yylval.characters=strdup(yytext);return ID;}
["][a-zA-Z0-9 \t]*["]+ {yylval.characters=strdup(yytext); return STRING;} 
'[a-zA-Z]' {yylval.characters=strdup(yytext); return CARACTER;}


"=" {return ASSIGN;}

[0-9]*  {yylval.integers=atoi(yytext); return NR_INT;}
[0-9]+"."[0-9]*	{ yylval.floats = atof(yytext); return NR_FLOAT;}

"&&" {return AND;}
"||" {return OR;}
">="|"<="|"=="|"!="|"<"|">" {return COMPARE;}

[ \t] ;
\n {yylineno++;}
. {return yytext[0];}
%%