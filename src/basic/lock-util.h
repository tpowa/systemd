/* SPDX-License-Identifier: LGPL-2.1-or-later */
#pragma once

#include <fcntl.h>

typedef struct LockFile {
        int dir_fd;
        char *path;
        int fd;
        int operation;
} LockFile;

int make_lock_file_at(int dir_fd, const char *p, int operation, LockFile *ret);
static inline int make_lock_file(const char *p, int operation, LockFile *ret) {
        return make_lock_file_at(AT_FDCWD, p, operation, ret);
}
int make_lock_file_for(const char *p, int operation, LockFile *ret);
void release_lock_file(LockFile *f);

#define LOCK_FILE_INIT { .dir_fd = -EBADF, .fd = -EBADF }

/* POSIX locks with the same interface as flock(). */
int posix_lock(int fd, int operation);
void posix_unlockpp(int **fd);

#define CLEANUP_POSIX_UNLOCK(fd)                                   \
        _cleanup_(posix_unlockpp) _unused_ int *CONCATENATE(_cleanup_posix_unlock_, UNIQ) = &(fd)

/* Open File Description locks with the same interface as flock(). */
int unposix_lock(int fd, int operation);
void unposix_unlockpp(int **fd);

#define CLEANUP_UNPOSIX_UNLOCK(fd)                                   \
        _cleanup_(unposix_unlockpp) _unused_ int *CONCATENATE(_cleanup_unposix_unlock_, UNIQ) = &(fd)
