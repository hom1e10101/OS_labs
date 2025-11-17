#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <string>

#include "Pipe.hpp"

#include <sys/wait.h>

int main() {
    int pipe1[2];
    int pipe2[2];
    PipeCreate(pipe1);
    PipeCreate(pipe2);
    
    std::cout << "input file for child1\n";
    std::string file1;
    std::cin >> file1;
    
    std::cout << "input file for child2\n";
    std::string file2;
    std::cin >> file2;


    pid_t pid1 = ProcessCreate();
    if (!pid1) {
        PipeClose(pipe1[1]);
        PipeRedirect(pipe1[0], STDIN_FILENO);
        PipeClose(pipe1[0]);
        ProcessExecute("./child", "child");
    }
    pid_t pid2 = ProcessCreate();
    if (!pid2) {
        PipeClose(pipe2[1]);
        PipeRedirect(pipe2[0], STDIN_FILENO);
        PipeClose(pipe2[0]);
        ProcessExecute("./child", "child");
    }
    PipeClose(pipe1[0]);
    PipeClose(pipe2[0]);

    sleep(2);

    std::cout << "Child1 started with PID: " << pid1 << ", file: " << file1 << std::endl;
    std::cout << "Child2 started with PID: " << pid2 << ", file: " << file2 << std::endl;

    PipeWrite(pipe1[1], file1.c_str(), sizeof(file1.c_str()));
    PipeWrite(pipe2[1], file2.c_str(), sizeof(file2.c_str()));

    std::cout << "\nWrite strings:\n";
    std::string input;

    int cnt = 0;
    while (std::cin >> input) {
        if (cnt % 2 == 0) {
            PipeWrite(pipe1[1], input.c_str(), sizeof(input.c_str()));
        } else {
            PipeWrite(pipe2[1], input.c_str(), sizeof(input.c_str()));            
        }
        cnt++;
    }

    PipeClose(pipe1[1]);
    PipeClose(pipe2[1]);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    std::cout << "Parent process finished. Processed " << cnt << " lines." << std::endl;
    return 0;
}
