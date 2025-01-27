#ifndef __TRIPLE_BUFFER_SYSTEM_HPP
#define __TRIPLE_BUFFER_SYSTEM_HPP

#ifdef __cplusplus

#include "TripleBufferSystem.h"

class TripleBufferSystemClass
{
public:
    TripleBufferSystemClass() {}
    TripleBufferSystemClass(const unsigned int size)
    {
        init(size);
    }
    TripleBufferSystemClass(const unsigned int size, void (*beforeSwap)(void), void (*afterSwap)(void))
    {
        init(size);
        setFunc(beforeSwap, afterSwap);
    }
    ~TripleBufferSystemClass()
    {
        destroy();
    }

    int init(const unsigned int size)
    {
        return TripleBufferSystem_Init(&tbs, size);
    }

    void setFunc(void (*beforeSwap)(void), void (*afterSwap)(void))
    {
        TripleBufferSystem_setFunc(&tbs, beforeSwap, afterSwap);
    }

    void destroy()
    {
        TripleBufferSystem_Destroy(&tbs);
    }

    void swap()
    {
        TripleBufferSystem_Swap(&tbs);
    }

    int transfer()
    {
        return TripleBufferSystem_Transfer(&tbs);
    }

    size_t get(unsigned char *buffer, const size_t size)
    {
        return TripleBufferSystem_Get(&tbs, buffer, size);
    }

    size_t read(unsigned char *buffer, const size_t size, int *error)
    {
        return TripleBufferSystem_Read(&tbs, buffer, size, error);
    }

    void put(unsigned char data)
    {
        TripleBufferSystem_Put(&tbs, data);
    }

    void headMove()
    {
        TripleBufferSystem_HeadMove(&tbs);
    }

    unsigned char *nextWriteBuffer()
    {
        return TripleBufferSystem_NextWriteBuffer(tbs);
    }

private:
    TripleBufferSystem tbs;
};

#endif // __cplusplus
#endif // __TRIPLE_BUFFER_SYSTEM_HPP