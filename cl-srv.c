/* Project Team Members 

Sridharan Rajagopalan - 1207570360
Karan Maniar - 1209340037
Santhosh Samuel Murali - 1207681887
*/



#include <stdlib.h>
#include <string.h>
#include "msgs.h"

#define spno  10
#define c3pno  5

struct Semaphore_t *msgsem, *printsem;

struct message m;
struct message pr;
char table[10][100];
int pos[10] = {0,0,0,0,0,0,0,0,0,0};
int full;
char str[100];

struct message m1;
char name1[100] = "CLIENT ADD 1";
char modify1[10][100] = {"CLIENT 1 MODIFY 0","CLIENT 1 MODIFY 1","CLIENT 1 MODIFY 2","CLIENT 1 MODIFY 3","CLIENT 1 MODIFY 4","CLIENT 1 MODIFY 5","CLIENT 1 MODIFY 6","CLIENT 1 MODIFY 7","CLIENT 1 MODIFY 8","CLIENT 1 MODIFY 9"};

struct message m2;
char name2[100] = "CLIENT ADD 2";
char modify2[10][100] = {"CLIENT 2 MODIFY 0","CLIENT 2 MODIFY 1","CLIENT 2 MODIFY 2","CLIENT 2 MODIFY 3","CLIENT 2 MODIFY 4","CLIENT 2 MODIFY 5","CLIENT 2 MODIFY 6","CLIENT 2 MODIFY 7","CLIENT 2 MODIFY 8","CLIENT 2 MODIFY 9"};


char tab[10][100];

void server()
{
	int count = 0;
	int len;
	int loop_no;
	int p;
	int j = 0;
	int k;
	int l;
	int st;
	int opt;
	int index;
	int i = 0;
	srand(time(NULL));
	while(1)
	{
		st = 0;
		for(i = 0 ; i < 100 ; i ++) str[i] = '\0';
		m = Receive(spno);
		opt = m.data[0];
		j = 0;
		if(opt >= 0 && opt <= 2)
		{
			len = m.data[1];
			index = m.data[2];
			if(index > 9)
			{
				printf("\nInvalid Index"); fflush(stdout); fflush(stdout);
			}
			loop_no = len/10 + 1;
			if(opt == 0 || opt == 1)
			{
				for (i = 0 ; i < loop_no ; i ++)
				{
					m = Receive(spno);
					for(k = 0 ; k < 10 && j < len; k ++) str[j ++] = (char) m.data[k];
				}
				str[j] = '\0';
			}
		}

		if(opt == 0) // ADD
		{
			for(i = 0 ; i < 10 ; i ++)
				if(pos[i] == 0)
				{
					strcpy(table[i], str);
					pos[i] = 1;
					printf("\nString added to table at %d : %s", i, table[i]); fflush(stdout); fflush(stdout);
					sleep(1);
					if(rand()%10 > 6)
					{
					for( p = 0 ; p < 10 ; p ++)
					{	if(pos[p] == 1)
							printf("\nString %d : %s", p, table[p]); 
						else
							printf("\nString %d is empty", p); 
					}}
					break;		
				}
			if(i == 10) 
			{
				printf("\nTABLE FULL");  fflush(stdout); fflush(stdout);
			}
		}	

		else if(opt == 1)
		{
			if(pos[index] == 0) 
			{
				printf("\nNothing available to modify at %d", index);  fflush(stdout); fflush(stdout);
				sleep(1);
			}
			else
			{
				printf("\nString to Modify at %d : %s", index, table[index]); fflush(stdout); fflush(stdout);
				strcpy(table[index], str);
				printf("\nModified String at %d : %s", index, table[index]); fflush(stdout); fflush(stdout);
			}
		}

		else if(opt == 2)
		{
			if(pos[index] == 0) 
			{
				printf("\nNothing available to delete at %d", index);  fflush(stdout); fflush(stdout);
				sleep(1);
				
			}

			else
			{
				printf("\nString Deleted From Index %d is : %s ", index, table[index]); fflush(stdout); fflush(stdout);
				pos[index] = 0;
				sleep(1);
			}

		}

		if(opt == 3)
		{
			for(l = 0 ; l < 10 ; l ++)
			{	
				pr.data[0] = 0;
				pr.data[1] = strlen(table[l]);	
				pr.data[2] = -1;
				Send(c3pno, &pr);
				for( i = 0 ; table[l][i]!='\0'; i++)
				{
					pr.data[j++] = (int) (table[l][i]);
					if(j == 10 || table[l][i+1] == '\0')
					{
						Send(c3pno, &pr);
						j = 0;
						k ++;
					}
				}
			}
		}
	}
}

