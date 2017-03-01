#include "dllist.h"

DLList::DLList()
{
	first = last = nullptr;
}

DLList::~DLList()
{
	delete this;
}

void DLList::Prepend(void * item)
{
	if (first == nullptr)
	{
		first = new DLLElement(item, min_key-- - 1);
		max_key = min_key;
		last = first;
	}
	else
	{
		DLLElement *t = new DLLElement(item, min_key-- - 1);
		t->next = first;
		t->prev = nullptr;
		first->prev = t;
	}
}

void DLList::Append(void * item)
{
	if (last == nullptr)
	{
		last = new DLLElement(item, max_key++ + 1);
		min_key = max_key;
		last = first;
	}
	else
	{
		DLLElement *t = new DLLElement(item, max_key++ + 1);
		t->prev = last;
		t->next = nullptr;
		last->next = t;
	}
}

void * DLList::Remove(int * keyPtr)
{
	if (first = nullptr)
	{
		return nullptr;
	}
	else
	{
		*keyPtr = first->key;
		first->next->prev = nullptr;
		void *item = first->item;
		delete first;
		return item;
	}
}

bool DLList::IsEmpty()
{
	if (first == nullptr)
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
		first = last = t;
		t->next = t->prev = nullptr;
	}
	else
	{
		DLLElement *p = first;

		while (p != nullptr)
		{
			if (p->key > sortKey)
			{
				break;
			}
			p = p->next;
		}

		t->next = p;
		t->prev = p->prev;
	}

	if (sortKey > max_key)
	{
		max_key = sortKey;
	}

	if (sortKey < min_key)
	{
		min_key = sortKey;
	}
}

void * DLList::SortedRemove(int sortKey)
{
	if (IsEmpty())
	{
		return nullptr;
	}
	else
	{
		DLLElement *p = first;

		while (p != nullptr)
		{
			if (p->key == sortKey)
			{
				p->next->prev = p->prev;
				void *item = p->item;
				delete p;
				return item;
			}

			p = p->next;
		}

		return nullptr;
	}
}

DLLElement::DLLElement(void * itemPtr, int sortKey)
{
	item = itemPtr;
	key = sortKey;
	next = prev = nullptr;
}
