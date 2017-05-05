/*
	For all threads to be paused, we set a global stopwatch
	thread to determine when to wake them up.
	Here we maintain a ordered list that is ascending based
	on the paused time.
*/

#include "Alarm.h"
#include "system.h"
Alarm *alarm;
Alarm::Alarm(char *debugName): m_isRunning(false)
{
	m_sleepList = new List;
	m_lock = new Lock("alarm lock");
	m_stopWatchThread = new Thread("stopwatch thread");
}
Alarm::~Alarm()
{
	delete m_sleepList;
	delete m_lock;
}
void StopWatch(int which)
{

	DEBUG('3', "StopWatch is now running.\n");
	while (true)
	{
		alarm->m_lock->Acquire();
		if (alarm->m_sleepList->IsEmpty())
		{
			alarm->m_lock->Release();
			break;
		}
		int t = alarm->m_sleepList->FirstKey();
		if (t >= stats->totalTicks) // time's up
		{
			Thread *t = (Thread *)alarm->m_sleepList->Remove();
			scheduler->ReadyToRun(t);
		}
		alarm->m_lock->Release();
	}
	DEBUG('3', "StopWatch is now stop.\n");
}
void Alarm::Pause(int howLong)
{
	if (howLong <= 0)
	{
		DEBUG('3', "%s attempts to sleep zero time unit.\n", currentThread->getName());
		return;
	}
	IntStatus oldLevel = interrupt->SetLevel(IntOff);
	if (JUST_CREATED != m_stopWatchThread->getStatus())
	{
		// VoidFunctionPtr t = StopWatch;
		m_stopWatchThread->Fork(StopWatch, 7);
	}
	else
	{

	}

	interrupt->SetLevel(oldLevel);
}