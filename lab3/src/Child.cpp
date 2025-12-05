#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <set>
#include <cstring>
#include <cctype>

#include "os.hpp"

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

int main(int argc, char** argv) {
    SharedData* data = OpenSharedMemory(argv[1]);
    if (!data) {
        std::cout << "Error opening shared memory" << std::endl;
        return 1;
    }

    std::string filename(data->file);
    std::ofstream out(filename, std::ios::app);
    if (!out.is_open()) {
        std::cout << "Error, can't open " << filename << std::endl;
        SemaphorePost(&data->sem_parent);
        return 1;
    }

    SemaphorePost(&data->sem_parent);

    bool running = true;
    while (running) {
        SemaphoreWait(&data->sem_child);
        
        if (data->signal == 1) {
            running = false;
        } else if (data->signal == 0) {
            std::string input(data->file);
            if (!input.empty()) {
                std::string result = removeVowels(input);
                
                out << result << std::endl;
                std::cout << "Child " << getpid() << " processed: " << input 
                         << " -> " << result << std::endl;
            }
        }
        
        SemaphorePost(&data->sem_parent);
    }

    out.close();
    std::cout << "Child process " << getpid() << " finished." << std::endl;
    return 0;
}