#ifndef __FG_LOG_H__
#define __FG_LOG_H__

/*! @todo improve logging */

#ifndef __linux__
    /* obtain windows temp directory */
    /*#include <windows.h>
    #define FGS_LOG_INIT(unused)                            \
    do {                                                    \
        DWORD dwRetVal = 0;                                 \
        TCHAR lpTempPathBuffer[MAX_PATH];                   \
        dwRetVal = GetTempPath(MAX_PATH, lpTempPathBuffer); \
        if (dwRetVal > MAX_PATH || (dwRetVal == 0))         \
        {                                                   \
            zlog_init("C:/tmp/fgs.log");                    \
        }                                                   \
        else                                                \
        {                                                   \
            zlog_init(lpTempPathBuffer);                    \
        }                                                   \
    } while (0)
*/

/*! @todo logging for windows */
#define FGS_LOG_INIT(unused)
#define FGS_LOG_STOP(unused)
#define FGS_TRACE

#else
    #include <errno.h>
    #include <string.h>
    #include "zlog/zlog.h"
    
    /* just use /tmp/fgs.log on Linux */
    #define FGS_LOG_INIT(unused) zlog_init("/tmp/fgs.log")
    #define FGS_LOG_STOP(unused) zlog_finish()
    #define FGS_TRACE zlogf_time
#endif

#endif /* __FG_LOG_H__ */
