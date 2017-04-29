#ifndef ALARM_H
#define ALARM_H
class Alarm
{
public:
	Alarm(char *debugName);
	~Alarm();
	char *getName() { return m_name; }
	void Pause(int howLong);
}
#endif
