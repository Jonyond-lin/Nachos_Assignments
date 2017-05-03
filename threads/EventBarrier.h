/* EventBarrier is a synchronization mechanism which 
   allows a group of threads to wait and respond one 
   event synchronously.

   There are 2 categories of the caller of the 
   EventBarrier:
   1. EventBarier::Wait()'s caller:
   		Indicate that the caller is waiting for the
		finishing of the event.
   2. EventBarrier:::Signal()'s caller:
   		Indicate that the event has finished by the
		caller. If no waiter exists, this behavior
		will be logged.
*/

#ifndef EVENTBARRIER_H
#define EVENTBARRIER_H
#include "list.h"
#include "synch.h"

enum barrierStatus
{
	SIGNALED, UNSIGNALED
};

class EventBarrier
{

public:
	EventBarrier(const char *debugName);
	~EventBarrier();
	const char *getName() { return m_name; }

	void Wait(); // Wait until the event is signaled. 
	// Return immediately if already in the signaled state.
	
	void Signal(); // Signal the event and block until 
	// all threads that wait for this events have responded. The
	// EventBarrier reverts to the unsignaled state when Signal() returns.
	
	void Complete(); // Indicate that the calling thread has 
	// finished responding to a signaled event, and block 
	// until all other threads that wait for this event have also responed.
	
	int Waiters() { return m_waiterNum; } // Return a count of threads that are waiting for the event or that have not yet responded to it.

private:
	const char *m_name;
	barrierStatus m_status;
	int m_waiterNum;
	Lock *m_lock;
	Condition *m_unfinished, *m_completed;
};
#endif
