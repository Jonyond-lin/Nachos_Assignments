#ifndef BOUNDEDBUFFER_H
#define BOUNDEDBUFFER_H

#include "synch.h" 
#include "utility.h"
class BoundedBuffer {
   public:
     // create a bounded buffer with a limit of 'maxsize' bytes
     BoundedBuffer(char *name, int maxsize);
     ~BoundedBuffer();

     // read 'size' bytes from the bounded buffer, storing into 'data'.
     // ('size' may be greater than 'maxsize')
     void Read(void *data, int size);
     
     // write 'size' bytes from 'data' into the bounded buffer.
     // ('size' may be greater than 'maxsize')
     void Write(void *data, int size);
   private:
	 char *debugName;
	 void *m_buffer;
	 int m_maxSize;
	 Lock *m_lock;
	 Condition *m_notEmpty, *m_notFull;
	 int m_usedSize;
     // ???
};
#endif
