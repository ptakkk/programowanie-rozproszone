#include "queue.h"
#include "main.h"

#include <stdio.h>
#include <stdlib.h>

void initQueue(int capacity)
{
    queue.elements = (QueueElement *)malloc(sizeof(QueueElement) * capacity);
    queue.capacity = capacity;
    queue.size = 0;
}

void addToQueue(int processId, int timestamp)
{
    if (queue.size == queue.capacity)
        return;

    // dodaj element na koniec kolejki
    queue.elements[queue.size].processId = processId;
    queue.elements[queue.size].timestamp = timestamp;
    queue.size++;

    // sortowanie (priorytet ma opiekun)
    for (int i = queue.size - 1; i > 0; i--)
    {
        if (
            getPersonType(queue.elements[i].processId) == Boss && getPersonType(queue.elements[i - 1].processId) != Boss ||
            (getPersonType(queue.elements[i].processId) != Boss && (queue.elements[i].timestamp < queue.elements[i - 1].timestamp ||
                                                                    (queue.elements[i].timestamp == queue.elements[i - 1].timestamp &&
                                                                     queue.elements[i].processId < queue.elements[i - 1].processId))))
        {
            // zamien elementy
            QueueElement temp = queue.elements[i];
            queue.elements[i] = queue.elements[i - 1];
            queue.elements[i - 1] = temp;
        }
        else
            break;
    }
}

void removeFromQueue(int processId)
{
    int indexToRemove = -1;
    for (int i = 0; i < queue.size; i++)
    {
        if (queue.elements[i].processId == processId)
        {
            indexToRemove = i;
            break;
        }
    }

    if (indexToRemove == -1)
        return;

    
    for (int i = indexToRemove; i < queue.size - 1; i++)
    {
        queue.elements[i] = queue.elements[i + 1];
    }

    queue.size--;
}

int getFirst()
{
    if (queue.size == 0)
        return -1;
    else
        return queue.elements[0].processId;
}

int getPosition(int processId)
{
    for (int i = queue.size - 1; i >= 0; i--)
        if (queue.elements[i].processId == processId)
            return i;

    return -1;
}

int getSmellTotalUntilPosition(int position)
{
    int sum = 0;
    for (int i = 0; i <= position; i++)
        sum += getPersonSmell(queue.elements[i].processId);

    return sum;
}

void printQueue()
{
    printf("[%d] Queue (size: %d): ", rank, queue.size);
    for (int i = 0; i < queue.size; i++)
        printf("(P: %d, T:%d) ", queue.elements[i].processId, queue.elements[i].timestamp);
    printf("\n");
}
