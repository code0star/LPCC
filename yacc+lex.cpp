%{
#include "y.tab.h"
#include <bits/stdc++.h>
// Add this block to the Lex file
extern "C" {
    int yylex();
}
%}

%%
[0-9]+(\.[0-9]+)? { yylval.num = atof(yytext); return NUMBER; }
[a-zA-Z]+         { yylval.str = strdup(yytext); return ID; }

"+"|"-"|"*"|"/"|"="|"("|")"|"\n" { return yytext[0]; }

[ \t]             ;
.                 { std::cout << "Invalid: " << yytext << std::endl; }
%%

int yywrap() { return 1; }


// yacc part 
%{
#include <bits/stdc++.h>

using namespace std;

// C++ Map for Symbol Table
map<string, double> SYMTAB;

extern "C" {
    int yylex();
    void yyerror(const char *s);
}
%}

%union {
    double num;
    char *str;
}

%token <num> NUMBER
%token <str> ID
%type <num> expr

%left '+' '-'
%left '*' '/'

%%

input:
      /* empty */
    | input line
;

line:
      expr '\n'          { cout << "Result = " << $1 << endl; }
    | ID '=' expr '\n'   { SYMTAB[string($1)] = $3; }
    | '\n'               { }
;

expr:
      expr '+' expr      { $$ = $1 + $3; }
    | expr '-' expr      { $$ = $1 - $3; }
    | expr '*' expr      { $$ = $1 * $3; }
    | expr '/' expr      { if($3 == 0) yyerror("Divide by zero"); else $$ = $1 / $3; }
    | '(' expr ')'       { $$ = $2; }
    | NUMBER             { $$ = $1; }
    | ID                 { $$ = SYMTAB[string($1)]; }
    | ID '(' expr ')'    {
        string func = string($1);
        if(func == "sin") $$ = sin($3);
        else if(func == "cos") $$ = cos($3);
        else if(func == "sqrt") $$ = sqrt($3);
        else { cout << "Unknown function: " << func << endl; $$ = 0; }
    }
;

%%

void yyerror(const char *s) {
    cerr << "Error: " << s << endl;
}

int main() {
    cout << "C++ Scientific Calculator (Bison/Flex)" << endl;
    cout << "Enter expressions (e.g., x = sqrt(16) or sin(0.5))" << endl;
    yyparse();
    return 0;
}
