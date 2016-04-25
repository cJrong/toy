
/*
* CopyRight (C) Jrong Chen
*/

#ifndef __SB__UNIX__FILES__H__INCLUDE__
#define __SB__UNIX__FILES__H__INCLUDE__

#include <sb/sb_config.h>

#include <unistd.h>
#include <fcntl.h>

typedef int         sb_fd_t;

#define sb_open_file(name, mode, create, access)                              \
    open((const char *)(name), (mode)|(create), (access))

#define SB_FILE_RDONLY          O_RDONLY
#define SB_FILE_WRONLY          O_WRONLY
#define SB_FILE_RDWR            O_RDWR
#define SB_FILE_APPEND          O_APPEND

#define SB_FILE_CREATE_OR_OPEN  O_CREAT
#define SB_FILE_OPEN            O_RDONLY

#define sb_close_file(fd)           close((fd))

#define sb_delete_file(name)        unlink((const char *)(name))

#define sb_read_file(fd, buffer, size)  \
    read(fd, buffer, size)

#endif

