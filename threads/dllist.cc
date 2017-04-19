#include "dllist.h"
#include "system.h"
#include "dllist.h"
#include "synch.h"
Lock *lock3;

extern int testnum;
DLList::DLList()
{
	first = last = (DLLElement *)NULL;
	max_key = 0;
	min_key = 10000;
}

DLList::~DLList()
{
	delete this;
}

void DLList::Prepend(void * item)
{
	if (first == (DLLElement *)NULL)
	{
		first = new DLLElement(item, min_key-- - 1);
		max_key = min_key;
		last = first;
	}
	else
	{
		DLLElement *t = new DLLElement(item, min_key-- - 1);
		t->next = first;
		t->prev = (DLLElement *)NULL;
		first->prev = t;
	}
	length++;
}

void DLList::Append(void * item)
{
	if (last == (DLLElement *)NULL)
	{
		last = new DLLElement(item, max_key++ + 1);
		min_key = max_key;
		last = first;
	}
	else
	{
		DLLElement *t = new DLLElement(item, max_key++ + 1);
		t->prev = last;
		t->next = (DLLElement *)NULL;
		last->next = t;
	}
	length++;
}

void * DLList::Remove(int * keyPtr)
{
	lock3 = new Lock("dllist lock");
	if (first == (DLLElement *)NULL)
	{
		return (DLLElement *)NULL;
	}
	else
	{
		*keyPtr = first->key;
		void *item;
		if(3 == testnum)
		{
			lock3->Acquire();
			currentThread->Yield();
		}

		if(first->next == NULL)
		{
			item = first->item;
			delete first;
			length--;
			return item;
		}
		first->next->prev = (DLLElement *)NULL;
		item = first->item;
		DLLElement *t= first->next;
		delete first;
		first = t;
		length--;
		if(3 == testnum)
		{
			currentThread->Yield();
			lock3->Release();
		}
		return item;
	}
}

bool DLList::IsEmpty()
{
	if (first == (DLLElement *)NULL)
	{
		return true;
	}
	return false;
}

void DLList::SortedInsert(void * item, int sortKey)
{
	// Note that after invoking Prepend or Append, the list
	// is sorted as a ascending order.
	DLLElement *t = new DLLElement(item, sortKey);

	if (IsEmpty())
	{
		if(testnum == 4) {
			currentThread->Yield();
		}
		first = last = t;
		t->next = t->prev = (DLLElement *)NULL;
	}
	else
	{
		DLLElement *p = first;

		while (p != (DLLElement *)NULL)
		{
			if (p->key > sortKey)
			{
				break;
			}
			p = p->next;
		}
		if(p == NULL)
		{
			// Meaning that this item should be inserted into tha tail of the list.
			p = last;
			p->next = t;
			t->prev = p;
			t->next = (DLLElement *)NULL;
			last = t;
		}
		else if(p == first)
		{
			// Meaning that this item should be inserted into the head of the list.
			t->next = first;
			t->prev = (DLLElement *)NULL;
			first->prev = t;
			first = t;
		}
		else
		{
			t->next = p;
			t->prev = p->prev;
		}
	}

	if (sortKey > max_key)
	{
		max_key = sortKey;
	}

	if (sortKey < min_key)
	{
		min_key = sortKey;
	}

	length++;
}

void * DLList::SortedRemove(int sortKey)
{
	if (IsEmpty())
	{
		return (DLLElement *)NULL;
	}
	else
	{
		DLLElement *p = first;

		while (p != (DLLElement *)NULL)
		{
			if (p->key == sortKey)
			{
				p->next->prev = p->prev;
				void *item = p->item;
				delete p;
				length--;
				return item;
			}

			p = p->next;
		}

		return (DLLElement *)NULL;
	}
}

DLLElement::DLLElement(void * itemPtr, int sortKey)
{
	item = itemPtr;
	key = sortKey;
	next = prev = (DLLElement *)NULL;
}
