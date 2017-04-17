#include "BoundedBuffer.h"
#include "utility.h"
#include "string.h"

#define REMAINING_SIZE (m_maxSize - m_usedSize)

BoundedBuffer::BoundedBuffer(char *name, int maxsize):debugName(name), m_maxSize(maxsize), m_usedSize(0)
{
	m_lock = new Lock("buffer");
	m_notEmpty = new Condition("not empty");
	m_notFull = new Condition("not full");
	m_buffer = (void *) AllocBoundedArray(sizeof(void *) * maxsize);
}

BoundedBuffer::~BoundedBuffer()
{
	DeallocBoundedArray((char *) m_buffer, sizeof(void *) * m_maxSize);
	delete m_lock;
	delete m_notEmpty;
	delete m_notFull;
}

void BoundedBuffer::Read(void *data, int size)
{
	// Notice that 'size' may be greater than 'm_maxSize'
	if (0 == size)
	{
		DEBUG('b', "Moving 0 byte from a bounded buffer.\n");
		return;
	}
	m_lock->Acquire();
	while(0 == m_usedSize)
	{
		m_notEmpty->Wait(m_lock);
	}
	if (size > m_usedSize)
	{
		DEBUG('b', "The size of the data to be read (%d byte) is larger than the data size in the buffer (%d byte).\n", size, m_usedSize);
		size = REMAINING_SIZE;
	}
	memcpy(data, (char *) m_buffer + (m_usedSize - size), size);
	DEBUG('b', "Move the last %d byte data from the buffer %x to %x.\n", size, m_buffer, data);
	m_usedSize -= size;
	m_notFull->Signal(m_lock);
	m_lock->Release();
}

void BoundedBuffer::Write(void *data, int size)
{
	if(0 == size)
	{
		DEBUG('b', "Writing 0 byte into a bounded buffer.\n");
		return;
	}
	m_lock->Acquire();
	while(m_usedSize == m_maxSize)
	{
		m_notFull->Wait(m_lock);
	}
	if(size > REMAINING_SIZE)
	{
		DEBUG('b', "The size of the data to be written (%d byte) is larger than the size of the remaining data in the buffer (% byte).\n", size, REMAINING_SIZE);
		size = REMAINING_SIZE;
	}
	memcpy((char *) m_buffer + m_usedSize, data, size);
	DEBUG('b', "Move %d bytes data from %x to the buffer %x.\n", size, data, m_buffer);
	m_usedSize += size;
	m_notEmpty->Signal(m_lock);
	m_lock->Release();
}
