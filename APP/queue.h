#ifndef _QUEUE_H
#define _QUEUE_H

#include "stm8l15x.h"

#define FIFO_SIZE 6

typedef struct _SNode_t
{   
    u8 data_len;
	u8 body[49];
}SNode;
//定义一个先进先出(FIFO)的队列NetQueue:
typedef struct queue
{
	SNode Pack[FIFO_SIZE];
	unsigned int front,rear;
}queue,*Pqueue;

extern void CreateQueue(Pqueue  Q);
extern bool EnQueue(Pqueue Q,SNode *NetPackage);
extern bool DeQueue(Pqueue Q,SNode *NetPackage);
extern bool IsEmpty(Pqueue Q);

extern void queue_show(Pqueue Q);
extern bool IsFull(Pqueue Q);
char size_queue(Pqueue Q);

void set_pack(u8 *getdata,SNode *pack,u8 len);
void get_pack(SNode *pack,u8 *arr);
#endif
