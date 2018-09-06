#include <stdio.h>
#include <fcntl.h> /* for open syscall */
#include <sys/stat.h>
int main(int argc,char** argv){
  const char* this_file = "filetype.c";
  const char* this_dir = "../filetype";

  struct stat st;

  stat(this_file, &st);
  if(S_ISREG(st.st_mode)){
    printf("%s is a regular file\n", this_file);
  }

  stat(this_dir, &st);
  if(S_ISDIR(st.st_mode)){
    printf("%s is a directory\n", this_dir);
  }

  return 0;
}
