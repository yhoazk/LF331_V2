
#include "is_palindrome.h"

namespace solib {
/**
 * This function can be improved to verify only 1/2 string
*/
bool is_palindrome(std::string& word){
    auto m = std::mismatch(word.rbegin(), word.rend(), word.begin(), word.end());
    //std::cout << *m.first << '\n';
    //std::cout << *m.second << '\n';
    return ( (m.first == word.rend()) && (m.second == word.end()));
}

} // namespace solib
