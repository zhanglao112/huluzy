//
// Author: zhanglao (nwlzee at gmail dot com)
//

#include "joblist.h"
using namespace huluzy;

JobList::JobList() {

}

JobList::~JobList() {

}

void JobList::InsertRange(void* start,void* end,int N) {
  //MutexHold h(&lock_);
  JobProc *jph = (JobProc)job_entry_.head;
  JobProc *jpt = (JobProc)job_entry_.tail;
  
  // TODO : how to do it well
  JobProc *insStart = (JobProc*)start;
  JobProc *insEnd = (JobProc*)end;
  jpt.next = start;
  jpt= insEnd;
  kjobnum_ += N;
}

void JobList::InsertOne(void *one) {
  // TODO: how can do it well
  JobProc* ins_one = (JobProc)one;
  JobProc* jpt = (JobProc)job_entry_.tail;
  jpt.next = ins_one;
  jpt = ins_one;
  kjobnum++;
}

void* JobList::RemoveOne() {
  JobProc* rm_one = (JobProc*)job_entry_.head;
  job_entry_.head = rm_one.next;
  rm_one.next = NULL;
  kjobnum--;
  return (void*)rm_one;

}
