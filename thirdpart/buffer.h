#ifndef _BUFFER_H
#define _BUFFER_H
typedef struct buffer
{
    long ref_count;
    unsigned long capacity;
    unsigned long size;
    struct buffer *next;
    char   buf[0];
}*buffer_t;


buffer_t buffer_create_and_acquire(buffer_t,unsigned long);
buffer_t buffer_acquire(buffer_t,buffer_t);
void     buffer_release(buffer_t*);

#endif
