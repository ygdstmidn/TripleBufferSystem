#if (0)

#include <stdio.h>
#include "TripleBufferSystem.hpp"

void enable_irq(void)
{
    printf("enable_irq\n");
}
void disable_irq(void)
{
    printf("disable_irq\n");
}

int main(void)
{
    TripleBufferSystemClass tbs;
    printf("Init:%d\n", tbs.init(10));
    tbs.setFunc(disable_irq, enable_irq);

    // standard usage
    tbs.put('a');
    tbs.put('b');
    tbs.put('c');
    tbs.swap();
    printf("transfer:%d\n", tbs.transfer());
    unsigned char tempBuffer[10];
    size_t tempBufferSize = tbs.get(tempBuffer, 10);
    for (size_t i = 0; i < tempBufferSize; i++)
    {
        printf("%c\n", tempBuffer[i]);
    }

    // head move
    *tbs.nextWriteBuffer() = 'd';
    tbs.headMove();
    *tbs.nextWriteBuffer() = 'e';
    tbs.headMove();
    *tbs.nextWriteBuffer() = 'f';
    tbs.headMove();
    tbs.swap();
    tbs.transfer();
    tempBufferSize = tbs.get(tempBuffer, 10);
    for (size_t i = 0; i < tempBufferSize; i++)
    {
        printf("%c\n", tempBuffer[i]);
    }

    // read function
    for (int i = 0; i < 5; i++) // input 5
    {
        tbs.put('g' + i);
    }
    int tbsReadError = 0;
    tempBufferSize = tbs.read(tempBuffer, 3, &tbsReadError); // erase 3 = 2
    for (size_t i = 0; i < tempBufferSize; i++)
    {
        printf("%c\n", tempBuffer[i]);
    }
    for (int i = 0; i < 9; i++) // input 9 = 11
    {
        tbs.put('l' + i);
    }
    tempBufferSize = tbs.read(tempBuffer, 3, &tbsReadError); // erase 3 = 8
    for (size_t i = 0; i < tempBufferSize; i++)
    {
        printf("%c\n", tempBuffer[i]);
    }
    for (int i = 0; i < 9; i++) // input 9 = 17
    {
        tbs.put('u' + i);
    }
    tempBufferSize = tbs.read(tempBuffer, 3, &tbsReadError); // erase 3 = 14
    for (size_t i = 0; i < tempBufferSize; i++)
    {
        printf("%c\n", tempBuffer[i]);
    }
    tempBufferSize = tbs.read(tempBuffer, 3, &tbsReadError); // erase 3 = 11//not swap
    for (size_t i = 0; i < tempBufferSize; i++)
    {
        printf("%c\n", tempBuffer[i]);
    }
    tempBufferSize = tbs.read(tempBuffer, 10, &tbsReadError); // erase 10 = 1
    for (size_t i = 0; i < tempBufferSize; i++)
    {
        printf("%c\n", tempBuffer[i]);
    }
    tempBufferSize = tbs.read(tempBuffer, 10, &tbsReadError); // erase 10 = 0
    for (size_t i = 0; i < tempBufferSize; i++)
    {
        printf("%c\n", tempBuffer[i]);
    }

    // write buffer is full
    for (int i = 0; i < 10; i++)
    {
        tbs.put('a' + i);
    }
    tbs.swap();
    printf("transfer:%d\n", tbs.transfer());

    // read buffer is full
    tbs.destroy();
    tbs.init(10);
    tbs.setFunc(disable_irq, enable_irq);
    for (int i = 0; i < 9; i++)
    {
        tbs.put('a' + i);
    }
    tbs.swap();
    tbs.transfer();
    for (int i = 0; i < 9; i++)
    {
        tbs.put('a' + i);
    }
    tbs.swap();
    printf("transfer:%d\n", tbs.transfer());

    return 0;
}

#endif