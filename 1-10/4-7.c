#include<stdio.h>
#include<float.h>

int main(int argc, char **argv)
{
    double i = 1.0 / 3.0;
    float j = 1.0 / 3.0;

    printf("%.6f %.6lf\n", i, j);
    
    printf("%.12f %.12lf\n", i, j);

    printf("%.16f %.16lf\n", i, j);

    printf("%d %d\n",FLT_DIG,DBL_DIG);
    /*float六位后就乱了，double在15后依旧坚挺，在16后就乱了*/
    return 0;
}