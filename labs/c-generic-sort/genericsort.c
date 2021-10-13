#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void quicksort(void *lineptr[], int left, int right,
	   int (*comp)(void *, void *));

void mergesort(void *lineptr[], int left, int right,
	   int (*comp)(void *, void *));

int compInt(char *num1, char *num2){
    int n1 = atoi(num1);
    int n2 = atoi(num2);
    //printf("comp: %d %d",n1,n2);
    return n1-n2;
}

int compStrs(char *s1, char *s2){
    return strcmp(s1,s2);
}

int main(int argc, char **argv){
    if (argc<2){
        printf("Missing arguments\n");
        return -1;
    }else{
        if(strcmp(argv[1],"-n")==0){
            if (argc<6)
            {
                printf("Missing arguments\n");
                return -1;
            }
            FILE *source;
            char line[250];
            source = fopen(argv[2],"r");
            if(!source){
                printf("Error in file\n");
                return -1;
            }
            char *vals[100];
            int index=0;
            while (fgets(line,250,source)){
                char *l=calloc(250,sizeof(char)*250);
                vals[index-1]=strcat(vals[index-1],"\0");
                strcpy(l,line);
                vals[index++]=l;
            }
            //vals[index-1]=strcat(vals[index-1],"\n");
            fclose(source);
            /*
            for (int i = 0; i < 100; i++)
            {
                printf("%s\n",vals[i]);
            }
            */
            int (*comp)(void*,void*);
            comp=(int (*)(void *, void *))&compInt;
            int q;
            if(strcmp(argv[3],"-quicksort")==0){
                quicksort((void *)vals,0,99,comp);
                q=1;
            }else if(strcmp(argv[3],"-mergesort")==0){
                mergesort((void *)vals,0,99,comp);
                q=0;
            }else{
                printf("no soting algorithm provided\n");
                return-1;
            }
            /*
            for(int i=0;i<100;i++){
                printf("%s\n",vals[i]);
            }
            */
            FILE *dest;
            dest = fopen(argv[5],"wb");
            if(!dest){
                printf("Error in destiny file\n");
            }
            for(int i=0;i<100;i++){
                fputs(vals[i],dest);
            }
            fclose(dest);
            if(q){
                printf("... sorting %s file with quicksort\n",argv[2]);
            }else{
                printf("... sorting %s file with mergesort\n",argv[2]);
            }
            printf("... Results file can be found at ./%s\n",argv[5]);
            return 0;
        }
        if (argc<5){
            printf("Missing arguments\n");
            return -1;
        }
        
        FILE *source;
        char line[250];
        char *strs[100];
        int index=0;
        source = fopen(argv[1],"r");
        if(!source){
            printf("Error in file\n");
            return-1;
        }
        while (fgets(line,250,source)){//getline() - probar en gitpod. strcat para agregar final de linea
            char *l=calloc(250,sizeof(char)*250);
            strcpy(l,line);
            strs[index++]=l;
        }
        //strs[index-1]=strcat(strs[index-1],"\n");
        fclose(source);
        int q;
        int (*comp)(void*,void*);
        comp=(int (*)(void *, void *))&compStrs;
        if(strcmp(argv[2],"-quicksort")==0){
            quicksort((void *)strs,0,99,comp);
            q=1;
        }else if(strcmp(argv[2],"-mergesort")==0){
            mergesort((void *)strs,0,99,comp);
            q=0;
        }else{
            printf("no soting algorithm provided\n");
            return-1;
        }
        FILE *dest;
        dest = fopen(argv[4],"wb");
        if(!dest){
            printf("Error in destiny file\n");
        }
        for(int i=0;i<100;i++){
            fputs(strs[i],dest);
        }
        fclose(dest);
        if(q){
            printf("... sorting %s file with quicksort\n",argv[1]);
        }else{
            printf("... sorting %s file with mergesort\n",argv[1]);
        }
        printf("... Results file can be found at ./%s\n",argv[4]);
        return 0;
    }
}
