#include <stdio.h>
#include <stdlib.h>

static int daymonthtab[2][13] = {
    {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

/* month_day function's prototype*/
void month_day(int year, int yearday, int *pmonth, int *pday){
    int month, leap;
    leap = year%4 == 0 && year%100 != 0 || year%400 == 0;
    month=1;
    while(yearday>daymonthtab[leap][month]){
        yearday-=daymonthtab[leap][month];
        month++;
    }
    *pmonth=month;
    *pday=yearday;
}

int main(int argc, char **argv) {
    int m=0,d=0;
    int *pm=&m, *pd=&d;
    char *months[13]={"Inv","Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
    if(argc<3){
        printf("Error. Missing arguments\n");
        printf("Pleas run as ./month_day <year> <yearday>\n");
        return -1;
    }else{
        int year = atoi(argv[1]);
        int yearday = atoi(argv[2]);
        if(year == 0 || yearday == 0){
            printf("Error. Not number argument or equal to 0\n");
            return -1;
        }
        if(year<1||yearday<1){
            printf("Error. Arguments must be positive numbers\n");
            return -1;
        }
        int leap;
        leap = year%4 == 0 && year%100 != 0 || year%400 == 0;
        if(leap == 0&&yearday>365){
            printf("Error. Yearday on a not leap year must be greater than 0 and less than 365\n");
            return -1;
        }else if(yearday>366){
            printf("Error. Yearday on a leap year must be greater than 0 and less than 366\n");
            return -1;
        }else{
            month_day(year,yearday,pm,pd);
            printf("%s %02d, %d\n",months[m],d,year);
        }
    }
    return 0;
}