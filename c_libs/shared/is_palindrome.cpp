
#include "is_palindrome.h"

namespace solib {
/**
 * This function can be improved to verify only 1/2 string
 * The mismatch returns the position of the iterators at the
 * point where the difference was found. If there is no difference
 * the iterator should be the final for both of the instances
*/
bool is_palindrome(std::string& word){
    auto m = std::mismatch(word.rbegin(), word.rend(), word.begin(), word.end());
    return ( (m.first == word.rend()) && (m.second == word.end()) );
}

} // namespace solib
