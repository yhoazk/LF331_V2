#include "is_palindrome.h"
#include <vector>
#include <iostream>

int main(){
    std::vector<std::string> pos = {
    "anna",
    "civic",
    "kayak",
    "level",
    "madam",
    "mom",
    "noon",
    "racecar",
    "radar",
    "redder",
    "refer",
    "repaper",
    "rotator",
    "rotor",
    "sagas",
    "solos",
    "stats",
    // false
    "rate",
    "anal", // if the commas are removed the compiler concatenates this strings
    "testi",
    "topoto",
    // /false
    "tenet",
    "wow"
    };
    for(auto& test : pos){
        std::cout << test << " is ";
        if(!solib::is_palindrome(test)){
            std::cout << "not ";
        } 
        std::cout << "palindrome\n";
    }

    return 0;
}