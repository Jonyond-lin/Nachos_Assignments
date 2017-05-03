#ifndef ALARM_H
#define ALARM_H

#include "synch.h"
#include "list.h"

class Alarm
{
public:
	Alarm(char *debugName);
	void ~Alarm();
	char *getName() { return m_name; }
	void Pause(int howLong);
	void StopWatch(int which);
private:
	List *m_sleepList;
	Lock *m_lock;
	char *m_name;
	Thread *m_stopWatchThread;
	bool m_isRunning;
}

#endif
