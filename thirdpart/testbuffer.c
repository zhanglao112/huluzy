#include "buffer.h"

int main() {
	buffer_t cur = 0;
    buffer_t b1 = buffer_create_and_acquire(0,16);
    buffer_t b2 = buffer_acquire(0,b1);
    buffer_release(&b1);
    buffer_release(&b2);

    b1 = buffer_create_and_acquire(0,16);
    b1 = buffer_create_and_acquire(b1,16);
    buffer_release(&b1);

    b1 = buffer_create_and_acquire(0,16);
    b1->next = buffer_create_and_acquire(0,16);
    b1->next->next = buffer_create_and_acquire(0,16);
  
    cur = b1;
    while(cur)
    {	
		cur = buffer_acquire(cur,cur->next);
    }
}
