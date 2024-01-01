#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define MAX_STR         64
#define MAX_RUNS        50
#define BOREDOM_MAX     100
#define C_TRUE          1
#define C_FALSE         0
#define HUNTER_WAIT     5000
#define GHOST_WAIT      600
#define NUM_HUNTERS     4
#define FEAR_MAX        10
#define LOGGING         C_TRUE

typedef enum EvidenceClass EvidenceClass;
typedef enum GhostClass GhostClass;

enum EvidenceClass { EMF, TEMPERATURE, FINGERPRINTS, SOUND, EV_COUNT, EV_UNKNOWN };
enum GhostClass { POLTERGEIST, BANSHEE, BULLIES, PHANTOM, GHOST_COUNT, GH_UNKNOWN };
enum LoggerDetails { LOG_FEAR, LOG_BORED, LOG_EVIDENCE, LOG_SUFFICIENT, LOG_INSUFFICIENT, LOG_UNKNOWN };

typedef struct Room RoomType;
typedef struct Ghost GhostType;
typedef struct House HouseType;
typedef struct RoomNode RoomNodeType;
typedef struct RoomsList RoomsListType; 
typedef struct Evidence EvidenceType; 
typedef struct EvidenceNode EvidenceNodeType;
typedef struct EvidencesList EvidencesListType;
typedef struct Hunter HunterType; 
typedef struct HunterNode HunterNodeType; 
typedef struct HunterList HunterListType;
typedef struct Thread ThreadType; 




struct Room{
    sem_t mutex; 
    char name[MAX_STR];
    RoomsListType *connected; 
    EvidencesListType *evidence; 
    HunterListType *hunters; 
    GhostType *ghost;  
};

struct House{
    sem_t mutex; 
    HunterListType *hunters; 
    RoomsListType *rooms; 
    EvidencesListType *evidences; 
};

struct RoomNode {
    RoomType *data;
    struct RoomNode *next; 
}; 

struct RoomsList{
    RoomNodeType *head; 
    RoomNodeType *tail; 
}; 

struct EvidenceNode{
    EvidenceClass data; 
    struct EvidenceNode *next;
}; 

struct EvidencesList{
    EvidenceNodeType *head;
    EvidenceNodeType *tail; 
}; 

struct Hunter{
    RoomType *room; 
    EvidenceClass evidence; 
    char name[MAX_STR]; 
    EvidencesListType *gatheredEvidence; 
    int boredem; 
    int fear; 
}; 

struct HunterNode {
    HunterType *data;
    struct HunterNode *next; 
}; 

struct HunterList{
    HunterNodeType *head; 
    HunterNodeType *tail; 
};

struct Ghost{
    GhostClass GhostClass;
    RoomType *room; 
    int bordem; 
};

struct Thread{
    HouseType *house;
    HunterType *hunter;
};


//Room Function 
RoomType* createRoom(const char* roomName); 
void initRoomsList(RoomsListType *list);
void connectRooms(RoomType *room1,  RoomType *room2);
void addRoom(RoomsListType *roomList, RoomType *room);

void initHuntersList(HunterListType *list);
void initEvidenceList(EvidencesListType *list);
void initHunter(RoomType *room, EvidenceClass evidence, char *name, HunterType **hunter);


void addHunter(HunterListType *hunterList, HunterType *hunter);
void hunterIntoRoom(RoomType *room, HunterType *hunter);
void initGhost(GhostClass type, RoomType *room, GhostType **ghost);
RoomNodeType* addGhostToRandomRoom(RoomsListType *room);
void addRandomEvidence (GhostType *currGhost);
void *ghostThread (void* arg);
void addEvidence(EvidencesListType *list, EvidenceNodeType *evidence);
void moveGhost(GhostType *currentGhost);
void populateRooms(HouseType* house);
void initHouse(HouseType *house);
void freeRoomList(RoomsListType *list);
void freeEvidenceData(EvidencesListType *list);
void freeEvidenceList(EvidencesListType *list);
void freeHunterData(HunterListType *list);
void freeHunter(HunterType *hunter);
void freeHunterList(HunterListType *list);
void freeRoomList(RoomsListType *list);
void *hunterThread (void* arg);
int addEvidenceHouse(HouseType *house, EvidenceClass evidence);
void collectingEvidence (HunterType *currHunter, HouseType *house);
RoomNodeType* HunterToRandomRoom(RoomsListType *room);
int removeHunterFromList(HunterListType *list, HunterType *hunterToRemove);
int moveHunter(HunterType *hunter);
int reviewEvidences (HouseType *house);

void getFearAndBoredom(HouseType *house);
int removeEvidenceFromRoom(EvidencesListType *list, EvidenceClass evidenceToRemove);
GhostClass getGhostClass(HouseType *house, GhostType *ghost);
void printEvidence(HouseType *house);
void printResults(HouseType *house, GhostType *ghost);

// Helper Utilies
int randInt(int,int);        // Pseudo-random number generator function
float randFloat(float, float);  // Pseudo-random float generator function
enum GhostClass randomGhost();  // Return a randomly selected a ghost type
void ghostToString(enum GhostClass, char*); // Convert a ghost type to a string, stored in output paremeter
void evidenceToString(enum EvidenceClass, char*); 
// Convert an evidence type to a string, stored in output parameter


// Logging Utilities
void l_hunterInit(char* name, enum EvidenceClass equipment);
void l_hunterMove(char* name, char* room);
void l_hunterReview(char* name, enum LoggerDetails reviewResult);
void l_hunterCollect(char* name, enum EvidenceClass evidence, char* room);
void l_hunterExit(char* name, enum LoggerDetails reason);
void l_ghostInit(enum GhostClass type, char* room);
void l_ghostMove(char* room);
void l_ghostEvidence(enum EvidenceClass evidence, char* room);
void l_ghostExit(enum LoggerDetails reason);
