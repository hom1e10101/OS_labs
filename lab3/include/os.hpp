#pragma once

#include "shared_data.hpp"

enum ProcessRole {
    IS_PARENT,
    IS_CHILD
};
ProcessRole ProcessCreate();
SharedData* CreateSharedMemory(const char*);
SharedData* OpenSharedMemory(const char*);
void DestroySharedMemory(const char*, SharedData * data);
void ProcessExecute(const char* program, const char* arg, const char*);
void SemaphoreWait(sem_t* sem);
void SemaphorePost(sem_t* sem);