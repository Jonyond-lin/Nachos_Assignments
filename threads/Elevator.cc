#include "Elevator.h"
#include "system.h"
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
	 
}

Building::~Building()
{

}

void Building::CallUp(int fromFloor)
{

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