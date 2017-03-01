#include "dllist.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

int random(int start, int end)
{
	srand(unsigned(time(0)));
	return start + ((float)end - start) * rand() / RAND_MAX;
}
void GenerateNItems(int N, DLList *list)
{
	
}

void RemoveNItems(int N, DLList *list)
{

}

int main()
{
	cout << random(1,10) << endl;
	return 0;
}