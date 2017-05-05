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
	alarm = this;
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
			continue;
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
	if (!m_isRunning)
	{
		m_stopWatchThread->Fork(StopWatch, 7);
		m_sleepList->SortedInsert((void *)currentThread, howLong * TimerTicks);
		m_isRunning = true;
	}
	else
	{
		m_sleepList->SortedInsert((void *)currentThread, howLong * TimerTicks);
	}

	interrupt->SetLevel(oldLevel);
}
