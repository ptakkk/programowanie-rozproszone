#ifndef UTILH
#define UTILH
#include "main.h"

#define MAX_CAPACITY 4
#define MAX_SMELL_TOTAL 22

extern pthread_mutex_t mutex;

typedef enum
{
    Boss = 0,
    Otaku = 1,
} person_type;

/* typ pakietu */
typedef struct
{
    int ts; /* timestamp (zegar lamporta */
    int src;

    person_type personType; /* przykładowe pole z danymi; można zmienić nazwę na bardziej pasującą */
} packet_t;
/* packet_t ma trzy pola, więc NITEMS=3. Wykorzystane w inicjuj_typ_pakietu */
#define NITEMS 2

/* Typy wiadomości */
/* TYPY PAKIETÓW */
#define ACK 1
#define REQUEST 2
#define RELEASE 3
#define APP_PKT 4
#define FINISH 5

extern MPI_Datatype MPI_PAKIET_T;
void initPacketType();

void sendPacket(int destination, int tag);

void broadcast(int tag);

int getPersonSmell(int processId);

person_type getPersonType(int processId);

typedef enum
{
    StateReady,
    StateWantPlace,
    StateInPlace,
    StateInPlaceForever,
    StateFinish,
} state_t;
extern state_t state;
extern pthread_mutex_t mutex;
/* zmiana stanu, obwarowana muteksem */
void changeState(state_t);
#endif
