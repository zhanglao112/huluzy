#ifndef __HULUZY_HULUZY_H__
#define __HULUZY_HULUZY_H__

#include <stdint.h>

typedef int32_t HANDLE;
namespace huluzy {
struct list_node {
    struct list_node *next;
};

struct st_io {
    list_node node;
    struct iovec *iovec;
    int32_t iovec_count;
    uint32_t err_code;
};

}
