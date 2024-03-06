#include "main.h"
#include "util.h"
#include "watek_komunikcyjny.h"
#include "queue.h"

#define max(a, b) ((a) > (b) ? (a) : (b))

void *startKomWatek(void *ptr)
{
    MPI_Status status;
    packet_t pakiet;

    while (state != StateFinish)
    {
        MPI_Recv(&pakiet, 1, MPI_PAKIET_T, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        int src = pakiet.src;
        int receivedTime = pakiet.ts;

        // Aktualizuj czas Lamporta
        pthread_mutex_lock(&mutex);
        currentTime = max(currentTime, receivedTime) + 1;
        pthread_mutex_unlock(&mutex);

        switch (status.MPI_TAG)
        {
        case REQUEST:
            pthread_mutex_lock(&mutex);

            currentTime++;
            sendPacket(src, ACK);
            addToQueue(src, receivedTime);

            pthread_mutex_unlock(&mutex);

            break;

        case ACK:
            ackCount++;

            break;

        case RELEASE:
            pthread_mutex_lock(&mutex);
            removeFromQueue(src);
            pthread_mutex_unlock(&mutex);

            break;

        default:
            break;
        }
    }

    return NULL;
}
