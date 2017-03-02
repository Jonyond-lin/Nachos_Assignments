#include "dllist.h"
#include <iostream>

using namespace std;

void GenerateNItems(int N, DLList *list);
void RemoveNItems(int N, DLList *list);

int main()
{
	DLList *list = new DLList();
	GenerateNItems(10, list);
	RemoveNItems(5, list);
	

	return 0;
}
