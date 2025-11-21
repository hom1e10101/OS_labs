#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <set>

#include "Pipe.hpp"

std::string removeVowels(const std::string& str) {
    std::string res;
    std::set<char> ms = {'a', 'e', 'i', 'o', 'u', 'y'};

    for (char c : str) {
        if (c != 0 && ms.find(tolower(c)) == ms.end()) {
            res.push_back(c);
        }
    }

    return res;
}

int main() {
    char filename[256];
    PipeRead(0, filename, sizeof(filename));
    std::ofstream out(filename, std::ios::app);
    if (!out.is_open()) {
        std::cout << "Error, can`t open " << filename << std::endl;
        return 1;
    }

    char s[256];
    while(PipeRead(0, &s, sizeof(s)) > 0) {
        std::string str = removeVowels(s);

        PipeWrite(1, str.c_str(), sizeof(str.c_str()));
        out << str << std::endl;
    }

    out.close();
    std::cout << "Child process " << getpid() << " finished." << std::endl;
    return 0;
}
