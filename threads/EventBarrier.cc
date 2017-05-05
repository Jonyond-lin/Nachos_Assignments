#include "EventBarrier.h"
#include "system.h"

EventBarrier::EventBarrier(const char *debugName):
	m_name(debugName),
	m_waiterNum(0),
	m_status(UNSIGNALED)
{
	m_lock = new Lock("eventbarrier lock");
	m_unfinished = new Condition("event unfinished condition");
	m_completed = new Condition("all threads completed conditon");
}

EventBarrier::~EventBarrier()
{
	delete m_lock;
	delete m_unfinished;
	delete m_completed;
}
void EventBarrier::Wait()
{
	DEBUG('3', "Thread [%s] wants to become a waiter.\n", currentThread->getName());
	m_lock->Acquire();
	if(SIGNALED == m_status)
	{
		m_waiterNum++;
		DEBUG('3', "No unsignaled event, return immediately.\n");
		m_lock->Release();
		return;
	}
	m_unfinished->Wait(m_lock);
	m_lock->Release();

}

void EventBarrier::Signal()
{
	DEBUG('3', "Thread [%s] has finished the event and launches a signal.\n", currentThread->getName());
	m_lock->Acquire();
	m_status = SIGNALED;
	m_unfinished->Broadcast(m_lock);
	m_completed->Wait(m_lock);
	m_status = UNSIGNALED;
	m_lock->Release();
	DEBUG('3', "Thread [%s] has done the signal process.\n", currentThread->getName());
}

void EventBarrier::Complete()
{
	DEBUG('3', "Thread [%s] invokes the EventBarrier::Complete() function.\n", currentThread->getName());
	m_lock->Acquire();
	if(1 != m_waiterNum) // The last waiter
	{
		m_waiterNum--;
		m_completed->Broadcast(m_lock);
		DEBUG('3', "All of the waiter threads have done their work.\n");
	}
	else
	{
		m_waiterNum--;
		m_completed->Wait(m_lock);
	}
	m_lock->Release();
}
