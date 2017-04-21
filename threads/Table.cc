#include "Table.h"
#include "utility.h"
#include "string.h"
Table::Table(char *name, int size) : m_debugName(name), m_maxSize(size), m_usedSize(0)
{
	m_lock = new Lock("table");
	m_entries = (void **) AllocBoundedArray(sizeof(void *) * size);
	memset(m_entries, NULL, sizeof(void *) * size);
}

int Table::Alloc(void *object)
{
	if (m_usedSize >= m_maxSize)
	{
		DEBUG('T', "The table is full.");
		return -1;
	}
	m_lock->Acquire();
	// Using (loc = object % m_maxSize) as the hash function
	int t = (int) object;
	int loc = t % m_maxSize;
	while(NULL != *(m_entries + loc))
	{
		// Rehashing
		loc = ++t % m_maxSize;
	}
	*(m_entries + loc) = object;
	m_lock->Release();
	m_usedSize++;
	return loc;
}

void *Table::Get(int index)
{
	ASSERT(index >= 0 && index < m_maxSize);
	m_lock->Acquire();
	void *t = *(m_entries + index);
	m_lock->Release();
	return t;
}

void Table::Release(int index)
{
	m_lock->Acquire();
	*(m_entries + index) = NULL;
	m_lock->Release();
}
