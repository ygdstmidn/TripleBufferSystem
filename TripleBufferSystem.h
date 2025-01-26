#ifndef __TRIPLEBUFFERSYSTEM_H
#define __TRIPLEBUFFERSYSTEM_H

#include "RingBuffer/RingBuffer.h"

typedef struct
{
    RingBuffer writeBuffer[2];
    RingBuffer readBuffer;
    unsigned int writeBufferIndex; // 0 or 1
    void (*enable_irq)(void);
    void (*disable_irq)(void);
} TripleBufferSystem;

int TripleBufferSystem_Init(TripleBufferSystem *tbs, unsigned int size);
void TripleBufferSystem_set_irq(TripleBufferSystem *tbs, void (*enable_irq)(void), void (*disable_irq)(void));
void TripleBufferSystem_Destroy(TripleBufferSystem *tbs);
void TripleBufferSystem_Swap(TripleBufferSystem *tbs);
int TripleBufferSystem_Transfer(TripleBufferSystem *tbs);
size_t TripleBufferSystem_Get(TripleBufferSystem *tbs, unsigned char *buffer, const size_t size);
size_t TripleBufferSystem_Read(TripleBufferSystem *tbs, unsigned char *buffer, const size_t size, int *error);
void TripleBufferSystem_Put(TripleBufferSystem *tbs, unsigned char data);
void TripleBufferSystem_HeadMove(TripleBufferSystem *tbs);

#define TripleBufferSystem_NextWriteBuffer(tbs) ((tbs).writeBuffer[(tbs).writeBufferIndex].buffer + (tbs).writeBuffer[(tbs).writeBufferIndex].head)

#endif // __TRIPLEBUFFERSYSTEM_H