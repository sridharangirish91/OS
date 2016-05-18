#include "threads.h"

struct Semaphore_t
{
	int count;
	struct TCB_t *sq;
};


struct Semaphore_t* CreateSem(int InputValue)
{
	struct Semaphore_t *s;
	s = malloc(sizeof(struct Semaphore_t));
	s->count = InputValue;
	InitQ(&(s->sq));
	return s;
}

void P(struct Semaphore_t *sem)
{
	struct TCB_t *temp;
	ucontext_t parent;
	sem->count -= 1;
	if(sem->count < 0)
	{
		temp = DelQ(&(RunQ));
		AddQ(&(sem->sq), temp);
		if( RunQ != NULL )
			swapcontext(&(temp->context), &(RunQ->context));  // start the first thread
		
	}
}

void V(struct  Semaphore_t *sem)
{
	struct TCB_t *temp;
	sem->count += 1;
	if(sem->count <= 0)
	{

		temp = DelQ(&(sem->sq));
		AddQ(&RunQ, temp);
	}
	yield();
}