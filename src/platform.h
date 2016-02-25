#ifndef PLATFORM_H
#define PLATFORM_H

#include <string.h>
#include <stdlib.h>
#ifdef _WIN32
#include <direct.h>
#include <io.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif

void getFilePath(char filePath[], const char file[]);

void getGameDirPath(char filePath[], const char fileName[]);

void createGameDir();

bool fileExists(const char *filePath);

#endif
