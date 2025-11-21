#pragma once

#include <cstddef>
#include <string>

int ProcessCreate();
int PipeCreate(int fd[2]);
int PipeWrite(int, const void*, size_t);
int PipeRead(int, void*, size_t);
void PipeClose(int);

void PipeRedirect(int, int);
void ProcessExecute(const char*, const char*);