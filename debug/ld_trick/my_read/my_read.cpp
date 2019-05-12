//#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>

namespace std
{
 namespace istream
 {
    ssize_t read(int fd, void* data, size_t size){
      static const char text[] = "so much peeps";
      strcpy(data, text);
      return(sizeof(text));
    }
 } // namespace nameststeam
} // namespace std