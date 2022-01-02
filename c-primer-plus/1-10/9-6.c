#include<stdio.h>

void three(double*i, double*j, double*k);

int main(int argc, char **argv)
{
    double i, j, k;
    while(scanf("%lf %lf %lf",&i, &j, &k) == 3){
        three(&i, &j, &k);
        printf("Min: %f\n",i);
        printf("Middle: %f\n",j);
        printf("Max: %f\n",k);
        printf("Enter q to quit\n");
    }

    return 0;
}

void three(double*i, double*j, double*k)
{
    double max = *i;
    double min = 0;
    double mid = 0;

    
    if(*j > *i ){
        if(*k > *j){
            max = *k;
            min = *i;
            mid = *j;
        }else{
            max = *j;
            if(*k > *i){
                min = *i;
                mid = *k;
            }else{
                min = *k;
                mid = *i;
            }
        }
    }

    if(*k > *i ){
        if(*j > *k){
            max = *j;
            min = *i;
            mid = *k;
        }else{
            max = *k;
        if(*j > *i){
            min = *i;
            mid = *j;
        }else{
            min = *j;
            mid = *i;
        }
        }
    }

    if(*i > *j && *i > *k){
        if(*j > *k){
            min = *k;
            mid = *j;
        }else{
            min = *j;
            mid = *k;
        }
    }

    *i = min;
    *j = mid;
    *k = max;
}