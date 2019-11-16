#include <math.h>
#include <stdio.h>

float my_math(float x){

    if(abs(x - M_PI/2.0f) < 0.0001f){
        return INFINITY;
    }
    return sin(x) / cos(x);
}


int main(int argc, char const *argv[])
{
    float in_x = 0;
    scanf("%f", &in_x);
    printf("%f\n", my_math(in_x));
    return 0;
}
