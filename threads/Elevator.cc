#include "Elevator.h"
#include "system.h"
#include "utility.h"
#include <sstream>
using namespace std;

Elevator::Elevator(char *debugName, int numFloors, int myID): m_name(debugName), m_maxFloors(numFloors), m_currentFloor(1), m_occupancy(0)
{

}

Elevator::~Elevator()
{

}

void Elevator::OpenDoors()
{

}

void  Elevator::CloseDoors()
{

}

void Elevator::VisitFloor(int floor)
{

}

void Elevator::Enter()
{

}

void Elevator::Exit()
{

}

void Elevator::RequestFloor(int floor)
{

}

Building::Building(char *debugName, int numFloors, int numElevators): m_name(debugName), m_numFloors(numFloors), m_numElevators(numElevators)
{
	DEBUG(BLUE, '3', "A building is now being constructing!");
	m_elevator = (Elevator **)AllocBoundedArray(sizeof(Elevator *) * m_numElevators);
	for (int i = 0; i < m_numElevators; i++)
	{
		m_elevator[i] = new Elevator("elevator", m_numFloors, i);
	}
	m_noElevator = new Condition("no elevator condition");
	m_noElevatorLock = new Lock("no elevator condition lock");
}

Building::~Building()
{
	for (int i = 0; i < m_numElevators; i++)
	{
		delete m_elevator[i];
	}
	DeallocBoundedArray((char *)elevator, sizeof(Elevator *) * m_numElevators);
	delete m_noElevator;
	delete m_noElevatorLock;
}

void Building::CallUp(int fromFloor)
{
	//TODO: Simulate the press 
}

void Building::CallDown(int fromFloor)
{

}

Elevator *Building::AwaitUp(int fromFloor)
{

}

Elevator *Building::AwaitDown(int fromFloor)
{

}

