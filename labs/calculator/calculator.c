#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// adds/subtracts/multiplies all values that are in the *values array.
// nValues is the number of values you're reading from the array
// operator will indicate if it's an addition (1), subtraction (2) or
// multiplication (3)
long calc(int operator, int nValues, int *values) {
    long sum = values[0];
    if (operator == 1){
        for (int i = 1;i<nValues;i++){
            sum+=values[i];
        }
    }else if (operator == 2){
        for (int i = 1;i<nValues;i++){
            sum-=values[i];
        }
    }else if (operator == 3){
        for (int i = 1;i<nValues;i++){
            sum*=values[i];
        }
    }else {
        return -1;
    }
    return sum;
}

int main(int argc, char **argv) {
    if (argc<3){
        printf("you need to pass at least 3 arguments:\n");
        printf("./calculator <operator> <numbers separated by space>\n");
        return -1;
    }else{
        int *values,length;
        length = argc-2;
        values = malloc(sizeof(int) * length);
        long result;
        for (int i =0;i<length;i++){
            int n = atoi(argv[i+2]);
            if (n==0){
                printf("Conversion error.\n");
                printf("pass only positive integers\n");
            }else{
                values[i]=n;
            }
        }
        char *oper = argv[1], *add = "add", *sub = "sub", *mult = "mult";
        if (strcmp(oper,add)==0){
            result = calc(1,length,values);
        }else if(strcmp(oper,sub)==0){
            result = calc(2,length,values);
        }else if(strcmp(oper,mult)==0){
            result = calc(3,length,values);
        }else{
            printf("wrong operator. Must be add, sub or mult\n");
            return -1;
        }
        printf("result: %ld",result);
        return 0;
    }
}