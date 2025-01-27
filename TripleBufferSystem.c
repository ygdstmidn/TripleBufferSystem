#include "TripleBufferSystem.h"

int TripleBufferSystem_Init(TripleBufferSystem *tbs, const unsigned int size)
{
    tbs->writeBufferIndex = 0;
    tbs->enable_irq = NULL;
    tbs->disable_irq = NULL;
    int ErrorCheck = 0;
    ErrorCheck |= RingBuffer_Init(&tbs->writeBuffer[0], size);
    ErrorCheck |= RingBuffer_Init(&tbs->writeBuffer[1], size);
    ErrorCheck |= RingBuffer_Init(&tbs->readBuffer, size);
    return ErrorCheck;
}

void TripleBufferSystem_set_irq(TripleBufferSystem *tbs, void (*enable_irq)(void), void (*disable_irq)(void))
{
    tbs->enable_irq = enable_irq;
    tbs->disable_irq = disable_irq;
}

void TripleBufferSystem_Destroy(TripleBufferSystem *tbs)
{
    RingBuffer_Destroy(&tbs->writeBuffer[0]);
    RingBuffer_Destroy(&tbs->writeBuffer[1]);
    RingBuffer_Destroy(&tbs->readBuffer);
}

void TripleBufferSystem_Swap(TripleBufferSystem *tbs)
{
    tbs->writeBufferIndex = !tbs->writeBufferIndex;
}

int TripleBufferSystem_Transfer(TripleBufferSystem *tbs)
{
    RingBuffer *wb = &tbs->writeBuffer[!tbs->writeBufferIndex];
    RingBuffer *rb = &tbs->readBuffer;
    if (wb->isntFull == 0)
    {
        return 1;
    }
    while (wb->head != wb->tail)
    {
        const unsigned int nextHead = (rb->head + 1) % rb->size;
        if (nextHead == rb->tail)
        {
            return 2;
        }
        RingBuffer_Put(rb, RingBuffer_Get(wb));
    }
    return 0;
}

size_t TripleBufferSystem_Get(TripleBufferSystem *tbs, unsigned char *buffer, const size_t size)
{
    size_t i = 0;
    while (i < size && tbs->readBuffer.head != tbs->readBuffer.tail)
    {
        buffer[i++] = RingBuffer_Get(&tbs->readBuffer);
    }
    return i;
}

size_t TripleBufferSystem_Read(TripleBufferSystem *tbs, unsigned char *buffer, const size_t size, int *error)
{
    *error = 0;
    size_t i = 0;
    int transferError;

    transferError = TripleBufferSystem_Transfer(tbs); // 書き込みバッファから読み込みバッファに移す

    i += TripleBufferSystem_Get(tbs, buffer + i, size - i); // 元々入っているぶんを取り出し

    transferError = TripleBufferSystem_Transfer(tbs); // 書き込みバッファから読み込みバッファに移す
    if (transferError == 1)
    {
        *error = 1;
        return 0;
    }
    else if (transferError == 0) // 全部移すことができた→swap
    {
        tbs->disable_irq();
        TripleBufferSystem_Swap(tbs);
        tbs->enable_irq();
        transferError = TripleBufferSystem_Transfer(tbs); // 書き込みバッファから読み込みバッファに移す
        if (transferError == 1)
        {
            *error = 1;
            return 0;
        }
    }

    i += TripleBufferSystem_Get(tbs, buffer + i, size - i);
    if (transferError == 2) // 読み込みバッファがいっぱいだった
    {
        transferError = TripleBufferSystem_Transfer(tbs);
    }

    *error = transferError;
    return i;
}

void TripleBufferSystem_Put(TripleBufferSystem *tbs, const unsigned char data)
{
    RingBuffer_Put(&tbs->writeBuffer[tbs->writeBufferIndex], data);
}

void TripleBufferSystem_HeadMove(TripleBufferSystem *tbs)
{
    RingBuffer_HeadMove(&tbs->writeBuffer[tbs->writeBufferIndex]);
}