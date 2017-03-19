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

using namespace std;

// testnum is set in main.cc
int testnum = 1;
DLList *list;
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
void
SimpleThread1(int which)
{
	// printf("Thread %d\n", which);
	GenerateNItems(5, list);
	currentThread->Yield();
	// printf("Thread %d\n", which);
	RemoveNItems(2, list);
	currentThread->Yield();
	// printf("Thread %d\n", which);
	RemoveNItems(3, list);
}
void SimpleThread2(int which)
{
	GenerateNItems(3,list);
	currentThread->Yield();
	
	MergeNItems(list);//Merge 1&2
	currentThread->Yield();

	MergeNItems(list);//Merge 2&3
	currentThread->Yield();

	MergeNItems(list);//print result
}

//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between two threads, by forking a thread 
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

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
	testnum = 2;
	list = new DLList();
    switch (testnum) {
    case 1:
	ThreadTest1(SimpleThread1);
	break;
    case 2:
	ThreadTest1(SimpleThread2);
	break;
    default:
	printf("No test specified.\n");
	break;
    }
}	

