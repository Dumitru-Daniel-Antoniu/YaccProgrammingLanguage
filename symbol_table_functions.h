#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
extern int yylineno;
struct functions
{   
    char name[50];
    char type[50];
    char parameters[500];
    char names[50];
    char pnames[50][50];
    int nrPar;
}vf[100];

int nr=1;

void addParameters(char* parameters1,char* parameters2,char* type,char* name)
{
    strcat(parameters1,type);
    strcat(parameters1,", ");
    strcat(parameters2,name);
    strcat(parameters2,", ");
}
int checkPar(char* name)
{
    int i;
    vf[nr].nrPar=vf[nr].nrPar+1;
    strcpy(vf[nr].pnames[vf[nr].nrPar],name);
    for(i=1;i<vf[nr].nrPar;i++)
    {
        if(strcmp(vf[nr].pnames[i],vf[nr].pnames[vf[nr].nrPar])==0)
        {
            printf("Error! The name of the parameter is already taken!\n");
            return -1;
        }
    }
    return 0;
}
void def()
{
    vf[nr].nrPar=0;
}
int newFunction(char* name,char* type,char* parameters1,char* parameters2)
{
    if(strcmp(parameters1," void")!=0) {parameters1[strlen(parameters1)-2]='\0';parameters2[strlen(parameters2)-2]='\0';}
    int i;
    for(i=1;i<nr;i++)
    {
        if(strcmp(vf[i].name,name)==0&&strcmp(vf[i].parameters,parameters1)==0)
        {
            printf("The function was already defined!\n");
            return -1;
        }
    }
    strcpy(vf[nr].name,name);
    strcpy(vf[nr].type,type);
    strcpy(vf[nr].parameters,parameters1);
    strcpy(vf[nr].names,parameters2);
    //printf("params %s %s\n",vf[nr].name,vf[nr].parameters);
    //printf("Name: %s, type: %s, parameters: %s\n",vf[nr].name,vf[nr].type,vf[nr].parameters);
    nr++;
    strcpy(parameters1," ");
    strcpy(parameters2," ");
    return 0;
}
int give_func(char* name)
{
    int i,j;
    for(i=1;i<nr;i++)
    {
        if(strcmp(vf[i].name,name)==0)
        {
            return i;
        }
    }
    return 0;
}
void function_table()
{
    FILE *f;
    int i;
    f=fopen("symbol_table_functions.txt","w");
    if(f==NULL)
    {
        printf("Error");
        exit(1);
    }
    for(i=1;i<nr;i++)
    {
        fwrite("Name: ",1,6,f);
        fwrite(vf[i].name,1,strlen(vf[i].name),f);
        fwrite(", type: ",1,8,f);
        fwrite(vf[i].type,1,strlen(vf[i].type),f);
        fwrite(", parameters:",1,13,f);
        fwrite(vf[i].parameters,1,strlen(vf[i].parameters),f);
        fwrite(", names of the parameters:",1,26,f);
        fwrite(vf[i].names,1,strlen(vf[i].names),f);
        //fwrite(", no:",1,5,f);
        //fwrite(&vf[i].nrPar,1,1,f);
        //printf("no param: %d\n",vf[i].nrPar);
        fwrite("\n",1,1,f);
    }
    fclose(f);
}
int Check_declared_func(char * func_name){

    int i;
    for(i=1; i<=nr; i++){
        if(strcmp(func_name,vf[i].name)==0){
            return 1;
        }
    }
    return 0;
}
int Check_params(char * func_name, char * params){
    int i;
    //printf("check params %s\n",params);
    i = give_func(func_name);
    if(strcmp(params,vf[i].parameters)==0){
                //printf("func %s noi %s",params,vf[i].parameters);
                return 1;
            }
    return 0;
}
int verifFunc(char* name)
{
    int i,j,ok=0;
    for(i=1;i<nr;i++)
    {
        if(strcmp(vf[i].name,name)==0)
        {
            ok=1;
            return 0;
        }
    }
    if(ok==0)
    {
        printf("Error!The function has not been defined!Line:%d\n",yylineno);
        return -1;
    }

}
void TypeOfFunc(char * type_of,char * func_name){
    int i;
    i= give_func(func_name);
    strcpy(type_of,vf[i].type);

}