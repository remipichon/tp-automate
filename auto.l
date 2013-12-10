%{
#include "auto.tab.h"
%}
%%
[a-zA-Z]   {yylval.c=yytext;
            return VARIABLE;}
"("        {return '(';}
")"        {return ')';}
"#"        {yylval.c=yytext;
            return SHARP;}
"*"        {yylval.c=yytext;
            return STAR;}
"+"        {yylval.c=yytext;
            return OR;}
"."        {yylval.c=yytext;
            return AND;}
