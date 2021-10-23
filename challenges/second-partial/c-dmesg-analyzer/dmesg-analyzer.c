#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//linked list taken from https://www.geeksforgeeks.org/generic-linked-list-in-c-2/
struct Node
{
    char  *data;
    struct Node *next;
};
  

void push(struct Node** head_ref, char *new_data)
{
    struct Node* new_node = (struct Node*)calloc(1,sizeof(struct Node));
  
    new_node->data  =strdup(new_data);
    new_node->next = (*head_ref);
    (*head_ref)    = new_node;
}

void printList(struct Node *node)
{
    while (node != NULL)
    {
        printf("%s\n",node->data);
        node = node->next;
    }
}

struct nlist{
    struct nlist *next;
    char *name;
    char *defn;
    struct Node *vals;
};
#define HASHSIZE 1024
struct nlist *hashtable[HASHSIZE];

unsigned hash(char *s){
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++){
        hashval = *s + 31 * hashval;
    }
    return hashval % HASHSIZE;
}

struct nlist *lookup(char *s){
    struct nlist *np;
    for(np = hashtable[hash(s)]; np !=NULL; np = np->next){
        if(strcmp(s,np->name)==0){
            return np;
        }
    }
    return NULL;
}

struct nlist *install(char *name, char *defn){
    struct nlist *np;
    unsigned hashval;
    if((np=lookup(name))==NULL){ //not found
        np = (struct nlist *) calloc(1,sizeof(*np));
        if(np == NULL || (np->name=strdup(name))==NULL){
            return NULL;
        }
        hashval = hash(name);
        np->next = hashtable[hashval];
        hashtable[hashval] = np;
        np->vals=NULL;
    }else
        free((void *)np->defn);
    if((np->defn = strdup(defn))==NULL)
        return NULL;
    push(&np->vals,defn);
    return np;
}


#define REPORT_FILE "report.txt"

void analizeLog(char *logFile, char *report);

int main(int argc, char **argv) {

    if (argc < 2) {
	printf("Usage:./dmesg-analizer logfile.txt\n");
	return 1;
    }

    analizeLog(argv[1], REPORT_FILE);

    return 0;
}

void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);
    struct nlist *start = NULL;
    struct Node *lis = NULL;
    // Implement your solution here.
    int infile, outfile, openFlags;
    mode_t filePerms;
    infile = open(logFile,O_RDONLY);
    if (infile == -1){
        printf("Error opening file\n");
        exit(-1);
    }
    
    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                S_IROTH | S_IWOTH;      /* rw-rw-rw- */
    outfile = open(report,openFlags,filePerms);
    
    if (outfile == -1){
        printf("Error opening file\n");
        exit(-1);
    }
        
    ssize_t numread;
    char buf[10];
    long lsize=0;
    off_t se;
    while ((numread = read(infile, buf, 1)) > 0){
        if(strcmp(buf,"\n")==0){
            if((se=lseek(infile,-lsize-1,SEEK_CUR))!=-1){
                ssize_t lineread;
                char *line = (char *) calloc(lsize,sizeof(char));
                if((lineread= read(infile,line,lsize))>=0){
                    int linesize = strlen(line);
                    char *categoria=calloc(linesize,sizeof(char));
                    char *log=calloc(linesize,sizeof(char));
                    int inicio = 14;
                    int fin=0;
                    for(int i=inicio;i<linesize-1;i++){
                        if(line[i]==':'&&line[i+1]==' '){
                            fin=i;
                            i=linesize;
                        }
                    }
                    if(fin==0){
                        //general category
                        categoria="general\0";
                        log=line;
                        log[linesize]='\0';
                    }else{
                        int ind=0;
                        
                        for (int i = inicio; i < fin; i++)
                        {
                            categoria[ind++]=line[i];
                        }
                        categoria[ind]='\0';
                        ind = 0;
                        for (int i = 0; i < inicio; i++)
                        {
                            log[ind++]=line[i];
                        }
                        for (int i = fin; i < linesize; i++)
                        {
                            log[ind++]=line[i];
                        }
                        log[ind]='\0';
                    }
                    install(categoria,log);
                    lsize=0;
                    if((se=lseek(infile,1,SEEK_CUR))==-1){
                        printf("Error reading line\n");
                        free(line);
                        exit(-1);
                    }
                    free(line);
                }else{
                    printf("Error reading line\n");
                    free(line);
                    exit(-1);
                }
            }else{
                printf("Error seeking\n");
                exit(-1);
            }
        }else{
            lsize++;
        }
        
    }
    if (lsize>0){
        if((se=lseek(infile,-lsize,SEEK_CUR))!=-1){
                ssize_t lineread;
                char *line = (char *) calloc(lsize,sizeof(char));
                if((lineread= read(infile,line,lsize))>=0){
                    int linesize = strlen(line);
                    char *categoria=(char *)calloc(linesize,sizeof(char));
                    char *log=(char *)calloc(linesize,sizeof(char));
                    int inicio = 14;
                    int fin=0;
                    for(int i=inicio;i<linesize-1;i++){
                        if(line[i]==':'&&line[i+1]==' '){
                            fin=i;
                            i=linesize;
                        }
                    }
                    if(fin==0){
                        categoria="general\0";
                        log=line;
                        log[linesize]='\0';
                    }else{
                        int ind=0;
                        for (int i = inicio; i < fin; i++)
                        {
                            categoria[ind++]=line[i];
                        }
                        categoria[ind]='\0';
                        ind = 0;
                        for (int i = 0; i < inicio; i++)
                        {
                            log[ind++]=line[i];
                        }
                        for (int i = fin; i < linesize; i++)
                        {
                            log[ind++]=line[i];
                        }
                        log[ind]='\0';
                    }
                    install(categoria,log);
                    lsize=0;
                    free(line);
                }else{
                    printf("Error reading line\n");
                    free(line);
                    exit(-1);
                }
            }else{
                printf("Error seeking\n");
                exit(-1);
            }
    }

    for (int i=0;i<HASHSIZE;i++){
        start = hashtable[i];
        if (start!=NULL){
            int catsize=0;
            for(int i=0;i<strlen(start->name);i++){
                if(start->name[i]=='\0'){
                    catsize++;
                    i=strlen(start->name);
                }else{
                    catsize++;
                }
            }
            if (write(outfile, start->name, catsize) <0)
                exit(-1);
            if (write(outfile, ":\n", 2) <0)
                exit(-1);
            for (lis=start->vals;lis!=NULL;lis=lis->next){
                int logsize=0;
                for(int i=0;i<strlen(lis->data);i++){
                    if(lis->data[i]=='\0'){
                        logsize++;
                        i=strlen(lis->data);
                    }else{
                        logsize++;
                    }
                }
                if (write(outfile, "  ", 2) <0)
                    exit(-1);
                if (write(outfile, lis->data, logsize) <0)
                    exit(-1);
                if (write(outfile, "\n", 1) <0)
                    exit(-1);
                    
            }
        }
    }

    if (numread == -1)
        exit(-1);
    if (close(infile) == -1){
        printf("Error closing file\n");
        exit(-1);
    }
    if (close(outfile) == -1){
        printf("Error closing file\n");
        exit(-1);
    }
    printf("Report is generated at: [%s]\n", report);
}
