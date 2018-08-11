#ifndef LEAKS_DETECTOR_H
#define LEAKS_DETECTOR_H

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

#define HANDLE_ERROR(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)
#define HANDLE_ERROR_EN(en, msg) do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#define BUF 1024

#endif /*LEAKS_DETECTOR_H*/