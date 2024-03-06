#include "main.h"
#include "util.h"
MPI_Datatype MPI_PAKIET_T;

state_t state = StateReady;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct tagNames_t
{
    const char *name;
    int tag;
} tagNames[] = {{"pakiet aplikacyjny", APP_PKT}, {"finish", FINISH}, {"potwierdzenie", ACK}, {"prośbę o sekcję krytyczną", REQUEST}, {"zwolnienie sekcji krytycznej", RELEASE}};

const char *const tag2string(int tag)
{
    for (int i = 0; i < sizeof(tagNames) / sizeof(struct tagNames_t); i++)
    {
        if (tagNames[i].tag == tag)
            return tagNames[i].name;
    }
    return "<unknown>";
}

int getPersonSmell(int processId)
{
    return getPersonType(processId) == Boss ? 0 : processId * 2;
}

person_type getPersonType(int processId)
{
    return processId < 2 ? Boss : Otaku;
}

void initPacketType()
{
    int blocklengths[NITEMS] = {1, 1};
    MPI_Datatype typy[NITEMS] = {MPI_INT, MPI_INT};

    MPI_Aint offsets[NITEMS];
    offsets[0] = offsetof(packet_t, ts);
    offsets[1] = offsetof(packet_t, src);

    MPI_Type_create_struct(NITEMS, blocklengths, offsets, typy, &MPI_PAKIET_T);

    MPI_Type_commit(&MPI_PAKIET_T);
}

void sendPacket(int destination, int tag)
{
    packet_t *pkt = malloc(sizeof(packet_t));

    pkt->src = rank;
    pkt->ts = currentTime;

    MPI_Send(pkt, 1, MPI_PAKIET_T, destination, tag, MPI_COMM_WORLD);
    debug("Wysyłam %s do %d\n z t=%d", tag2string(tag), destination, currentTime);

    free(pkt);
}

void broadcast(int tag)
{
    for (int i = 0; i <= size - 1; i++)
        if (i != rank)
            sendPacket(i, tag);
}
