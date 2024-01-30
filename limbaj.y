%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include "symbol_table_functions.h"
#include "symbol_table.h"

#define LIMIT 1000
extern FILE* yyin;
extern char* yytext;
extern int yylineno;

char list_elements[100];
char name_array[100];
char parameters1[500]=" ";
char parameters2[500]=" ";
int no_elements_arr=0;
char num[50];

char type_of[100];
char type_of_global[100];
char one_var[100];
char params_call[100];
char copy[100];
int tester=1;
int constant = 0;
%}
%union{
    char* characters;
    int integers;
    float floats;
    struct AST* node;
}
%token<characters> TIP CTIP
%token<characters> ID EVAL TYPE_OF
%token<characters> STRING
%token<characters> CARACTER
%token<integers> NR_INT
//%token<integers> BOOL
%token<floats> NR_FLOAT
%token<characters> ASSIGN CLASS PRIVATE PUBLIC BGIN END IF ELSE FOR WHILE COMPARE AND OR
%type<characters> parametrii
%type<characters> functie
%type<characters> functii
%type<node> exp_arit
%type<node> valoare
%left OR
%left AND
%left COMPARE
%left '+''-'
%left '*''/'
%nonassoc UMINUS
%start program
%%
program : declaratii functii classes main {write_file(); printf("program sintactic corect!\n");}
        ;
declaratii : declaratie ';' 
	   | declaratii declaratie ';'
           ;

