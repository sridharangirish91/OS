#include "sem.h"

#define N 10

struct message
{
	int data[10];
	struct message *prev, *next;
};

struct port
{
	struct message *mess;
	int count;
	struct Semaphore_t *full, *empty;
	struct Semaphore_t *mutex;
};

struct set
{
	struct port p[100];
};

struct set s;

void init_port()
{
	int i;
	for(i = 0; i < 100; i ++)
	{ 
		s.p[i].mess = NULL;
		s.p[i].count = 0;
		s.p[i].mutex = CreateSem(1);
		s.p[i].full = CreateSem(0);
		s.p[i].empty = CreateSem(N);
	}

}

void Add(struct port *p, struct message **q, struct message *tcb)
{
	struct message *temp, *qt;
	qt = *q;
	if(qt == NULL)
	{
		qt = tcb;
		qt->next = qt->prev = NULL;
		*q = qt;
		p->count++;
	}

	else if(qt->prev == NULL)
	{
		temp = tcb;
		qt->prev = qt->next = temp;
		temp->prev = temp->next = qt;
		*q = qt;
		p->count ++;

	}

	else 
	{
		if(p->count >= N)
			printf("\nQUEUE IS FULL");
		else
		{
			temp = tcb;
			qt->prev->next = temp;
			temp->prev = qt->prev;
			temp->next = qt;
			qt->prev = temp;
			*q = qt;
			p->count++;
		}
	}
}

struct message* Del(struct port *p, struct message **q)
{
	struct message *temp, *qt;
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
		p->count --;
		*q = qt;
		return temp;
	}

	else if(qt->next->next == qt)
	{
		temp = qt;
		qt = qt->next;
		qt->next = qt->prev = NULL;
		*q = qt;
		p->count --;
		return temp;
	}
	else
	{
		temp = qt;
		qt->next->prev = qt->prev;
		qt->prev->next = qt->next;
		qt = qt->next;
		*q = qt;
		p->count --;
		return temp;
	}
}

void Send(int pno, struct message *m)
{
	P(s.p[pno].empty);
	P(s.p[pno].mutex);
	Add(&s.p[pno], &s.p[pno].mess, m);
	V(s.p[pno].mutex);
	V(s.p[pno].full);

}

struct message Receive(int pno)
{
	struct message *m;
a :	P(s.p[pno].full);
	P(s.p[pno].mutex);
	m = Del(&s.p[pno], &s.p[pno].mess);
	if (m == NULL) 
	{
		V(s.p[pno].mutex);
		V(s.p[pno].empty);
		goto a;
	}
	V(s.p[pno].mutex);
	V(s.p[pno].empty);
	return *m;
}

