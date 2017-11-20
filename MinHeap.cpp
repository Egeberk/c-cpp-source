#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 1000

typedef int ElemType;
#define PARENT(n) (((n)-1)/2 )
#define LC(n) ((2*(n))+1)
#define RC(n) ((2*(n))+2)

typedef struct {
	int size;
	ElemType items[MAXSIZE];
}MinHeap;

void initHeap(MinHeap *);
int isHeapEmpty(MinHeap *);
int isHeapFull(MinHeap*);
void insertToHeap(MinHeap *,ElemType);
ElemType deleteMin(MinHeap *);
void printHeap(MinHeap *);
int main()
{
	MinHeap myHeap;
	initHeap(&myHeap);
	
	insertToHeap(&myHeap,5);
	
	insertToHeap(&myHeap,1);
	
	insertToHeap(&myHeap,8);
	insertToHeap(&myHeap,19);
	insertToHeap(&myHeap,23);
	insertToHeap(&myHeap,18);
	insertToHeap(&myHeap,3);
	insertToHeap(&myHeap,2);
	insertToHeap(&myHeap,16);
	insertToHeap(&myHeap,21);

	while(!isHeapEmpty(&myHeap) )
		printf(" %5d,", deleteMin(&myHeap) );
	printf("\n");
}

void initHeap(MinHeap *h)
{
	 h->size=0;
}
int isHeapEmpty(MinHeap *h)
{
	return h->size == 0;
}
int isHeapFull(MinHeap *h)
{
	return h->size >= MAXSIZE;
}

void printHeap(MinHeap *h)
{
	printf("\n Heap:");
	for(int i=0;i<h->size;i++)
		printf(" %d",h->items[i]);
	printf("\n");
}
//private method or helper function for insert
void heapUp(MinHeap *h, int loc)
{
	while(loc> 0 )
	{
		if(h->items[PARENT(loc)] > h->items[loc])
		{
			ElemType t=h->items[loc];
			h->items[loc]=h->items[PARENT(loc)];
			h->items[PARENT(loc)]=t;
			loc=PARENT(loc);
		}
		else
			break;
	}//end while
}
void insertToHeap(MinHeap *h,ElemType e)
{
	if(!isHeapFull(h) ){
		h->items[h->size]=e;
		heapUp(h,h->size);
		h->size ++;
	}
}
//private method or helper function for delete
void heapDown(MinHeap *h,int loc)
{
	int minLoc=-1;
	while(loc < h->size )
	{
		if(LC(loc) < h->size )
		{
			minLoc=LC(loc);
			if(RC(loc) < h->size )
				if( h->items[minLoc] > h->items[RC(loc)] )
					minLoc=RC(loc);
			if(h->items[loc] > h->items[minLoc] )
			{
				ElemType t=h->items[loc];
				h->items[loc]=h->items[minLoc];
				h->items[minLoc]=t;
				loc=minLoc;
			}
			else break;
		}
		else break;
	}//end while
}
ElemType deleteMin(MinHeap *h)
{
	if(isHeapEmpty(h) )
		exit(0);
	ElemType e=h->items[0];
	h->items[0]=h->items[h->size-1];
	h->size --;
	heapDown(h,0);
	return e;
}


