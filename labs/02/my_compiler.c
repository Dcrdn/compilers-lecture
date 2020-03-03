#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 

char stackP[1000],stackC[1000];
int posP[1000],posC[1000];
int top1=-1;
int top2=-1;
int posComilla=-1;
bool errors=false;
bool longComment=false;
bool comilla=false;

void checkLine(char *line, int size, int nline){
    for(int i=0;i<size;i++){
        //comentarios
        if(line[i]=='/' && line[i+1]=='/'){
            break;
        }else if(line[i]=='/' && line[i+1]=='*'){
            longComment=true;
            continue;
        }else if(line[i]=='*' && line[i+1]=='/'){
            longComment=false;
            continue;
        }
        if(longComment){
            continue;
        }
        //comillas
        if(line[i]=='"' ||line[i]=='\'' ){
            if(comilla){
                comilla=false;
                posComilla=-1;
            }else{
                comilla=true;
                posComilla=nline;
            }
        }
        if(comilla){
            continue;
        }

        //parentesis, brackets y corchetes
        if(line[i]=='('){
            stackP[++top1]='(';
            posP[top1]=nline;
        }else if(line[i]==')' && stackP[top1]=='('){
            top1--;
        }else if(line[i]==')' && top1==-1){
            printf("Extra closing parentesis ) at line %d\n", nline);
            errors=true;
        }else if(line[i]=='{'){
            stackC[++top2]='{';
            posC[top2]=nline;
        }else if(line[i]=='}' && stackC[top2]=='{'){
            top2--;
        }else if(line[i]=='}' && top2==-1){
            printf("Extra closing corchete  } at line %d\n", nline);
            errors=true;
        }
    }
}

void readFile(char *nameFile){
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    fp = fopen(nameFile, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    int nline=1;
    while ((read = getline(&line, &len, fp)) != -1) {
        checkLine(line, strlen(line), nline);
        nline++;
    }   
    if(posComilla!=-1){
        errors=true;
        printf("Wrong comilla at line: %d\n", posComilla);
    }
    for(int i=0;i<=top1;i++){
        if(i>posComilla && posComilla!=-1)
            break;
        errors=true;
        if(posP[i]==0){
            continue;
        }
        printf("Unclosed parentesis at line: %d\n", posP[i]);
    }
    for(int i=0;i<=top2;i++){
        if(i>posComilla && posComilla!=-1)
            break;
        errors=true;
        if(posC[i]==0){
            continue;
        }
        printf("Unclosed brace at line: %d\n", posC[i]);
    }
    if(!errors){
        printf("There is no errors\n");
    }
    fclose(fp);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv){
    char *file=argv[1];
    readFile(file);
}