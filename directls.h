#ifndef DIRECTLS_H
#define DIRECTLS_H

#include <dirent.h>

typedef struct Directory_
{
    char name[MAXNAMELEN + 1];
} Directory;

int directory(const char *path, Directory **dir);

#endif
