#include <stdio.h>
/*
Note that onle the functions compiled here have the call to instrumented fncs,
for example printf is linked and thus it does not call the enter/exit fncs.

Also note that main also gets the enter/exit and the calling address is always
different due to the address space layout randomization.
*/


// void __cyg_profile_func_enter(void* fn, void* loc) { W/o attribute we get a seg fault bc it will blow the stack 
//for recursive calls
void __attribute__((no_instrument_function)) __cyg_profile_func_enter(void* fn, void* loc) {
    printf("In enter: %p at address %p\n", fn , loc);
}
void __attribute__((no_instrument_function)) __cyg_profile_func_exit(void* fn, void* loc) {
    printf("In exit: %p at address %p\n", fn , loc);
}

int sum(int a, int b){ return a+b; }

// int __attribute__((no_instrument_function)) main(){

int main(){
    printf("Sum: %d\n", sum(5,6));
    return 0;
}