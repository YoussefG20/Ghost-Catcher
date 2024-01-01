#include "defs.h"

/*
    function: initRoomsList(RoomsListType *list)
    purpose: intializes the room list head and tail to NUll; 
    in: RoomsListType *list: take in the list that we are intializing 
*/
void initRoomsList(RoomsListType *list){
    list->head = NULL;
    list->tail = NULL;
} 

/*
    function: initHunterList(HunterListType *list)
    purpose: intializes the hunter list head and tail to NUll; 
    in: HunterListType *list: take in the list that we are intializing 
*/
void initHuntersList(HunterListType *list){
    list->head = NULL;
    list->tail = NULL; 
}

/*
    function: initEvidenceList(EvidenceListType *list)
    purpose: intializes the evidnece list head and tail to NUll; 
    in: EvidenceListType *list: take in the list that we are intializing 
*/
void initEvidenceList(EvidencesListType *list){
    list->head = NULL; 
    list->tail = NULL;
}

/*
    function: initHunter(RoomType *room, EvidenceClass evidence, char *name, HunterType **hunter)
    purpose: intializes the hunter struct; 
    in: RoomType *room: intializes the room in the hunter struct 
    in: EvidenceClass evidence: intializes the evidence in the hunter struct 
    in: char *name: intializes the name of the hunter 
    in/out: HunterType **hunter: take in the hunter that we are intializing and intialize all the atrributes to it   
*/
void initHunter(RoomType *room, EvidenceClass evidence, char *name, HunterType **hunter){

    *hunter= (HunterType*) malloc(sizeof(HunterType));
    strcpy((*hunter)->name,name);
    (*hunter)->room = room; 
    (*hunter)->evidence = evidence; 
    (*hunter)->gatheredEvidence = (EvidencesListType*) malloc(sizeof(EvidencesListType));
    initEvidenceList((*hunter)->gatheredEvidence);
    (*hunter)->boredem = 0;
    (*hunter)->fear = 0;
    l_hunterInit((*hunter)->name,(*hunter)->evidence);
}

/*
    function: initGhost(GhostClass type, RoomType *room, GhostType **ghost)
    purpose: intializes the ghost
    in: GhostClass type: intializes the ghosttype of this ghost 
    in: RoomType *room: intializes the room that the ghost is in
    in/out: GhostType **ghost: take in the ghost that we are intializing and initilize all the attributes to it 
*/
void initGhost(GhostClass type, RoomType *room, GhostType **ghost){
    (*ghost) = (GhostType*)malloc(sizeof(GhostType));
    (*ghost)->GhostClass = type; 
    (*ghost)->room = room; 
    (*ghost)->bordem = 0;
    l_ghostInit((*ghost)->GhostClass,(*ghost)->room->name);
}

/*
    function: initHouse(HouseType *house)
    purpose: intializes the house
    in: HouseType *house: this takes in the house that we are intializing 
*/
void initHouse(HouseType *house){
    
    sem_init(&(house->mutex),0,1);

    house->evidences = (EvidencesListType*) malloc(sizeof(EvidencesListType));
    initEvidenceList(house->evidences);

    house->hunters = (HunterListType*) malloc(sizeof(HunterListType));
    initHuntersList(house->hunters);

    house->rooms = (RoomsListType*) malloc(sizeof(RoomsListType));
    initRoomsList(house->rooms);
}



