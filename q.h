#include <stdio.h>
#include <stdlib.h>
#include "TCB.h"

struct TCB_t *RunQ;
void InitQ(struct TCB_t **q)
{
	*q = NULL;
}

void AddQ(struct TCB_t **q, struct TCB_t *tcb)
{
	struct TCB_t *temp, *qt;
	qt = *q;
	if(qt == NULL)
	{
		qt = tcb;
		qt->next = qt->prev = NULL;
		*q = qt;
	}

	else if(qt->prev == NULL)
	{
		temp = tcb;
		qt->prev = qt->next = temp;
		temp->prev = temp->next = qt;
		*q = qt;

	}

	else 
	{
		temp = tcb;
		qt->prev->next = temp;
		temp->prev = qt->prev;
		temp->next = qt;
		qt->prev = temp;
		*q = qt;
	}
}

struct TCB_t* DelQ(struct TCB_t **q)
{
	struct TCB_t *temp, *qt;
	qt = *q;
	if(qt == NULL)
	{
		*q = qt;
		return NULL;
	}

	else if(qt->prev == NULL)
	{
		
		temp = qt;
		qt = NULL;
		*q = qt;
		return temp;
	}

	else if(qt->next->next == qt)
	{
		temp = qt;
		qt = qt->next;
		qt->next = qt->prev = NULL;
		*q = qt;
		return temp;
	}
	else
	{
		temp = qt;
		qt->next->prev = qt->prev;
		qt->prev->next = qt->next;
		qt = qt->next;
		*q = qt;

		return temp;
	}
}

void RotateQ(struct TCB_t **q)
{
	struct TCB_t *qt = *q;

	if( qt != NULL)
	{
		if(qt->next != NULL)
		{
			qt = qt->next;
			
			*q = qt;
		}
	}

}
