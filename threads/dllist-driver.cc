#include "dllist.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;
int times=1;
int random(int start, int end)
{
	srand(unsigned(time(0)));
	times *= 2;
	return (start + ((float)end - start) * rand() / RAND_MAX) * times;
}

bool existed(int *p, int key, int n)
{
	for(int i=0; i<n; i++)
	{
		if(key == *(p + i))
		{
			return true;
		}
	}
	return false;
}
void GenerateNItems(int N, DLList *list)
{
	int *keys = new int [N];
	memset(keys, 0, N * sizeof(int));
	int keyTemp = 0;
	for(int i = 0; i< N; i++)
	{
		// To generate each item.
		keyTemp = random(1, N * 2);
		while(existed(keys, keyTemp, N))
		{
			keyTemp = random(1, N * 2);
		}
		list->SortedInsert((DLLElement *)NULL, keyTemp);
		//list->ShowList();
		//if(list->IsEmpty()) printf("EMPTY!");
		//else printf("NO EMPTY!");
		cout << "The item with key " << keyTemp << " has been inserted into the list." << endl;
	}
}

void RemoveNItems(int N, DLList *list)
{
	// Just print the key of each to-be-removed item
	if((list == (DLList *)NULL || list->IsEmpty()) && N != 0)
	{
		cerr << "ERROR: The list is empty!!!" << endl;
		return;
	}
	int keyTemp = 0;
	for(int i = 0; i < N; i++)
	{
		if( i == list->length)
		{
			cerr << "ERROR: The list is already empty." << endl;
			return;
		}
		
		list->Remove(&keyTemp);
		//list->ShowList();
		// if(list->IsEmpty()) printf("EMPTY!");
		//else printf("NO EMPTY!");
		cout << "The item with key:" << keyTemp << " has been removed!" << endl;
	}
}
void MergeNItems(DLList *list){
	//Merge the first and the second item
	if(list == (DLList *)NULL || list->IsEmpty())
	{
		cerr << "ERROR: The list is empty!!!" << endl;
		return;
	}
	DLLElement *pp = list->first;
	int a = pp->key;
	pp = pp->next;
	if(pp == NULL)
		cout<<"The sum of three numbers is:"<<a<<endl;
	else	
	{
		int b = pp->key;
		pp->key += a;
		cout<<"Processes:"<<a<<"+"<<b<<"="<<pp->key<<endl;
	}
	list->first = pp;
}
