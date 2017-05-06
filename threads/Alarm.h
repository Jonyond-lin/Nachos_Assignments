#ifndef ALARM_H
#define ALARM_H

#include "synch.h"
#include "list.h"

class Alarm
{
public:
	Alarm(char *debugName);
	~Alarm();
	char *getName() { return m_name; }
	void Pause(int howLong);
	friend void StopWatch(int which);
	void SetShouldStop(bool shouldStop);
private:
	List *m_sleepList;
	Lock *m_lock;
	char *m_name;
	Thread *m_stopWatchThread;
	bool m_isRunning, m_shouldStop;
};

#endif
