#include<stdio.h>
#define JL_L 3.785f
#define YL_GL 1.609f

int main(int argc, char **argv)
{
    printf("请输入里程数和燃料加仑数：\n");
    float jl;//加仑
    float yl;//英里
    scanf("%f %f", &yl, &jl);
    printf("%.1fyl/jl\n",yl/jl);

    float gl = yl / YL_GL;//公里
    float l = jl / JL_L;//升
    printf("%.1fl/100km\n",l/gl);

    return 0;
}