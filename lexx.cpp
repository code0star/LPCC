%{
#include<bits/stdc++.h>
using namespace std;
set<string>s;

void to_set(char* word){
s.insert(string(word));
return;
}
%}
%%
(while|if|do|else|for) {
to_set(yytext);
cout<<yytext<<" :conditon"<<endl;}
[a-zA-Z][a-zA-Z0-9]* {to_set(yytext);
cout<<yytext<<" :identifier"<<endl;}
[0-9]+ {
to_set(yytext);
cout<<yytext<<" :integer"<<endl;}
. {}
[a-zA-Z] {to_set(yytext);cout<<" :Special"<<endl;}
[ \t\n] {}
%%
int yywrap(){
return 1;}
int main(){
yylex();
cout<<"Symbol table ======="<<endl;
for(auto &it : s){
cout<<it<<endl;
}
return 0;
}
