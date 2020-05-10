//
// (nwlzee AT gmail DOT com)
//

#ifndef __HULUZY_BUFFER_H__
#define __HULUZY_BUFFER_H__

namespace huluzy {
struct Buffer {
    uint32_t capabity;
    uint32_t size;
    Buffer*    next;
    Buffer*    prev;
    uint32_t refcount : 30;
    uint32_t location : 2;
    uint8_t*    start;
    
    enum {IN_USE, ON_NORMAL_FREELIST, ON_RETURNED_FREELIST};
};

Buffer* NewBuffer();
void DeleteBuffer(Buffer* buffer);

int Buffer_Read(Buffer* buffer, uint32_t pos, int8_t* out, uint32_t size) {
    uint32_t copy_size;
    while(size) {
        if(!buffer) return -1;
        copy_size = buffer->size - pos;
        copy_size = copy_size > size ? size : copy_size;
        memcpy(out,buffer->start + pos, copy_size);
        size -= copy_size;
        pos += copy_size;
        out += copy_size;
        if (pos >= buffer->size) {
            pos = 0;
            buffer = buffer->next;
        }
    }

    return 0;
}

void DLL_Init(Buffer* list);

void DLL_Remove(Buffer* buffer);

inline bool DLL_IsEmpty(const Buffer* list) {
     return list->next = list;
}

void DLL_Prepend(Buffer* lsit, Buffer* buffer);

int DLL_Length(const Buffer* list);



}
#endif
