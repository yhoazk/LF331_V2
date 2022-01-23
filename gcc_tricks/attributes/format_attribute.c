// Note this code does not actually generates a binary, but static
// checkers are able to detect the error on line 22

#include <stdio.h>

extern char *myFormatText1(const char*, ...);
extern char *myFormatText2(const char*, ...) __attribute__((format(printf, 1, 2)));


int main() {
    int a,b;
    float c;

    a = 5;
    b = 6;
    c = 9.099999;

    myFormatText1("Here are some integers %d, %d\n", a,b); // no type checking. Types match
    myFormatText1("Here are some integers %d, %d\n", a,c); // no type checking. Types  mismatch, but no warning

    myFormatText2("Here are some integers %d, %d\n", a,b); // no type checking. Types match
    myFormatText2("Here are some integers %d, %d\n", a,c); // type checking. Types  mismatch,  warning
}