declaratie : TIP ID { 
                if(Check_declared($2)==1)
                {
                        printf("Variabila declarata anterior!Eroare la linia %d\n",yylineno);
                        return 0;
                }else
                        insert_variables($1,$2,0,0);
                }
            | TIP ID ASSIGN NR_INT{
                if(Check_declared($2)==1)
                {
                        printf("Variabila declarata anterior!Eroare la linia %d\n",yylineno);
                        return 0;
                }else
                        insert_variables($1,$2,0,0);
                        int k = insertInt($2,$4);
                        if(k == 0)
                        {
                                printf("Variabila este de tip INT! Linia: %d\n",yylineno);
                                return 0;
                        }
                }
           | TIP ID ASSIGN NR_FLOAT{
                if(Check_declared($2)==1)
                {
                        printf("Variabila declarata anterior!Eroare la linia %d\n",yylineno);
                        return 0;
                }else
                        insert_variables($1,$2,0,0);
                        int k = insertFloat($2,$4);
                        if(k == 0)
                        {
                                printf("Variabila este de tip FLOAT! Linia: %d\n",yylineno);
                                return 0;
                        }
                }
           | TIP ID ASSIGN CARACTER{
                if(Check_declared($2)==1)
                {
                        printf("Variabila declarata anterior!Eroare la linia %d\n",yylineno);
                        return 0;
                }else{
                        insert_variables($1,$2,0,0);
                        int k = insertChar($2,$4);
                        if(k==0)
                        {
                        printf("Variabila nu este de tip CHAR! Linia : %d\n",yylineno);
                        return 0;
                        }
                }
                        
           }
           | TIP ID ASSIGN STRING {
                if(Check_declared($2)==1)
                {
                        printf("Variabila declarata anterior!Eroare la linia %d\n",yylineno);
                        return 0;
                }else{
                        insert_variables($1,$2,0,0);
                        int k=insertString($2,$4);
                        if(k==0)
                        {
                                printf("Variabila trebuie sa fie declarata de tip STRING! Linie: %d\n",yylineno);
                                return 0;
                        }
                }
           }
           | TIP ID ASSIGN exp_bool{
                if(Check_declared($2)==1)
                {
                        printf("Variabila declarata anterior!Eroare la linia %d\n",yylineno);
                        return 0;
                }else
                        insert_variables($1,$2,0,0);
           }
           | TIP ID '[' NR_INT ']' ASSIGN '[' lista_array ']'{
                char convertor_int[100];
                sprintf(convertor_int, "%d",$4);
                strcpy(name_array,$2);
                strcat(name_array,"[");
                strcat(name_array,convertor_int);
                strcat(name_array,"]");
                strcat(list_elements,"}");
                if(strcmp($1,"int") == 0){
                        if(Check_declared($2)==1)
                        {
                                printf("Variabila declarata anterior!Eroare la linia %d\n",yylineno);
                                return 0;
                        }else if(no_elements_arr > $4){
                                printf("Ati introdus prea multe elemente!\n");
                                printf("Vectorul '%s' a fost declarat cu lungimea %d",$2,$4);
                                printf(" si s-a incercat sa se puna %d elemente! Linia: %d\n",no_elements_arr,yylineno);
                                return 0;
                        }else{
                                insert_variables($1,name_array,0,1);
                                insertString(name_array,list_elements);
                                no_elements_arr=0;
                        }
                }else {
                        printf("Puteti declara vectori doar de tipul INT! Linie: %d\n", yylineno);
                        return 0;
                }
                bzero(name_array,100);
                bzero(list_elements,100);
           }
           /*| TIP ID ASSIGN exp_arit{
                if(Check_declared($2)==1)
                {
                        printf("Variabila declarata anterior!Eroare la linia %d\n",yylineno);
                        return 0;
                }else
                        insert_variables($1,$2,0,0);
           }*/
           | CTIP ID {
                printf("Variabila %s declarata de tip const! Trebuie initializata la linia %d.\n",$2,yylineno);
                return 0;
                }
           | CTIP ID ASSIGN CARACTER{
                if(Check_declared($2)==1)
                {
                        printf("Variabila declarata anterior!Eroare la linia %d\n",yylineno);
                        return 0;
                }else{
                        insert_variables($1,$2,1,0);
                        int k = insertChar($2,$4);
                        if(k==0)
                        {
                        printf("Variabila nu este de tip CHAR! Linia : %d\n",yylineno);
                        return 0;
                        }
                }
           }
           | CTIP ID ASSIGN STRING{
                if(Check_declared($2)==1)
                {
                        printf("Variabila declarata anterior!Eroare la linia %d\n",yylineno);
                        return 0;
                }else{
                        insert_variables($1,$2,1,0);
                        int k=insertString($2,$4);
                        if(k==0)
                        {
                                printf("Variabila trebuie sa fie declarata de tip STRING! Linie: %d\n",yylineno);
                                return 0;
                        }
                }
           }
           /*| CTIP ID ASSIGN exp_arit{
                if(Check_declared($2)==1)
                {
                        printf("Variabila declarata anterior!Eroare la linia %d\n",yylineno);
                        return 0;
                }else insert_variables($1,$2,1,0);
           }*/
           | CTIP ID ASSIGN NR_INT{
                 if(Check_declared($2)==1)
                {
                        printf("Variabila declarata anterior!Eroare la linia %d\n",yylineno);
                        return 0;
                }else
                        insert_variables($1,$2,1,0);
                        int k = insertInt($2,$4);
                        if(k == 0)
                        {
                                printf("Variabila este de tip INT! Linia: %d\n",yylineno);
                                return 0;
                        }
                }
           | CTIP ID ASSIGN NR_FLOAT{
                if(Check_declared($2)==1)
                {
                        printf("Variabila declarata anterior!Eroare la linia %d\n",yylineno);
                        return 0;
                }else
                        insert_variables($1,$2,0,0);
                        int k = insertFloat($2,$4);
                        if(k == 0)
                        {
                                printf("Variabila este de tip FLOAT! Linia: %d\n",yylineno);
                                return 0;
                        }
                }
           | CTIP ID ASSIGN exp_bool{
               if(Check_declared($2)==1)
                {
                        printf("Variabila declarata anterior!Eroare la linia %d\n",yylineno);
                        return 0;
                }
        
                else insert_variables($1,$2,0,0);
           }
           ;
lista_array: val_arr
           | lista_array ',' val_arr 
           ;
val_arr : NR_INT {
               char convertor_int[30];
                sprintf(convertor_int, "%d",$1);
                no_elements_arr++;
                if(list_elements[0]!='{')
                {
                        strcat(list_elements,"{");
                }
                strcat(list_elements,convertor_int);
                strcat(list_elements," ");
           }
        ;

//functii
functii : functie '{''}'
        | functii functie '{''}'
        ;
functie : TIP ID '('')' {strcpy(parameters1," void");if(newFunction($2,$1,parameters1,parameters2)==-1) return -1;}
        | TIP ID '(' parametrii ')' {if(newFunction($2,$1,parameters1,parameters2)==-1) return -1;} 
        ;
