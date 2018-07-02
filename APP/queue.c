#include "queue.h"
#include <stdlib.h>
#include "stdio.h"
#include "stm8l15x_it.h"
#include "string.h"
#include "LCD_ZK.h"
void CreateQueue(Pqueue Q)
{
	if((Q=(Pqueue)malloc(sizeof(queue)))==NULL)
	{
		//printf("malloc err");	
	}

	(Q)->front=(Q)->rear= 0 ;
}


bool EnQueue(Pqueue Q,SNode *NetPackage)
{
	if(IsFull(Q))
	{
		//printf("sq_queue full");
		return FALSE;
	}

	Q->rear=(Q->rear+1)%FIFO_SIZE;
	Q->Pack[Q->rear]=*NetPackage;
	return TRUE;
}

bool IsEmpty(Pqueue Q)
{
	if(Q->front==Q->rear)
		return TRUE;
	else
		return FALSE;
}

bool IsFull(Pqueue Q)
{
	if((Q->rear+1)%FIFO_SIZE==Q->front)
		return TRUE;
	else
		return FALSE;
}

bool DeQueue(Pqueue Q,SNode *NetPackage)
{
	if(IsEmpty(Q))
	{
		//printf("queue empty");
		return FALSE;
	}
	Q->front=(Q->front+1)%FIFO_SIZE;
	*NetPackage=Q->Pack[Q->front];
	return TRUE;
}
char size_queue(Pqueue Q)
{
   return (Q->rear-Q->front+FIFO_SIZE)%FIFO_SIZE;
}
//取出队列中的数组和id
u16 get_pack(SNode *pack,u8 *arr)
{
    u8 i;
    memset(arr,0x00,49);
     
    for(i = 0; i < pack->data_len; i++)
      arr[i] = pack->body[i];
    
   return pack->callID;
}
//串口得到的消息打包入队
void set_pack(u8 *getdata,SNode *pack,u8 len)
{
	u8 i;
    memset(pack->body,0x00,49);
    pack->callID = 0;
    pack->data_len = len;
   
	for(i=0;i<len;i++)
	{
		pack->body[i] = getdata[i+2];
	}
    pack->callID = (getdata[0]<< 8 ) | getdata[1];
}