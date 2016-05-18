#include "q.h"
#include <errno.h>

void start_thread(void (*function)(void))
{ 
  void *stackP = malloc(8192);

  struct TCB_t *TCB = (struct TCB_t*)malloc(sizeof(struct TCB_t));
  init_TCB(TCB, function, stackP, 8192); //call init_TCB with appropriate arguments
  AddQ(&RunQ, TCB); //  call addQ to add this TCB into the “RunQ” which is a global header pointer
  
}

void run()

{   
  ucontext_t parent;     // get a place to store the main context, for faking
  getcontext(&parent);   // magic sauce
  swapcontext(&parent, &(RunQ->context));  // start the first thread
}

void yield() // similar to run
{
  int err;

  ucontext_t parent;     // get a place to store the main context, for faking
    
  getcontext(&parent);   // magic sauce

  RunQ->context = parent;
  
  RotateQ(&RunQ);

  err = swapcontext(&parent, &(RunQ->context));  // start the first thread

  if(err == -1)
  {
    perror("ERROR");
    exit(0);
  }
  
}