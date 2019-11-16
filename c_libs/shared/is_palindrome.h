#ifndef __IS_PALINDROME__
#define __IS_PALINDROME__

#include <string>
#include <iterator>
#include <algorithm>


namespace solib {
    bool is_palindrome(std::string&);
}

#endif // __IS_PALINDROME__
// -U_FORTIFY_SOURCE -fstack-protector -Wall -Wunused-but-set-parameter -Wno-free-nonheap-object -fno-omit-frame-pointer '-std=c++0x' -MD -MF