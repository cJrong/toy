

#ifndef __SB__TIME__H__INCLUDE__
#define __SB__TIME__H__INCLUDE__

#include <sb/sb_config.h>

#if defined(SB_LINUX) || defined(SB_UNIX)
#include <sb/os/unix/sb_unix_time.h>
#endif

#if defined(SB_WIN32)
#include <sb/os/win32/sb_win32_time.h>
#endif


#endif
