#include "Alarm.h"
#include "synch.h"
#include "system.h"
#include "list.h"

List sleepList;
int elapsedTime;
void Alarm::Pause(int howLong)
{
	if (howLong <= 0)
	{
		DEBUG('3', "%s attempts to sleep 0 time unit.\n", currentThread->getName());
		return;
	}
	IntStatus oldLevel = interrupt->SetLevel(IntOff);
	// For each thread to be paused
	interrupt->SetLevel(oldLevel);
}