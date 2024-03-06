#include "main.h"
#include "watek_glowny.h"
#include "queue.h"

void mainLoop()
{
	switch (state)
	{
	case StateReady:
	{
		int requiredPercent = getPersonType(rank) == Boss ? 15 : 25;

		srandom(time(NULL) * (rank + 1));
		if (random() % 100 < requiredPercent)
		{
			pthread_mutex_lock(&mutex);

			state = StateWantPlace;
			ackCount = 0;
			addToQueue(rank, ++currentTime);
			println("Czas %6d | %s chce do pokoju (pozycja: %d)", currentTime, getPersonType(rank) == Boss ? "opiekun" : "otaku", getPosition(rank));
			broadcast(REQUEST);

			pthread_mutex_unlock(&mutex);
		}

		break;
	}

	case StateWantPlace:
		pthread_mutex_lock(&mutex);

		int pos = getPosition(rank);

		if ((ackCount == size - 1 || pos < MAX_CAPACITY))
		{
			if (getPersonType(rank) == Boss)
			{
				state = StateInPlaceForever;
				println("Czas %6d | wchodzi opiekun (pozycja: %d)", currentTime, getPosition(rank));
			}
			else
			{
				int first = getFirst();

				if (first != -1 && getPersonType(first) == Boss && getSmellTotalUntilPosition(pos) <= MAX_SMELL_TOTAL)
				{
					state = StateInPlace;
					println("Czas %6d | wchodzi otaku [+%d cuchow] (pozycja: %d)", currentTime, getPersonSmell(rank), getPosition(rank));
				}
			}
		}

		pthread_mutex_unlock(&mutex);

		break;

	case StateInPlace:
		if (getPersonType(rank) == Boss)
			break;

		srandom(time(NULL) * (rank + 1));
		sleep(3 + random() % 10);

		pthread_mutex_lock(&mutex); 

		println("Czas %6d | otaku wyszedl [-%d cuchow]", currentTime, getPersonSmell(rank));

		state = StateReady;
		removeFromQueue(rank);
		currentTime++;
		broadcast(RELEASE);

		pthread_mutex_unlock(&mutex);

		break;

	default:
		break;
	}

	sleep(SEC_IN_STATE);
}
