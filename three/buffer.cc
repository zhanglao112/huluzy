#include "span.h"

namespace huluzy {

Buffer* NewBuffer(uint32_t capabity) {
    Buffer* result = xmalloc(sizeof(Buffer));
    assert(result != NULL);
    char* buffer = (char*)xmalloc(capabity);
    assert(buffer != NULL);
    result->start = buffer;
    result->capabity = capabity;

}

void DeleteBuffer(Buffer* buffer) {
    if (!--buffer->refcount) {
        free(buffer->start);
        free(buffer);
    }
}

void DLL_Init(Buffer* list) {
    list->next = list;
    list->prev = list;
}

void DLL_Remove(Buffer* buffer) {
    buffer->prev->next = buffer->next;
    buffer->next->prev = buffer->prev;
    buffer->prev = NULL;
    buffer->next  = NULL;
}

int DLL_Length(const Buffer* list) {
    int result  = 0;
    for(Buffer* b = list->next; b != list; b = b->next) {
        result++;
    }

    return result;
}

void DLL_Preprend(Buffer* list, Buffer* buffer) {
    assert(buffer->next == NULL);
    assert(buffer->prev == NULL);
    buffer->next = list->next;
    buffer->prev = list;
    list->next->prev = buffer;
    list->next = buffer;
}

}
