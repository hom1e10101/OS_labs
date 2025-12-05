#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <string>
#include <cstring>
#include <sys/wait.h>

#include "os.hpp"

int main() {
    SharedData* data1 = CreateSharedMemory("/shared1");
    SharedData* data2 = CreateSharedMemory("/shared2");
    
    if (!data1 || !data2) {
        std::cout << "Error creating shared memory" << std::endl;
        return 1;
    }

    std::cout << "Input file for child1: ";
    std::string file1;
    std::cin >> file1;
    
    std::cout << "Input file for child2: ";
    std::string file2;
    std::cin >> file2;

    strncpy(data1->file, file1.c_str(), sizeof(data1->file));
    data1->signal = 0;
    data1->number = 0;

    strncpy(data2->file, file2.c_str(), sizeof(data2->file));
    data2->signal = 0;
    data2->number = 0;

    ProcessRole role1 = ProcessCreate();
    if (role1 == IS_CHILD) {
        ProcessExecute("./child", "child", "/shared1");
    }

    ProcessRole role2 = ProcessCreate();
    if (role2 == IS_CHILD) {
        ProcessExecute("./child", "child", "/shared2");
    }

    if (role1 == IS_PARENT && role2 == IS_PARENT) {
        std::cout << "Child1 started with PID: " << getpid() + 1 << ", file: " << file1 << std::endl;
        std::cout << "Child2 started with PID: " << getpid() + 2 << ", file: " << file2 << std::endl;

        SemaphoreWait(&data1->sem_parent);
        SemaphoreWait(&data2->sem_parent);

        std::cout << "\nWrite strings (Ctrl+D to finish):\n";
        std::string input;

        int cnt = 0;
        while (std::cin >> input) {
            SharedData* currentData = (cnt % 2 == 0) ? data1 : data2;
            
            strncpy(currentData->file, input.c_str(), sizeof(currentData->file) - 1);
            currentData->file[sizeof(currentData->file) - 1] = '\0';
            currentData->signal = 0;
            
            SemaphorePost(&currentData->sem_child);
            SemaphoreWait(&currentData->sem_parent);
            
            cnt++;
        }

        data1->signal = 1;
        data2->signal = 1;
        SemaphorePost(&data1->sem_child);
        SemaphorePost(&data2->sem_child);

        wait(NULL);
        wait(NULL);

        DestroySharedMemory("/shared1", data1);
        DestroySharedMemory("/shared2", data2);

        std::cout << "Parent process finished. Processed " << cnt << " lines." << std::endl;
    }

    return 0;
}