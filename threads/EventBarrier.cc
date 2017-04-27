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
