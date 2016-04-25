
/*
 * CopyRight (C) Jrong Chen
 */

#ifndef __SB__FILES__H__INCLUDE__
#define __SB__FILES__H__INCLUDE__

#include <sb/sb_config.h>

#include <windows.h>

typedef HANDLE  sb_fd_t;

sb_fd_t sb_open_file(sb_char_t *name, sb_ulong_t mode, sb_ulong_t create, sb_ulong_t access);

#endif