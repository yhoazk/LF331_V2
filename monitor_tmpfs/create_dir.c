#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    int ret_val = 0;
    if ((ret_val = mkdir("/tmp/my_tmp/test_dir", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) != 0){
        printf("asdasd\n");
    } else {
        printf("xxxxx\n");
    }
    return 0;
}
