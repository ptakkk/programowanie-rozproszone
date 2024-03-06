#ifndef QUEUE_H
#define QUEUE_H

typedef struct
{
    int processId;
    int timestamp;
} QueueElement;

typedef struct
{
    QueueElement *elements;
    int capacity;
    int size;
} Queue;

void initQueue(int capacity);
void addToQueue(int processId, int timestamp);
void removeFromQueue(int processId);
int getFirst();
int getPosition(int processId);
int getSmellTotalUntilPosition(int position);
void printQueue();

#endif // QUEUE_H
