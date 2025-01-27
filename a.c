#if (0)

#include <stdio.h>
#include "TripleBufferSystem.h"

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
    TripleBufferSystem tbs;
    printf("Init:%d\n", TripleBufferSystem_Init(&tbs, 10));
    TripleBufferSystem_set_irq(&tbs, enable_irq, disable_irq);

    // standard usage
    TripleBufferSystem_Put(&tbs, 'a');
    TripleBufferSystem_Put(&tbs, 'b');
    TripleBufferSystem_Put(&tbs, 'c');
    TripleBufferSystem_Swap(&tbs);
    printf("Transfer:%d\n", TripleBufferSystem_Transfer(&tbs));
    char tempBuffer[10];
    size_t tempBufferSize = TripleBufferSystem_Get(&tbs, tempBuffer, 10);
    for (size_t i = 0; i < tempBufferSize; i++)
    {
        printf("%c\n", tempBuffer[i]);
    }

    // head move
    *TripleBufferSystem_NextWriteBuffer(tbs) = 'd';
    TripleBufferSystem_HeadMove(&tbs);
    *TripleBufferSystem_NextWriteBuffer(tbs) = 'e';
    TripleBufferSystem_HeadMove(&tbs);
    *TripleBufferSystem_NextWriteBuffer(tbs) = 'f';
    TripleBufferSystem_HeadMove(&tbs);
    TripleBufferSystem_Swap(&tbs);
    TripleBufferSystem_Transfer(&tbs);
    tempBufferSize = TripleBufferSystem_Get(&tbs, tempBuffer, 10);
    for (size_t i = 0; i < tempBufferSize; i++)
    {
        printf("%c\n", tempBuffer[i]);
    }

    // read function
    for (int i = 0; i < 5; i++) // input 5
    {
        TripleBufferSystem_Put(&tbs, 'g' + i);
    }
    int tbsReadError = 0;
    tempBufferSize = TripleBufferSystem_Read(&tbs, tempBuffer, 3, &tbsReadError); // erase 3 = 2
    for (size_t i = 0; i < tempBufferSize; i++)
    {
        printf("%c\n", tempBuffer[i]);
    }
    for (int i = 0; i < 9; i++) // input 9 = 11
    {
        TripleBufferSystem_Put(&tbs, 'l' + i);
    }
    tempBufferSize = TripleBufferSystem_Read(&tbs, tempBuffer, 3, &tbsReadError); // erase 3 = 8
    for (size_t i = 0; i < tempBufferSize; i++)
    {
        printf("%c\n", tempBuffer[i]);
    }
    for (int i = 0; i < 9; i++) // input 9 = 17
    {
        TripleBufferSystem_Put(&tbs, 'u' + i);
    }
    tempBufferSize = TripleBufferSystem_Read(&tbs, tempBuffer, 3, &tbsReadError); // erase 3 = 14
    for (size_t i = 0; i < tempBufferSize; i++)
    {
        printf("%c\n", tempBuffer[i]);
    }
    tempBufferSize = TripleBufferSystem_Read(&tbs, tempBuffer, 3, &tbsReadError); // erase 3 = 11//not swap
    for (size_t i = 0; i < tempBufferSize; i++)
    {
        printf("%c\n", tempBuffer[i]);
    }
    tempBufferSize = TripleBufferSystem_Read(&tbs, tempBuffer, 10, &tbsReadError); // erase 10 = 1
    for (size_t i = 0; i < tempBufferSize; i++)
    {
        printf("%c\n", tempBuffer[i]);
    }
    tempBufferSize = TripleBufferSystem_Read(&tbs, tempBuffer, 10, &tbsReadError); // erase 10 = 0
    for (size_t i = 0; i < tempBufferSize; i++)
    {
        printf("%c\n", tempBuffer[i]);
    }

    // write buffer is full
    for (int i = 0; i < 10; i++)
    {
        TripleBufferSystem_Put(&tbs, 'a' + i);
    }
    TripleBufferSystem_Swap(&tbs);
    printf("Transfer:%d\n", TripleBufferSystem_Transfer(&tbs));

    // read buffer is full
    TripleBufferSystem_Destroy(&tbs);
    TripleBufferSystem_Init(&tbs, 10);
    for (int i = 0; i < 9; i++)
    {
        TripleBufferSystem_Put(&tbs, 'a' + i);
    }
    TripleBufferSystem_Swap(&tbs);
    TripleBufferSystem_Transfer(&tbs);
    for (int i = 0; i < 9; i++)
    {
        TripleBufferSystem_Put(&tbs, 'a' + i);
    }
    TripleBufferSystem_Swap(&tbs);
    printf("Transfer:%d\n", TripleBufferSystem_Transfer(&tbs));

    return 0;
}

#endif