parametrii: TIP ID {def();if(checkPar($2)!=-1) addParameters(parameters1,parameters2,$1,$2);else return -1;}
          | parametrii ',' TIP ID {if(checkPar($4)!=-1) addParameters(parameters1,parameters2,$3,$4); else return -1;}
          ;


//clase
classes : class 
        | classes class
        ;
class : CLASS ID '{' info_class '}' 
      ;
info_class : PRIVATE ':' class_content PUBLIC ':' class_content 
           ;
class_content : class_content declaration 
              | declaration  
              ;
declaration : TIP ID ';'
            ;
//main 


main: BGIN declaratii continut evals END
    | BGIN continut evals END
    ;
continut : statements 
         | statements continut
         | asignari ';' continut
         | asignari ';'
         ;
evals : eval ';' 
      | type ';'
      | evals eval ';'
      | evals type ';'
      ;
eval : EVAL '(' exp_arit ')'{
        if(elType("eval_checker")==-1) return -1;
        int l=evalAST($3);
        freeAST($3);
        printf("Valoare Eval: %d\n",l);
}
type : TYPE_OF '(' STRING ')' { printf("Type of '%s' is string!\n",$3);}
     | TYPE_OF '(' CARACTER ')' { printf("Type of '%s' is caracter!\n",$3);}
     | TYPE_OF '(' exp_arit ')' {
        //if(elType("eval_checker")==-1) return -1;
        printf("Type of expression from line %d is %s!\n",yylineno,type_of_global);
     }
     ;
statements : if_stat
           | while_stat
           | for_stat
           | ID '('')'
           ;
if_stat : IF '(' exp_bool ')' '{' continut '}'
        | IF '(' exp_bool ')' '{' continut '}' ELSE '{' continut '}'
        | IF '(' exp_bool ')' '{' continut '}' ELSE IF '(' exp_bool ')''{' continut '}'
while_stat : WHILE '(' exp_bool ')' '{' continut '}'
           ;
for_stat : FOR '(' asignari ';' exp_bool ';' asignari ')''{'continut'}'
         ;

asignari: /* ID ASSIGN ID
        | ID ASSIGN NR_INT
        | ID ASSIGN NR_FLOAT
        | ID'['NR_INT']' ASSIGN NR_INT */
        | ID ASSIGN CARACTER{
                addElements("char","no");
                if(elType($1)==-1) 
                        return -1;
                int k = insertChar($1,$3);
                if(k==0)
                {
                        printf("Variabila nu este de tip CHAR! Linia : %d\n",yylineno);
                        return 0;
                }
                /*if(check_char_string($1,1) == -1)
                {
                        printf("Valorile din expresie nu au acelasi tip! Linia: %d\n",yylineno);
                        return 0;
                }*/
        }
        | ID ASSIGN STRING{
                addElements("string","no");
                if(elType($1)==-1) return -1;
                int k = insertString($1,$3);
                if(k==1){
                        printf("Variabila nu a fost declarata! Linia: %d\n",yylineno);
                        return 0;
                }
                else if(k==0){
                        printf("Variabila trebuie sa fie declarata de tip STRING! Linie: %d\n",yylineno);
                        return 0;
                }
                //printf("aj aici!\n");
                /*if(check_char_string($1,2) == -1)
                {
                        printf("Valorile din expresie nu au acelasi tip! Linia: %d\n",yylineno);
                        return 0;
                }*/
        }
        | ID ASSIGN exp_arit {
                if(Check_declared($1)==0)
                {
                        printf("Variabilia %s nu a fost declarata! Linia: %d\n",$1,yylineno);
                        return 0;
                } else if(elType($1)==-1) return -1;
                if(check_int($1)==1) {int l=evalAST($3);
                                      //printf("%d\n",l);
                                      insertInt($1,l);
                                      freeAST($3);}
        }
        //| ID ASSIGN exp_bool {if(strcmp($1,"bool")!=0) {printf("Error!The variable is not of type BOOL!\n");return -1;}}
        | ID '['NR_INT']' ASSIGN exp_arit{
                char name_check[100];
                //char convertor_int[30];
                //sprintf(convertor_int, "%d",$3);
                strcpy(name_check,$1); 
                strcat(name_check,"[");
                int lungime = strlen(name_check);
                //strcpy(name_check,convertor_int);
                int k=Check_declared_arr(name_check,lungime,$3);
                if(k==-1){
                        printf("Variabila %s[%d] nu a fost declarata! Linia: %d\n",$1,$3,yylineno);
                        return 0;
                }
                else if(k==-2){
                        printf("Out of scope! Nu putem insera pe pozitia ceruta! Vectorul este mai scurt! Linia: %d\n",yylineno);
                        return 0;
                } else if(elType(name_check)==-1) return -1;
                if(variables[k].int_type==1){
                   // printf("VAR: %s\n",variables[k].nume);
                    int l=insert_value_var(name_check,$3,evalAST($6));
                    //printf("Numar: %d\n",l);
                    freeAST($6);
                }
        }
        ;
