#include <stdlib.h>

int mystrlen(char *str){
    int c=0;
    int act=0;
    while (str[act]!=0){
        c++;
        act++;
    }
    return c;
}

char *mystradd(char *origin, char *addition){
    int lenOr = mystrlen(origin);
    int lenAdd = mystrlen(addition);
    char *newString = calloc(lenAdd+lenOr,sizeof(char));
    int act=0;
    while (act<lenOr){
        newString[act] = origin[act];
        act++;
    }
    int actAdd = 0;
    while (actAdd<lenAdd){
        newString[act]=addition[actAdd];
        actAdd++;
        act++;
    }
    return newString;
}

int mystrfind(char *origin, char *substr){
    int lensub = mystrlen(substr);
    int lentocheck = mystrlen(origin)-lensub;
    int act = 0;
    while (act<lentocheck){
        if (substr[0]==origin[act])
        {
            int dif=0;//0 false 1 true
            int j=act+1;
            for (int i = 1; i < lensub; i++)
            {
                if(substr[i]!=origin[j]){
                    i=lensub;
                    dif = 1;
                }else{
                    j++;
                }
                
            }
            if(dif==0){
                return act;
            }
        }
        act++;
    }
    return -1;
}

int mystrcmp(char *string1, char *string2){
    int len1=mystrlen(string1);
    if (mystrlen(string2)!=len1){
        return 1;
    }
    for (int i = 0; i < len1; i++){
        if (string1[i]!=string2[i]){
            return 1;
        }
    }
    return 0;
}