void client3()
{
	int len, loop_no;
	struct message m;	
	int i, j, k, st, l;
	int ran;
	srand(time(NULL));
	while(1)
	{
		ran = (rand() % 10);
		if(ran > 6)
		{
			
			for(l = 0 ; l < 10 && ran > 10; l ++)
			{
				P(msgsem);
				m.data[0] = 3;
				Send(spno, &m);
				V(msgsem);
				m = Receive(c3pno);
				len = m.data[1];
				loop_no = len/10 + 1;
				for (i = 0 ; i < loop_no && len != 0 ; i ++)
				{
					m = Receive(c3pno);
					for(k = 0 ; k < 10 && j < len; k ++) str[j ++] = (char) m.data[k];
				}
				str[j] = '\0';
				strcpy(tab[l], str);
			}

			if(full != 0)
			{
				printf("\nTAB CONTENTS:"); fflush(stdout);
				for( i = 0 ; i < 10 ; i ++ )
					printf("\nString %d : %s ", i, tab[i]); fflush(stdout);
			}
		}
		// else
			yield();
	}
}

void client1()
{
	
	int i;
	int k;
	int j;
	int l;
	int z; 
	int alpha = 0;
	int ind; 
	srand(time(NULL));
	while(1)
	{
		do
		{
			z = rand() % 10; 
		}while (!(z>=0 && z <= 2));
		do
		{
			ind = rand() % 10; 
		}while (!(ind>=0 && ind <= 9));
		P(msgsem);
		switch(z)
		{
			case 0 :
			{ 
				j = 0;
				k = 0;
				m1.data[0] = 0;
				m1.data[1] = strlen(name1);
				m1.data[2] = -1;
				Send(spno, &m1);
				for( i = 0 ; name1[i]!='\0'; i++)
				{
					m1.data[j++] = (int) (name1[i]); 
					if(j == 10 || name1[i+1] == '\0')
					{
						Send(spno, &m1);
						j = 0;
						k ++;
					}
				}
				z = 9;
				break;
			}
			case 1 :
			{
				j = 0;
				k = 0;
				m1.data[0] = 1;
				m1.data[1] = strlen(modify1[ind]);
				m1.data[2] = ind;
				Send(spno, &m1);
				for( i = 0 ; modify1[ind][i]!='\0'; i++)
				{
					m1.data[j++] = (int) modify1[ind][i];
					if(j == 10 || modify1[ind][i+1] == '\0')
					{
						Send(spno, &m1);
						j = 0;
						k ++;
					}
				}
				z = 9;
				break;

			}
			case 2 :
			{
				m1.data[0] = 2;
				m1.data[1] = -2;
				m1.data[2] = ind;
				z = 9;
				break;
			}
		}
		V(msgsem);
	}
}

void client2()
{
	int i;
	int k;
	int j;
	int l;
	int z; 
	int alpha = 0;
	int ind; 
	srand(time(NULL));
	while(1)
	{
		do
		{
			z = rand() % 10; 
		}while (!(z>=0 && z <= 2));
		do
		{
			ind = rand() % 10; 
		}while (!(ind>=0 && ind <= 9));
		P(msgsem);
		switch(z)
		{
			case 0 :
			{ 
				j = 0;
				k = 0;
				m2.data[0] = 0;
				m2.data[1] = strlen(name2);
				m2.data[2] = -1;
				Send(spno, &m2);
				for( i = 0 ; name2[i]!='\0'; i++)
				{
					m2.data[j++] = (int) (name2[i]);
					
					if(j == 10 || name2[i+1] == '\0')
					{
						Send(spno, &m2);
						j = 0;
						k ++;
					}
				}
				z = 9;
				break;
			}
			case 1 :
			{
				j = 0;
				k = 0;
				m2.data[0] = 1;
				m2.data[1] = strlen(modify2[ind]);
				m2.data[2] = ind;
				Send(spno, &m2);
				for( i = 0 ; modify2[ind][i]!='\0'; i++)
				{
					m2.data[j++] = (int) modify2[ind][i];
					if(j == 10 || modify2[ind][i+1] == '\0')
					{
						Send(spno, &m2);
						j = 0;
						k ++;
					}
				}
				z = 9;
				break;

			}
			case 2 :
			{
				m2.data[0] = 2;
				m2.data[1] = -1;
				m2.data[2] = ind;
				z = 9;
				break;
			}
		}
		V(msgsem);
	}
}

void main()
{
	printf("\n\n ******Documentation of the program*******\n");
    sleep(1);
	printf("\n\n Client 1 Adds - CLIENT ADD 1 or modifies - CLIENT 1 MODIFY <index> at random \n");
	sleep(2);
	printf("\n\n Client 1 Adds - CLIENT ADD 2 or modifies - CLIENT 2 MODIFY <index> at random \n");
	sleep(2);
    printf("\n\n Client 3 either prints table after getting all 10 strings from the server or yields based on random function in C\n");
    sleep(2);
    printf("\n\n Server handles one request at a time. For Client 1 and Client 2 it sends positive or negative acknoledgements as per whether the server has been able to perform requested operation or not. \n");

	msgsem = CreateSem(1);
	printsem = CreateSem(1);
	init_port();
	start_thread(server);
	start_thread(client1);
	start_thread(client2);
	start_thread(client3);
	run();

}