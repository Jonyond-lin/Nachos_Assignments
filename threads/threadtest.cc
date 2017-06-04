// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "dllist.h"
#include <cstdio>
#include "synch.h"
#include "Table.h"
#include "BoundedBuffer.h"
#include "Alarm.h"
#include "Elevator.h"

using namespace std;

// testnum is set in main.cc
int testnum = 1;
DLList *list;
Lock *lock;
Table *table;
BoundedBuffer *buffer;
extern Alarm *g_alarm;
Building *g_building;
int numFloors = 5;
int numElevators = 1;
int numRiders = 2;
void rider(int id, int srcFloor, int dstFloor);
//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------
void GenerateNItems(int N, DLList *list);
void RemoveNItems(int N, DLList *list);
void MergeNItems(DLList *list);

void RiderThread(int which)
{
	//building = new Building("building", 10, 1);
	g_alarm->Pause(1);
	int from, to;
	while (1) {
		from = Random() % numFloors;
		to = Random() % numFloors;
		// printf("%d %d\n", from, to);
		// from = 1;
		// to = 7;
		if (to == from) g_alarm->Pause(1);
		else rider(which, from, to);
		// from = 7;
		// to = 2;
		// rider(which, from, to);
	}
	//building->GetElevator(0)->Run();
}

void TestElevator()
{
	g_building = new Building("building", 10, 1);
	for (int i = 0; i < numRiders; i++) {
		Thread *t = new Thread("rider thread");
		t->Fork(RiderThread, i);
	}
	g_building->GetElevator(0)->Run();
}

void rider(int id, int srcFloor, int dstFloor) {
	Elevator *e;

	if (srcFloor == dstFloor)
		return;

	DEBUG('a', "Rider %d travelling from %d to %d\n", id, srcFloor, dstFloor);
	do {
		if (srcFloor < dstFloor) {
			DEBUG('a', "Rider %d CallUp(%d)\n", id, srcFloor);
			g_building->CallUp(srcFloor);
			DEBUG('a', "Rider %d AwaitUp(%d)\n", id, srcFloor);
			e = g_building->AwaitUp(srcFloor);
		}
		else {
			DEBUG('a', "Rider %d CallDown(%d)\n", id, srcFloor);
			g_building->CallDown(srcFloor);
			DEBUG('a', "Rider %d AwaitDown(%d)\n", id, srcFloor);
			e = g_building->AwaitDown(srcFloor);
		}
		DEBUG('a', "Rider %d Enter()\n", id);
	} while (!e->Enter()); // elevator might be full!

	DEBUG('a', "Rider %d RequestFloor(%d)\n", id, dstFloor);
	e->RequestFloor(dstFloor); // doesn't return until arrival
	DEBUG('a', "Rider %d Exit()\n", id);
	e->Exit();
	DEBUG('a', "Rider %d finished\n", id);
}
void
SimpleThread1(int which)
{
	lock->Acquire();
	// printf("Thread %d\n", which);
	GenerateNItems(5, list);
	// printf("Thread %d\n", which);
	RemoveNItems(2, list);
	// printf("Thread %d\n", which);
	RemoveNItems(3, list);
	lock->Release();
}
void SimpleThread2(int which)
{
	lock->Acquire();
	GenerateNItems(3,list);
	currentThread->Yield();
	
	MergeNItems(list);//Merge 1&2
	currentThread->Yield();

	MergeNItems(list);//Merge 2&3
	currentThread->Yield();

	MergeNItems(list);//print result
	lock->Release();
}

//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between two threads, by forking a thread 
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void 
SimpleThread3(int which)
{
	if(list->IsEmpty()){
		GenerateNItems(1, list);
	}
	lock->Acquire();
		currentThread->Yield();
	RemoveNItems(1, list);
	currentThread->Yield();
	lock->Release();
}

void ThreadTestTable(int which)
{
	int user[3],i;
	for(i=0;i<3;i++)
	{
		void *obj = (void *)((i+2)*(which + 3) + 1 );
		user[i]=table->Alloc(obj);
		printf("Thread %d save %d in table[%d]\n",which,(int)obj,user[i]);
		currentThread->Yield();
	}
	for(i=0;i<3;i++)
	{	
		printf("Thread %d:table[%d] = %d\n",which,user[i],(int)table->Get(user[i]));
	}
	currentThread->Yield();
	for(i=0;i<3;i++){
		table->Release(user[i]);
		currentThread->Yield();
	}
}

void ThreadTestBuffer(int which){
	char c[2] = "A";
	int i;
	for(i=0;i<20;i++){
		buffer->Write(c,1);
		printf("Producer:write %s\n",c);
		c[0]++;
	}
}

void BoundedBufferTest(){
	Thread *t = new Thread("forked thread");
	t->Fork(ThreadTestBuffer,1);

	for(int i=0;i<10;i++){
                char buf[3] = {0};
                buffer->Read(buf,2);         
	        printf("Consumer:read %s\n",buf);
        }
}

void
ThreadTest1(void (*p)(int))
{
    DEBUG('t', "Entering ThreadTest1\n");

    Thread *t = new Thread("forked thread");

    t->Fork(p, 1);
    p(0);
}

//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void
ThreadTest()
{
	lock = new Lock("dllist lock");
	list = new DLList();
	table = new Table("t1",10);
	buffer = new BoundedBuffer("b1",5);
    switch (testnum) {
    case 1:
	ThreadTest1(SimpleThread1);
	break;
    case 2:
	ThreadTest1(SimpleThread2);
	break;
    case 3:
	ThreadTest1(SimpleThread3);
	break;
    case 4:
	ThreadTest1(SimpleThread1);
	break;
    case 5:
	ThreadTest1(ThreadTestTable);
	break;
    case 6:
	BoundedBufferTest();
	break;
    default:
	printf("No test specified.\n");
	break;
    }
}	

