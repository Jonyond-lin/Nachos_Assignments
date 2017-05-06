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

	DEBUG(YELLOW, '3', "StopWatch is now running.\n");
	while (true)
	{
		// alarm->m_lock->Acquire();
		IntStatus oldLevel = interrupt->SetLevel(IntOff);
		DEBUG(YELLOW, '3', "StopWatch is now running at %d ticks.\n", stats->totalTicks);
		if (alarm->m_sleepList->IsEmpty())
		{
			DEBUG(YELLOW, '3', "The sleepList is currently empty!\n");
			//alarm->m_lock->Release();
			interrupt->SetLevel(oldLevel);
			continue;
		}
		if (alarm->m_shouldStop)
		{
			DEBUG(YELLOW, '3', "Now the StopWatch should be stopped.\n");
			// alarm->m_lock->Release();
			interrupt->SetLevel(oldLevel);
			break;
		}

		int t = alarm->m_sleepList->FirstKey();
		DEBUG(YELLOW, '3', "The first key is %d and current tick is %d.\n", t, stats->totalTicks);
		if (stats->totalTicks >= t) // time's up
		{
			Thread *t = (Thread *)alarm->m_sleepList->Remove();
			scheduler->ReadyToRun(t);
		}
		// alarm->m_lock->Release();
		interrupt->SetLevel(oldLevel);
	}
	DEBUG(YELLOW, '3', "StopWatch is now stop.\n");
}
void Alarm::Pause(int howLong)
{
	if (howLong <= 0)
	{
		DEBUG(YELLOW, '3', "%s attempts to sleep zero time unit.\n", currentThread->getName());
		return;
	}
	IntStatus oldLevel = interrupt->SetLevel(IntOff);
	if (!m_isRunning)
	{
		m_sleepList->SortedInsert((void *)currentThread, stats->totalTicks + howLong * TimerTicks);
		m_isRunning = true;
		m_stopWatchThread->Fork(StopWatch, 7);
	}
	else
	{
		m_sleepList->SortedInsert((void *)currentThread, stats->totalTicks + howLong * TimerTicks);
	}
	DEBUG(YELLOW, '3', "thread %s will keep pausing until reaching tick %d.\n", currentThread->getName(), stats->totalTicks + howLong * TimerTicks);
	currentThread->Sleep();
	interrupt->SetLevel(oldLevel);
	
	
}

void Alarm::SetShouldStop(bool shouldStop)
{
	DEBUG(YELLOW, '3', "Set the m_shouldStop to true.");
	m_shouldStop = shouldStop;
}