#include <stdio.h>

int mystrlen(char *);
char *mystradd(char *, char *);
int mystrfind(char *, char *);
int mystrcmp(char *, char *);

int main(int argc, char **argv) {
    if (argc < 4){
        printf("Error. Missing arguments\n");
        printf("Pleas run as \n./main -add <string 1> <string 2>\nor\n./main -find <string 1> <string 2>\n");
        return -1;
    }
    if (mystrcmp(argv[1],"-add")==0){
        char *str = mystradd(argv[2],argv[3]);
        printf("Initial Lenght      : %d\nNew String          : %s\nNew length          : %d\n",mystrlen(argv[2]),str,mystrlen(str));
    }else if (mystrcmp(argv[1],"-find")==0){
        int pos = mystrfind(argv[2],argv[3]);
        if(pos!=-1){
            printf("['%s'] string was found at [%d] position\n",argv[3],pos);
        }else{
            printf("['%s'] string was not found at ['%s'] string\n",argv[3],argv[2]);
            return -1;
        }
    }else{
        printf("%s is not a valid functionality\n",argv[1]);
        return -1;
    }
    return 0;
}
