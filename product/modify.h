#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trimLeft(char s[]){
  int i=0, j;
  while(s[0]==' '){
    for(j=0;j<strlen(s)-1;j++){
      s[j]=s[j+1];
    }
    s[strlen(s)-1]='\0';
  }
}

void trimRight(char s[]){
  int i=strlen(s)-1;
  while(s[i]==' '){
    s[i--]='\0';
  }
}

void trimMiddle(char s[]){
  int i=0,j;
  while(s[i]==' ') i++;
  for(i;i<strlen(s);i++){
    if((s[i]==' ')&&(s[i+1]==' ')){
  for(j=i;j<strlen(s)-1;j++){
    s[j]=s[j+1];
  }
  s[strlen(s)-1]='\0';
  i--;
      }
  }
}

// void toLowcase(char s[]){
//   for(int i=0;i<strlen(s);i++){
//     tolower(s[i]);
//   }
// }

void trim(char s[]){
  // toLowcase(s);
  trimLeft(s);
  trimRight(s);
  trimMiddle(s);
}

int split(char *s, char c, char **substr){
    int i = 0, n = 0, j;
    char *ss = malloc(strlen(s));

    while(i < strlen(s)){
        
        j = 0;

        for(i; i < strlen(s); i++){
            if(s[i] == c) break;
            ss[j++] = s[i];
        }
        ss[j] = '\0';
        trim(ss);
        
        substr[n++] = strdup(ss);

        i++;
    }

    free(ss);
    return n;
}

int getFileLine(FILE *fptr){
    char c;
    int n = 0;
    c = fgetc(fptr);
    while(c != EOF) {
        if(c=='\n') n++;
        c = fgetc(fptr);
    }
    fseek(fptr,0,SEEK_SET);
    return n;
}

int getNum(char *s){
    int num = 0;
    for(int i=0; i < strlen(s); i++){
        num = num*10 + s[i] - '0';
    }
    return num;
}