exp_arit : exp_arit'+'exp_arit {struct AST* root=(struct AST*)malloc(sizeof(struct AST));
                                root->data.des=(char*)malloc(sizeof(char)*strlen("+"));
                                strcpy(root->data.des,"+");
                                $$=(struct AST*)buildAST((struct AST*)root,(struct AST*)$1,(struct AST*)$3,"OP");}
         | exp_arit'-'exp_arit {struct AST* root=(struct AST*)malloc(sizeof(struct AST));
                                root->data.des=(char*)malloc(sizeof(char)*strlen("-"));
                                strcpy(root->data.des,"-");
                                $$=(struct AST*)buildAST((struct AST*)root,(struct AST*)$1,(struct AST*)$3,"OP");}
         | exp_arit'*'exp_arit {struct AST* root=(struct AST*)malloc(sizeof(struct AST));
                                root->data.des=(char*)malloc(sizeof(char)*strlen("*"));
                                strcpy(root->data.des,"*");
                                $$=(struct AST*)buildAST((struct AST*)root,(struct AST*)$1,(struct AST*)$3,"OP");}
         | exp_arit'/'exp_arit {struct AST* root=(struct AST*)malloc(sizeof(struct AST));
                                root->data.des=(char*)malloc(sizeof(char)*strlen("/"));
                                strcpy(root->data.des,"/");
                                $$=(struct AST*)buildAST((struct AST*)root,(struct AST*)$1,(struct AST*)$3,"OP");}
         | '('exp_arit')' {$$=$2;}
         | '-'exp_arit %prec UMINUS {struct AST* root=(struct AST*)malloc(sizeof(struct AST));
                                     root->data.des=(char*)malloc(sizeof(char)*strlen("neg"));
                                     strcpy(root->data.des,"neg");
                                     $$=(struct AST*)buildAST((struct AST*)root,(struct AST*)$2,NULL,"OP");}
         | valoare
         ;
