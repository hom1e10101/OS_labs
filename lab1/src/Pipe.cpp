#include <iostream>
#include <unistd.h>
#include <cstdlib>

#include "Pipe.hpp"

pid_t ProcessCreate() {
    pid_t pid = fork();
    if (pid == -1) {
        std::cout << "Ошибка создания process" << std::endl;
        exit(-1);
    }
    return pid;
}

int PipeCreate(int fd[2]) {
    int err = pipe(fd);
    if (err == -1) {
        std::cout << "Ошибка создания pipe" << std::endl;
        exit(-1);
    }

    return err;
}

int PipeWrite(int fd, const void * buf, size_t count) {
    int bytes = write(fd, buf, count);
    if (bytes == -1) {
        std::cout << "Ошибка записи в pipe" << std::endl;
        exit(-1);
    }

    return bytes;
}

void PipeClose(int fd) {
    close(fd);
}

int PipeRead(int fd, void * buf, size_t count) {
    int bytes = read(fd, buf, count);
    if (bytes == -1) {
        std::cout << "Ошибка чтения из pipe" << std::endl;
        exit(-1);
    }
 
    return bytes;
}

void PipeRedirect(int oldfd, int newfd) {
    if (dup2(oldfd, newfd) == -1) {
        std::cerr << "Ошибка перенаправления дескриптора" << std::endl;
        exit(-1);
    }
}

void ProcessExecute(const char* program, const char* arg) {
    execl(program, program, arg, NULL);
    std::cerr << "Ошибка запуска программы: " << program << std::endl;
    exit(-1);
}
