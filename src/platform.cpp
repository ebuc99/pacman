#include "platform.h"
#include <string.h>
#ifdef _WIN32
#define WINDOWS 1
#else
#define WINDOWS 0
#endif
void getFilePath(char filePath[], const char file[]) {
     char installDir[256];
     if(WINDOWS)
         strcpy(installDir, "");
     else
         strcpy(installDir, "/usr/local/share/pacman/");
     strcpy(filePath, installDir);
     strcat(filePath, file);
}