valoare : NR_INT {
                strcpy(type_of,"int");
                strcpy(type_of_global,type_of);
                if(tester == 1) addElements("int","no");
                struct AST* root=(struct AST*)malloc(sizeof(struct AST));
                root->data.val=$1;
                root->data.des=(char*)malloc(sizeof(char)*strlen(" "));
                root->data.des=" ";
                $$=(struct AST*)buildAST((struct AST*)root,NULL,NULL,"NUMBER");
                }
        | NR_FLOAT {
                strcpy(type_of,"float");
                strcpy(type_of_global,type_of);
                if(tester == 1) addElements("float","no");
                struct AST* root=(struct AST*)malloc(sizeof(struct AST));
                root->data.des=(char*)malloc(sizeof(char)*strlen("operand"));
                strcpy(root->data.des,"operand");
                $$=(struct AST*)buildAST((struct AST*)root,NULL,NULL,"OTHER");
                }
        | ID { 
               if(Check_declared($1)==0)
                {
                        printf("Variabilia '%s' nu a fost declarata! Linia: %d\n",$1,yylineno);
                        return 0;
                }
                if(tester == 1) {addElements("id",$1);/*printf("Adaugat %s\n",$1);*/}
                TypeOf(type_of,$1);
                strcpy(type_of_global,type_of);
                struct AST* root=(struct AST*)malloc(sizeof(struct AST));
                root->data.des=(char*)malloc(sizeof(char)*strlen($1));
                strcpy(root->data.des,$1);
                $$=(struct AST*)buildAST((struct AST*)root,NULL,NULL,"IDENTIFIER");
        }
        | ID '['NR_INT']' {
                char name_check[100];
                strcpy(type_of_global,"int");
                strcpy(name_check,$1);
                strcat(name_check,"[");
                int lungime = strlen(name_check);
                int k=Check_declared_arr(name_check,lungime,$3);
                if(k==-1){
                        printf("Variabila '%s[%d]' nu a fost declarata! Linia: %d\n",$1,$3,yylineno);
                        return 0;
                }
                else if(k==-2){
                        printf("Out of scope! Nu putem insera de pe pozitia ceruta! Vectorul este mai scurt! Linia: %d\n",yylineno);
                        return 0;
                }
                if(tester == 1) addElements("id",name_check);
                struct AST* root=(struct AST*)malloc(sizeof(struct AST));
                sprintf(num,"%d",$3);
                strcpy(copy,$1);strcat(copy,"[");strcat(copy,num);strcat(copy,"]");
                root->data.des=(char*)malloc(sizeof(char)*strlen(copy));
                strcpy(root->data.des,copy);
                $$=(struct AST*)buildAST((struct AST*)root,NULL,NULL,"IDENTIFIER");
        }
        | ID '('')' {
                if(Check_declared_func($1)==0){
                        printf("Functie nedeclarata inainte!\n");
                        return 0;
                }else{
                        int k = Check_params($1," void"); 
                        //printf("k=%d\n",k);
                        if(k ==0){
                                printf("Functia are nevoie de parametrii! Linia: %d!\n",yylineno);
                                return 0;
                        }
                }
                TypeOfFunc(type_of,$1);           
                strcpy(type_of_global,type_of);

                if(tester == 1) addElements("function",$1);
                struct AST* root=(struct AST*)malloc(sizeof(struct AST));
                strcpy(copy,$1);strcat(copy,"(");strcat(copy,")");
                root->data.des=(char*)malloc(sizeof(char)*strlen(copy));
                strcpy(root->data.des,copy);
                $$=(struct AST*)buildAST((struct AST*)root,NULL,NULL,"IDENTIFIER");
        }
        | ID '(' valori ')' {
                if(Check_declared_func($1)==0){
                        printf("Functie nedeclarata inainte!\n");
                        return 0;
                }else{
                        params_call[strlen(params_call)-2]='\0';
                        if(Check_params($1,params_call)==0)
                        {
                                printf("Parametrii gresiti! Linie : %d\n",yylineno);
                                return 0;
                        }
                }
        TypeOfFunc(type_of,$1);
        strcpy(type_of_global,type_of);

        //if(tester == 1)
        addElements("function",$1);
        //printf("Adaugat %s\n",$1);
        bzero(params_call,100);
        tester = 1;
        struct AST* root=(struct AST*)malloc(sizeof(struct AST));
        strcpy(copy,$1);strcat(copy,"(");strcat(copy,")");
        root->data.des=(char*)malloc(sizeof(char)*strlen(copy));
        strcpy(root->data.des,copy);
        $$=(struct AST*)buildAST((struct AST*)root,NULL,NULL,"IDENTIFIER");
        }
        ;
valori: valoare { 
                if(tester == 1)
                {
                        strcpy(params_call," ");
                        strcat(params_call,type_of);
                        strcat(params_call,", ");
                        tester = 0;
                }
                else{
                        strcat(params_call,type_of);
                        strcat(params_call,", ");

                }
        }
      | valori ',' valoare { 
                if(tester == 1)
                {
                        strcpy(params_call,type_of);
                        strcat(params_call,", ");
                        tester = 0;
                }
                else{
                        strcat(params_call,type_of);
                        strcat(params_call,", ");

                }

        }
      ;
exp_bool : exp_bool AND exp_bool 
         | exp_bool OR exp_bool 
         | '['exp_bool']' 
         |'!''['exp_bool']' 
         | exp_arit COMPARE exp_arit 
         ;

%%

int yyerror(char * s){
printf("eroare: %s la linia:%d\n",s,yylineno);
return 0;
}

int main(int argc, char** argv){
yyin=fopen(argv[1],"r");
insert_variables("int","eval_checker",0,0);
insertInt("eval_checker",1);

yyparse();
function_table();
} 