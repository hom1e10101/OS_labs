#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <string>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <cstring>
#include <sys/wait.h>

#include "os.hpp"

SharedData* CreateSharedMemory(const char* name) {
    int shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(1);
    }
    ftruncate(shm_fd, sizeof(SharedData));
    SharedData* data = (SharedData*)mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (data == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    close(shm_fd);
    sem_init(&data->sem_file, 1, 1);
    sem_init(&data->sem_parent, 1, 0);
    sem_init(&data->sem_child, 1, 0);
    return data;
}
SharedData* OpenSharedMemory(const char* name) {
    int shm_fd = shm_open(name, O_RDWR, 0);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(1);
    }
    SharedData* data = (SharedData*)mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (data == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    close(shm_fd);
    return data;
}
void DestroySharedMemory(const char* name, SharedData* data) {
    sem_destroy(&data->sem_file);
    sem_destroy(&data->sem_parent);
    sem_destroy(&data->sem_child);
    munmap(data, sizeof(SharedData));
    shm_unlink(name);
}
ProcessRole ProcessCreate() {
    pid_t pid = fork();
    if (pid == -1) {
        std::cout << "Ошибка создания process" << std::endl;
        exit(-1);
    }
    if (pid == 0) {
        return IS_CHILD;
    }
    return IS_PARENT;
}
void ProcessExecute(const char* program, const char* arg, const char* shared_memory_name) {
    execl(program, arg, shared_memory_name, NULL);
    perror("execl");
    exit(1);
}
void SemaphorePost(sem_t* sem) {
    if (sem_post(sem) == -1) {
        perror("sem_post");
        exit(1);
    }
}
void SemaphoreWait(sem_t* sem) {
    if (sem_wait(sem) == -1) {
        perror("sem_wait");
        exit(1);
    }
}
