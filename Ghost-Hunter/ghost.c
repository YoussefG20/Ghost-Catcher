#include "defs.h"
/*
    function: addGhostToRandomRoom(RoomsListType *room)
    purpose: add ghost to a random connected room
    in: RoomsListType *room: takes in a room that we will be looking to add the ghost too
    return: this will return a RoomNodeType that has the ghost in it
*/
RoomNodeType* addGhostToRandomRoom(RoomsListType *room){

    RoomNodeType *currNode = room->head; 
    int counter = 0;

    while (currNode!=NULL){
        currNode = currNode->next; 
        counter ++; 
    }

    int random = randInt(0,counter-1);

    RoomNodeType *temp = room->head;
    for (int i=0; i<random; i++){
        temp = temp->next; 
    }

    return temp; 
}


/*
    function: addRandomEvidence (GhostType *currGhost)
    purpose: add evidence from the ghost to the room the ghost is in 
    in: GhostType *currGhost: takes in a ghost that we will be looking to take evidence from and add it to the current room
    return: this will return nothing but will drop a random piece of evidence to the room
*/
void addRandomEvidence (GhostType *currGhost){

    sem_wait(&(currGhost->room->mutex));

    EvidenceNodeType *evidenceNode = (EvidenceNodeType*) malloc(sizeof(EvidenceNodeType));

     int typeOfEvidence[3];
    if (currGhost->GhostClass == POLTERGEIST) {
        typeOfEvidence[0] = 0;
        typeOfEvidence[1] = 1;
        typeOfEvidence[2] = 2;
    }   
    else if (currGhost->GhostClass == BANSHEE) {
        typeOfEvidence[0] = 0;
        typeOfEvidence[1] = 1;
        typeOfEvidence[2] = 3;
    } 
    else if (currGhost->GhostClass == BULLIES) {
        typeOfEvidence[0] = 0;
        typeOfEvidence[1] = 2;
        typeOfEvidence[2] = 3;
    } 
    else if (currGhost->GhostClass == PHANTOM) {
        typeOfEvidence[0] = 1;
        typeOfEvidence[1] = 2;
        typeOfEvidence[2] = 3;
    }


    int random = typeOfEvidence[randInt(0,3)];

    
    evidenceNode->data = random;
    evidenceNode->next = NULL;
    addEvidence(currGhost->room->evidence, evidenceNode);
    l_ghostEvidence(evidenceNode->data, currGhost->room->name);

    
    sem_post(&(currGhost->room->mutex));
}

/*
    function: addEvidence(EvidencesListType *list, EvidenceNodeType *evidence)
    purpose: add evidence to a list
    in: EvidencesListType *list: evidence list to be added to 
    in: EvidenceNodeType *evidence: evidence to be added
    return: void, wil just add the evidence to the list
*/
void addEvidence(EvidencesListType *list, EvidenceNodeType *evidence){
    
    evidence->next = NULL; 

    if (list->tail == NULL){
        list->head = evidence; 
        list->tail = evidence; 
    }
    else{
        list->tail->next = evidence;
        list->tail = evidence;
    }
}

/*
    function: moveGhost(GhostType *currentGhost)
    purpose: move ghost to a random connected room
    in: GhostType *currentGhost: takes in a ghost that we will be looking to move
    return:void, will move the ghost to a random connected room
*/
void moveGhost(GhostType *currentGhost){
    RoomNodeType *traverseRoomNode = currentGhost->room->connected->head;
    int sizeCounter = 0;
    while(traverseRoomNode != NULL){
        sizeCounter++;
        traverseRoomNode = traverseRoomNode->next;
    }
    
    int randomNodeInt = randInt(0,sizeCounter);
    RoomNodeType *tempRoom = currentGhost->room->connected->head;
    for(int i = 0; i < randomNodeInt; i++) {
        tempRoom = tempRoom->next;
    }

    currentGhost->room->ghost = NULL;
    currentGhost->room = tempRoom->data;
    currentGhost->room->ghost = currentGhost;
}

/*
    function: *ghostThread (void* arg)
    purpose: create the ghost thread
    in: void* arg: takes in a voidi argument
    return: void, will create the ghost thread and run its operations
*/
void *ghostThread (void* arg){

    GhostType *ghost = (GhostType*) arg; 

    while (ghost->bordem < BOREDOM_MAX){
        usleep(GHOST_WAIT);

        if (ghost->room->hunters->head != NULL){
            
            ghost->bordem = 0; 

            int nextMove = randInt(0,2);

            if (nextMove == 1){
                addRandomEvidence(ghost);
            }

        }
        else {
            
            int nextMove = randInt(0,3); 
            ghost->bordem++; 

            if (nextMove == 0){
                moveGhost(ghost);
                l_ghostMove(ghost->room->name);
            }
            else if (nextMove==1){
                addRandomEvidence(ghost);
            }
        }
    }
    usleep(GHOST_WAIT);
    l_ghostExit(LOG_BORED);

    return NULL; 
}

/*
    function: freeEvidenceList(EvidencesListType *list)
    purpose: free the memory in evidence list
    in: EvidencesListType *list: takes in an evidence list
    return: void, will free the memory allocated
*/
void freeEvidenceList(EvidencesListType *list){
     EvidenceNodeType *tempNode;
	while(list->head != NULL){
		tempNode = list->head;
		list->head = list->head->next;
		free(tempNode);
	}
    free(list);
}

