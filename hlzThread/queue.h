//
// Author:  zhanglao (nwlzee at gmail dot com)
//


#ifndef __HULUZY_COMMON_QUEUE_H__
#define __HULUZY_COMMON_QUEUE_H__

#include <cstdio>
#include <cstdlib>
#include "job.h"
#include "common/noncopyable.h"
#include "common/mutex.h"

namespace huluzy {
/*
typedef int (*JobProc)(void*,void *);

typedef struct
{
  void* Context;
  JobProc fproc;
} Job;
 
*/

template<typename T>
class Queue : noncopyable {
 public:
 Queue():nelem_(0) {
    head_ = NULL;
    tail_ = NULL;
}
  ~Queue() {}
  void Push(void *data) {
    MutexHolder h(lock_);
  Cell *new_cell;
  if (data != NULL) {
    new_cell = malloc(sizeof(Cell));
    if (!new_cell) {
      // error
    }
    new_cell->ptr_data = data;
    new_cell->next = tail_;
    new_cell->prev = NULL;
    if ( tail_ == NULL) {
      //assert(nelem_ == 0);  // TODO
      head_ = new_cell;
      tail_ = new_cell;
    } else {
      tail_->prev = new_cell;
      tail_ = new_cell;
    }

    nelem_++;
  }
  }
  void Push_front(void *data) {
    MutexHolder h(lock_);
  Cell* new_cell;
  if ( NULL == data) {
    return;
  }
  new_cell = malloc(sizeof(Cell));
  new_cell->ptr_data;
  new_cell->prev = head_;
  new_cell->next = NULL;
  if (NULL == head_) {
    // TODO assert(nelem);
    head_ = new_cell;
    tail_ = new_cell;
  } else {
    head_->next = new_cell;
    head_ = new_cell;
  }
  nelem_++;
  }
  void *Pop() {
     void *data = NULL;
  Cell *remove_cell;

  MutexHolder h(lock_);
  if ( nelem_ <= 0) {
    return NULL;
  }
  remove_cell = head_;
  data = remove_cell->ptr_data;
  head_ = remove_cell->prev;
  if ( NULL== head_) {
    tail_ = NULL;
  } else {
    head_->next = NULL;
  }
  free(remove_cell);
  nelem_--;
  return data;
  }
  //  bool Replace(void* o,void* n);
  int QSize() {
    MutexHolder h(lock_);
  return nelem_;
  }
 private:
   struct Cell {
    void *ptr_data;
    Cell *next,*prev;
    };

  Cell *head_;
  Cell *tail_;
  int nelem_;
  Mutex lock_;
  
};

template<typename T> 
class DLinkedListQueue : noncopyable {
 public:
 DLinkedListQueue():head_(NULL),tail_(NULL) {}
  ~DLinkedListQueue(){}
  void Push(T* item) {
//	printf("insert item %p\n",item);
    MutexHolder h(lock_);
  if (NULL == item) {
    return;
  }
	count_++;
  item->next = tail_;
  item->prev = NULL;
  if ( NULL == tail_) {
    tail_ = item;
    head_ = item;
  } else {
    tail_->prev = item;
    tail_ = item;
  }
	//printf("head=%p,tail=%p\n",head_,tail_);
  }
  void Push_front(T* item) {
    MutexHolder h(&lock_);
  if (NULL == item) {
    return;
  }
  item->prev = head_;
  item->next = NULL;
  if (NULL == head_) {
    head_ = item;
    tail_ = item;
  } else {
    head_->next = item;
    head_ = item;
  }
  }
  T* Pop(){
     T* pitem;
  MutexHolder h(lock_);
  if (empty_()) {
//	printf("%d\n",count_);
    return NULL;
  }
  pitem = head_;
  head_ = head_->prev;
  if ( NULL == head_) {
    tail_ = NULL;
  } else {
    head_->next = NULL;
  }
  pitem->next = pitem->prev = NULL;
  return pitem;
  }
  bool Replace(T* o, T* n) {
    MutexHolder h(lock_);
  if (empty_()) {
    return false;
  }
  n->next = o ->next;
  o->next->prev = n;
  n->prev = o->prev;
  o->prev->next = n;

  o->next = o->prev = NULL;
  return true;
  }
 private:
  bool empty_() const {
    return (head_ == tail_ && NULL == head_);
  }
  T* head_;
  T* tail_;
  Mutex lock_;
  int count_;
};
} // namespace huluzy
#endif

