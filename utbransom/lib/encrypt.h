#ifndef CRYPTO
#define CRYPTO

#define _CRT_SECURE_NO_WARNINGS
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_BUFF_PATH 1024

off_t fsize(const char *filename);
void encrypt(const char *file, FILE *file_fd, int file_length);
void encrypt_all(const char* directory, char* current_program);

#endif