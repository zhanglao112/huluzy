
#ifndef __HULUZY_LOG_INL_H__
#define __HULUZY_LOG_INL_H__

#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <cstring>

/*
typedef enum {
    LOG_LEVEL_FATAL,
    LOG_LEVEL_VERBOSE,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_NOT_SET = -1
}   LogLevel;
//typedef int myint;


static struct {
    const char *name;
    LogLevel level;
} log_levels [] = 
{
    {"FATAL",    LOG_LEVEL_FATAL},
    {"VERBOSE",  LOG_LEVEL_VERBOSE},
    {"DEBUG",    LOG_LEVEL_DEBUG},
    {NULL,       LOG_LEVEL_NOT_SET}
};

const char*
log_level_name(LogLevel level)
{
    u_int i;
    for (i = 0;log_levels[i].name != NULL;++i) {
        if (log_levels[i].level == level) return log_levels[i].name;

    }
    return NULL;
}


void do_log(LogLevel level,const char *fmt,va_list args) {
    fprintf(stderr,"[%s:%d]"fmt"\n",__FILE__,__LINE__); 

}

*/


#define VERBOSE(fmt,...)    fprintf(stderr,"""VERBOSE:""[%s:%d]"fmt"\n",__FILE__,__LINE__,##__VA_ARGS__)

#ifdef DEBUG
#define DEBUG(fmt,...)    fprintf(stderr,"[DEBUG][%s:%d]"fmt"\n",__FILE__,__LINE__,##__VA_ARGS__)
#else
#define DEBUG(fmt,...)
#endif

#define FATAL(fmt,...)    fprintf(stderr,"[FATAL][%s:%d]"fmt"\n",__FILE__,__LINE__,##__VA_ARGS__)



#endif
//
