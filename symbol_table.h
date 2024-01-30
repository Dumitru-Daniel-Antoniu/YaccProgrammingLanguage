#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "symbol_table_functions.h"

extern FILE* yyin;
extern char* yytext;
extern int yylineno; 

union val{
    int integer;
    float floating;
    bool boolean;
    char string[200];
    char character;
};

struct variabila{
    char nume[50];
    char type[20];
    union val valoare;
    //union type_of_var ident;
    int constant; // 1 constant 0 not constant
    int array; // 1 array 0 not array

    int init;// 1 initialized 0 otherwise
    int int_type;
    int float_type;
    int string_type;
    int bool_type;
    int char_type;
}variables[250];

int count = 0;

struct elements{
    struct number{
        int i;//int
        int f;//float
        int c;//char
        int s;//string
        int var;//variable
        int fun;//function
        char name[50];//the name(variable or function)
    }y[100];
}x[100];
struct AST {
    struct data{
        int val;
        char* des;
    }data;
    char* type;
    struct AST* left;
    struct AST* right;
};

int nrX=1;
int nrY=1;

void insert_variables(char * type, char * name, int var_c, int array_test){
    strcpy(variables[count].nume,name);
    strcpy(variables[count].type,type);
    variables[count].array = array_test;
    if(strcmp(type,"char")==0 || strcmp(type,"const char")==0){
        variables[count].char_type = 1;
    }else if(strcmp(type,"string")==0 || strcmp(type,"const string")==0){
                variables[count].string_type = 1;
    }else if(strcmp(type,"int")==0 || strcmp(type,"const int")==0){
        variables[count].int_type = 1;
    }else if(strcmp(type,"float")==0 || strcmp(type,"const float")==0){
        variables[count].float_type = 1;
    }else if(strcmp(type,"bool")==0 || strcmp(type,"const bool")==0){
        variables[count].bool_type = 1;
    }
    variables[count].constant = var_c;
    count++;
}
int give_number(char * var_name){
    int i;
    for(i=0; i < count; i++)
    {
        if(strcmp(var_name,variables[i].nume)==0)
            return i;
    }
    return -2;
}
void print_var(){
    for(int i=0; i<count; i++)
    {
        printf("Nume : %s , tip: %s , ",variables[i].nume,variables[i].type);
        printf(" valoare: ");
        if(variables[i].int_type == 1 && variables[i].array == 1)
        {
            printf("%s\n",variables[i].valoare.string);

        }else if(variables[i].int_type == 1)
        {
            printf("%d\n",variables[i].valoare.integer);
        }else if(variables[i].float_type == 1)
        {
            printf("%f\n",variables[i].valoare.floating);
        }else if(variables[i].char_type == 1)
        {
            printf("%c\n",variables[i].valoare.character);
        }else if(variables[i].string_type == 1)
        {
            printf("%s\n",variables[i].valoare.string);
        }

    }
}
int Check_declared(char * var_name){
    int i;
    for(i=0; i < count; i++)
    {
        if(strcmp(var_name,variables[i].nume)==0)
            return 1;
    }
    return 0;
}
int Check_declared_arr(char * arr_name,int char_leng, int length){
    //char convertor_int[30];
    //sprintf(convertor_int, "%d",length);
    for(int i = 0; i< count; i++)
    {
        if(variables[i].array == 1)
        {
            int ok=1;
            int k=0;
            for(int j=0; j<char_leng && ok == 1;j++){
                if(arr_name[k] != variables[i].nume[j]){
                    ok=0;
                }
                k++;
            }
            if(ok==1){
                int convertor_char=0;
                char nr;
                int cifra;
                k=char_leng;          
                while(variables[i].nume[k]!=']'){
                    nr=variables[i].nume[k];
                    cifra=(int)nr-'0';
                    convertor_char=convertor_char*10+cifra;
                    k++;
                }

                if(convertor_char < length){
                    return -2;
                }
                return i;
            }
        }
    }
    return -1;
}
int insertChar(char * var_name, char *char_value){

    for(int i = 0; i < count; i++)
        {
            if(strcmp(var_name, variables[i].nume)==0){
                if(variables[i].char_type == 1){
                    variables[i].valoare.character=char_value[1];
                    variables[i].init=1;
                    return 1; //succes!
                }
                else return 0;

            }
        }
    return 2;
}
int insertString(char * var_name, char * string_value){
    for(int i=0; i<count; i++){
        if(strcmp(var_name,variables[i].nume)==0){
            if(variables[i].string_type == 1 || variables[i].array==1){
                strcpy(variables[i].valoare.string,string_value);
                variables[i].init=1;
                return 2; // succes
            }
            else return 0;
        }
    }
    return 1;
}
int insertInt(char * var_name, int int_value){
    //printf("Am aj aici cu %s\n",var_name);
    for(int i=0; i<count; i++){
        if(strcmp(variables[i].nume,var_name)==0){
            if(variables[i].int_type == 1){
                variables[i].valoare.integer = int_value;
                variables[i].init=1;
                return 2; //succes
            }
            else
                return 0; // val de alt tip
        }
    }
    return 1; //variabila nu exista
}
int insertFloat(char * var_name, float float_value){
    for(int i=0; i<count; i++){
            if(strcmp(variables[i].nume,var_name)==0){
                if(variables[i].float_type == 1){
                    variables[i].valoare.floating = float_value;
                    variables[i].init=1;
                    return 2; //succes
                }
                else
                    return 0;
            }
        }
        return 1;
}
void write_file()
{
    FILE * fp;
    fp=fopen("symbol_table.txt","w");
    if(fp == NULL)
     {
          printf("Error!");   
          exit(1);             
     }
     for(int i=0; i<count; i++)
    {
        fprintf(fp,"Nume : %s , tip: %s , ",variables[i].nume,variables[i].type);
        fprintf(fp," valoare: ");
        if(variables[i].int_type == 1 && variables[i].array == 1)
        {
            fprintf(fp,"%s\n",variables[i].valoare.string);

        }else if(variables[i].int_type == 1)
        {
            fprintf(fp,"%d\n",variables[i].valoare.integer);
        }else if(variables[i].float_type == 1)
        {
            fprintf(fp,"%f\n",variables[i].valoare.floating);
        }else if(variables[i].char_type == 1)
        {
            fprintf(fp,"%c\n",variables[i].valoare.character);
        }else if(variables[i].string_type == 1)
        {
            fprintf(fp,"%s\n",variables[i].valoare.string);
        }

    }
}
void TypeOf(char * type_of,char * var_name){
    int i;
    i = give_number(var_name);
    strcpy(type_of,variables[i].type);
}
void addElements(char* el,char* name)
{
    if(strcmp(el,"int")==0)
    {
        x[nrX].y[nrY].i=1;
        nrY++;
    }
    if(strcmp(el,"float")==0)
    {
        x[nrX].y[nrY].f=1;
        nrY++;
    }
    if(strcmp(el,"char")==0)
    {
        x[nrX].y[nrY].c=1;
        nrY++;
    }
    if(strcmp(el,"string")==0)
    {
        x[nrX].y[nrY].s=1;
        nrY++;
    }
    if(strcmp(el,"id")==0)
    {
        x[nrX].y[nrY].var=1;
        strcpy(x[nrX].y[nrY].name,name);
        nrY++;
    }
    if(strcmp(el,"function")==0)
    {
        x[nrX].y[nrY].fun=1;
        strcpy(x[nrX].y[nrY].name,name);
        nrY++;
    }
}
int Check_compatibility(int i, int j){
    if(strstr(variables[j].type,variables[i].type)!=NULL){
        return 1;
    }else return -1;
}
int elType(char* name)
{
    int i,j,number=0,in,in2;
    nrY=nrY-1;
    char n[1000]; //verifica tipul fiecarui element al expresiei din dreapta
    if(nrY>1) //pentru expresii aritmetice
    {
        for(i=1;i<=nrY;i++)//parcurge toate elementele
        {
            if(x[nrX].y[i].i==1) n[number]='1';//int 
            if(x[nrX].y[i].f==1) n[number]='2';//float
            if(x[nrX].y[i].var==1)
            {
                if(x[nrX].y[i+1].fun==1)
                {
                    in=give_func(x[nrX].y[i+1].name);
                    if(strcmp(vf[in].parameters," void")!=0) i++;
                }
                else if(Check_declared(x[nrX].y[i].name)==1)
                {
                    in=give_number(x[nrX].y[i].name);
                    if(variables[in].int_type==1) n[number]='1';
                    if(variables[in].float_type==1) n[number]='2';
                    if(variables[in].bool_type==1) n[number]='3';
                    if(variables[in].char_type==1||variables[in].string_type==1)
                    {
                        printf("Error!You cannot add chars or strings in arithmetic operations!Line:%d\n",yylineno);
                        return -1;
                    }
                    if(variables[in].init==0)
                    {
                        printf("Error!The value %s was not initialized!Line:%d\n",x[nrX].y[i].name,yylineno);
                        return -1;
                    }
                }
                else
                {
                    //aici
                    int res = Check_declared_arr(x[nrX].y[i].name,strlen(x[nrX].y[i].name),0);
                    if(res == -1){
                        printf("Error!The variable from the right was not declared!Line:%d\n",yylineno);
                        return -1;
                    }
                    if(variables[res].int_type==1) n[number]='1';
                    if(variables[res].float_type==1) n[number]='2';
                    if(variables[res].char_type==1||variables[in].string_type==1)
                    {
                        printf("Error!You cannot add chars or strings in arithmetic operations!Line:%d\n",yylineno);
                        return -1;
                    }
                }
            }
            if(x[nrX].y[i].fun==1)
            {
                if(verifFunc(x[nrX].y[i].name)==0)
                {
                    in=give_func(x[nrX].y[i].name);
                    if(strcmp(vf[in].type,"int")==0) n[number]='1';
                    if(strcmp(vf[in].type,"float")==0) n[number]='2';
                    if(strcmp(vf[in].type,"bool")==0) n[number]='3';
                    if(strcmp(vf[in].type,"char")==0||strcmp(vf[in].type,"string")==0)
                    {
                        printf("Error!You cannot add chars or strings in arithmetic operations!Line:%d\n",yylineno);
                        return -1;
                    }
                }
                else
                {
                    printf("Error!The function %s was not declared!Line:%d\n",vf[j].name,yylineno);
                    return -1;
                }
            }
            number++;
        }
        n[number]='\0';
        //printf("START:%d\n",n[0]);
        for(i=1;i<strlen(n);i++)//verifica daca toate elementele din dreapta sunt de acelasi tip
        {
            //printf("HERE:%d\n",n[i]);
            if(n[i]!=n[0])
            {
                printf("Error!The variables in the right don't have the same type!Line:%d\n",yylineno);
                return -1;
            }
        }
        if(Check_declared(name)==1)
        {
            in=give_number(name);
            if(variables[in].constant==1)
            {
                printf("Error!You can't assign a value to a constant!Line:%d\n",yylineno);
                return -1;
            }
            if((n[0]=='1'&&strcmp(variables[in].type,"int")!=0)||(n[0]=='2'&&strcmp(variables[in].type,"float")!=0)||(n[0]=='3'&&strcmp(variables[in].type,"bool")!=0))
            {
                printf("Error!The variable from the left does not have the same type as the value from the right!Line:%d\n",yylineno);
                return -1;
            }//verificare pentru acelasi tip stanga-dreapta, 3 e bool
        }
        else
        {
           int in = Check_declared_arr(name,strlen(name),0);

            if(in == -1){
                printf("Error!The variable from the left was not declared!Line:%d\n",yylineno);
                return -1;
            }
            //printf("Var declarata! %d\n",in);
            if((n[1]=='1'&&strcmp(variables[in].type,"int")!=0)||(n[1]=='2'&&strcmp(variables[in].type,"float")!=0)||(n[1]=='3'&&strcmp(variables[in].type,"bool")!=0))
            {
                printf("Error!The variable from the left does not have the same type as the value from the right!Line:%d\n",yylineno);
                return -1;
            }
        }
    }
    else //un singur element
    {
        if(Check_declared(name)==1) //exista variabila din stanga
        {
            in=give_number(name);
            /*printf("%s %d\n",variables[in].nume,in);
            printf("%s, %d\n",variables[in].type,x[nrX].y[1].c);*/ //pentru afisare 
            if(variables[in].constant==1)
            {
                printf("Error!You can't assign a value to a constant!Line:%d\n",yylineno);
                return -1;
            }
            if(x[nrX].y[1].i==1&&strcmp(variables[in].type,"int")!=0 )
            {
                 printf("Error!The variable from the left does not have the same type as the value from the right!Line:%d\n",yylineno);
                return -1;
            }
            if(x[nrX].y[1].f==1&&strcmp(variables[in].type,"float")!=0)
            {
                printf("Error!The variable from the left does not have the same type as the value from the right!Line:%d\n",yylineno);
                return -1;
            }
            if(x[nrX].y[1].c==1&&strcmp(variables[in].type,"char")!=0)
            {
                 printf("Error!The variable from the left does not have the same type as the value from the right!Line:%d\n",yylineno);
                return -1;
            }
            if(x[nrX].y[1].s==1&&strcmp(variables[in].type,"string")!=0)
            {
                printf("Error!The variable from the left does not have the same type as the value from the right!Line:%d\n",yylineno);
                return -1;
            }
            if(x[nrX].y[1].var==1)
            {
                if(Check_declared(x[nrX].y[1].name)==1)
                {
                    in2=give_number(x[nrX].y[1].name);
                   
                    if(Check_compatibility(in,in2)==-1){
                        printf("Error!The variables are not of same type!Line:%d\n",yylineno);
                        return -1;
                    }

                    if(variables[in].init==0)
                    {
                        printf("Error!The value %s was not initialized!Line:%d\n",x[nrX].y[1].name,yylineno);
                        return -1;
                    }
                    return 0;
                }
                else
                {
                    int res = Check_declared_arr(x[nrX].y[1].name,strlen(x[nrX].y[1].name),0);
                    if(res == -1){
                        printf("Error!The variable from the right was not declared!Line:%d\n",yylineno);
                        return -1;
                    }
                    if(strcmp(variables[in].type,variables[res].type)!=0)
                    {
                        printf("Error!The variables are not of same type!Line:%d\n",yylineno);
                        return -1;
                    }
                }
            }
            if(x[nrX].y[1].fun==1)
            {
                if(verifFunc(x[nrX].y[1].name)==0)
                {
                    in2=give_func(x[nrX].y[1].name);
                    if(strcmp(vf[in2].type,variables[in].type)!=0)
                    {
                        printf("Error!The variables are not of same type!Line:%d\n",yylineno);
                        return -1;
                    }
                    return 0;
                }
                else
                {
                    printf("Error!The function from the right was not declared!Line:%d\n",yylineno);
                    return -1;
                }
            }
        }
        else 
        {
            int res2 = Check_declared_arr(name,strlen(name),0);

            if(res2 == -1){
                printf("Error!The variable from the left was not declared!Line:%d\n",yylineno);
                return -1;
            }
            if(x[nrX].y[1].i==1&&strcmp(variables[res2].type,"int")!=0 )
            {
                 printf("Error!The variable from the left does not have the same type as the value from the right!Line:%d\n",yylineno);
                return -1;
            }
            if(x[nrX].y[1].f==1&&strcmp(variables[res2].type,"float")!=0)
            {
                printf("Error!The variable from the left does not have the same type as the value from the right!Line:%d\n",yylineno);
                return -1;
            }
            if(x[nrX].y[1].c==1&&strcmp(variables[res2].type,"char")!=0)
            {
                 printf("Error!The variable from the left does not have the same type as the value from the right!Line:%d\n",yylineno);
                return -1;
            }
            if(x[nrX].y[1].s==1&&strcmp(variables[res2].type,"string")!=0)
            {
                printf("Error!The variable from the left does not have the same type as the value from the right!Line:%d\n",yylineno);
                return -1;
            }
            if(x[nrX].y[1].var==1)
            {
                if(Check_declared(x[nrX].y[1].name)==1)
                {
                    in2=give_number(x[nrX].y[1].name);
                    //aici
                    /*if(strcmp(variables[in].type,variables[in2].type)!=0)
                    {
                        printf("Error!The variables are not of same type!Line:%d\n",yylineno);
                        return -1;
                    }*/
                    if(Check_compatibility(res2,in2)==-1){
                        printf("Error!The variables are not of same type!Line:%d\n",yylineno);
                        return -1;
                    }
                    return 0;
                }
                else
                {
                    int res = Check_declared_arr(x[nrX].y[1].name,strlen(x[nrX].y[1].name),0);
                    if(res == -1){
                        printf("Error!The variable from the right was not declared!Line:%d\n",yylineno);
                        return -1;
                    }
                    if(strcmp(variables[res2].type,variables[res].type)!=0)
                    {
                        printf("Error!The variables are not of same type!Line:%d\n",yylineno);
                        return -1;
                    }
                }
            }
            if(x[nrX].y[1].fun==1)
            {
                if(verifFunc(x[nrX].y[1].name)==0)
                {
                    in2=give_func(x[nrX].y[1].name);
                    if(strcmp(vf[in2].type,variables[res2].type)!=0)
                    {
                        printf("Error!The variables are not of same type!Line:%d\n",yylineno);
                        return -1;
                    }
                    return 0;
                }
                else
                {
                    printf("Error!The function from the right was not declared!Line:%d\n",yylineno);
                    return -1;
                }
            }

        }
    }
    nrX=nrX+1;
    nrY=1;
    return 0;
}
int check_char_string(char * var_name, int type){
    int var;
    var = give_number(var_name);

    if(type == 1){//char
        if(strcmp(variables[var].type,"char") != 0){
            return -1;
        }
    }else if(type == 2){
         if(strcmp(variables[var].type,"string") != 0){
            return -1;
        }
    }
}
int get_value(char * name){
    char * number;

    if((number=strstr(name,"["))!=NULL) // this is an array
    {
        //printf("Vector! %s\n",name);
        int convert=0;
        int value=0;
        int counter=0;
        int i;
        int cifra;
        char * result;
        char parcurge[250];
        char temp[250];
        int start,stop;
        number = number + 1;
        number[strlen(number)-1]='\0';

        for(i=0; i<strlen(number); i++){
            cifra = (int)number[i]-'0';
            convert = convert * 10 + cifra;
        }
        //printf("convert : %d\n",convert);
        name[strlen(name)-1-strlen(number)]='\0';
        int k = Check_declared_arr(name,strlen(name),convert);
        if(k == -1)
        {
            printf("Variabila '%s]' nu a fost declarata! Linia: %d\n",name,yylineno);
            return -1;
        }else if(k==-2){
            printf("Out of scope! Nu putem afisa de pe pozitia ceruta! Vectorul este mai scurt! Linia: %d\n",yylineno);
            return -1;
        }
        else{
            //printf("val: %s\n", variables[k].valoare.string);
            result = (char *)malloc(strlen(variables[k].valoare.string));
            result = variables[k].valoare.string;
            result[strlen(result)]='\0';
            result++;
            start=0;
            i=0;
            bzero(temp,250);
            while(result[i]!='\0'){

                if(counter-1 == convert){
                    if(temp[0]=='n'){
                    return 0;
                    }
                    for(int j=0; j<stop; j++)
                        {
                            cifra =(int)temp[j]-'0';
                            value = value*10+cifra;
                            
                        }
                    
                        return value;
                } else
                if(result[i]==' '){
                    bzero(temp,250);

                    strcpy(temp,parcurge);
                    bzero(parcurge,250);
                    stop=start;
                    start=0;
                    counter++;
                }else
                {parcurge[start]=result[i];
                start++;}
                i++;
            }
            
        }
        printf("Nu puteti afisa de pe aceasta pozitie intrucat nu se afla nimic in ea! Linie: %d\n",yylineno);
        return -1;
    }else if(strstr(name,"(")!= NULL){
        //printf("Functie\n");
        return -1;
    }
    else{
        int id_var = give_number(name);
        return variables[id_var].valoare.integer;
    }
}
int insert_value_var(char * var_name, int pozitie, int expr_value){
    char * number_poz;
    int spaces[100];
    char temp[250];
    char temp1[250];
    int poz;
    bzero(temp,250);
    char convertor_int[30];
    int first,last;
    int counter;
    int j=0;
    poz = Check_declared_arr(var_name,strlen(var_name),pozitie);
    if(poz == -1){
        printf("Vector nedeclarat! Linie: %d\n",yylineno);
        return -1;
    }
    else if(poz == -2){
        printf("Out of scope! Pozitia pe care incercati sa inserati nu exista! Vectorul este mai mic! Linie: %d\n",yylineno);
        return -1;
    }else{
        number_poz = strstr(variables[poz].valoare.string,"{");
        number_poz++;
        first = 0;
        spaces[0]=0;
        j++;
        for(int i = 0; i<strlen(number_poz);i++){
            if(number_poz[i]==' '){
                spaces[j]=i;
                j++;
            }
        }
        if(pozitie<j-1){
            for(int y=0; y<spaces[pozitie]; y++){
                temp[y]=number_poz[y];
            }
            if(pozitie != 0)
                strcat(temp," ");
            sprintf(convertor_int, "%d",expr_value);   
            strcat(temp,convertor_int);
            number_poz=number_poz+spaces[pozitie+1];
            strcat(temp,number_poz);
        
        }else{
            number_poz[strlen(number_poz)-1]='\0';
            strcpy(temp,number_poz);
            for(int y=0; y<pozitie-j+1; y++){
                strcat(temp,"n ");
            }
            sprintf(convertor_int, "%d",expr_value);   
            strcat(temp,convertor_int);
            strcat(temp," ");
            strcat(temp,"}");
        }
        strcpy(temp1,"{");
        strcat(temp1,temp);
        strcpy(variables[poz].valoare.string,temp1);
    }
    
}
struct AST* buildAST(struct AST* root, struct AST* left, struct AST* right, char* type)
{
    //type=(char*)malloc(sizeof(char)*strlen(type));
    root->type=type;
    root->left=left;
    root->right=right;
    return (root);
}
int check_int(char* name)
{
    int in=give_number(name);
    if(variables[in].int_type==1) return 1;
    return 0;
}
int evalAST(struct AST* root)
{
    int k;
    if(strcmp(root->type,"OP")==0)
    {
        if(strcmp(root->data.des,"neg")==0) return evalAST(root->left)*(-1);
        else if(strcmp(root->data.des,"+")==0) return evalAST(root->left)+evalAST(root->right);
             else if(strcmp(root->data.des,"-")==0) return evalAST(root->left)-evalAST(root->right);
                  else if(strcmp(root->data.des,"*")==0) return evalAST(root->left)*evalAST(root->right);
                       else if(strcmp(root->data.des,"/")==0) return evalAST(root->left)/evalAST(root->right);
    }
    else
    {
        if(strcmp(root->type,"NUMBER")==0) return root->data.val;
        else
        {
            k=get_value(root->data.des);
            if(k==-1) {printf("Warning: Vom considera una dintre valori ca fiind 0.Linie: %d\n",yylineno);return 0;}
            else return k;
        }
    }
}
void freeAST(struct AST* root)
{
    //if(root->data.des!=" ") printf("%s\n",root->data.des);
    //else printf("%d\n",root->data.val);
    if(root->left!=NULL) freeAST(root->left);
    if(root->right!=NULL) freeAST(root->right);
    free(root);
}