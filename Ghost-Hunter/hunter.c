#include "defs.h"

/*
    function: addHunter(HunterListType *hunterList, HunterType *hunter)
    purpose: adds a hunter to a hunterlist 
    in: HunterListType *hunterList: take in the list that we are putting the hunter in 
    in: HuterType *hunter: taking in the hunter that we will be adding to the list 
*/
void addHunter(HunterListType *hunterList, HunterType *hunter){

    HunterNodeType *newNode = malloc(sizeof(HunterNodeType));

    newNode->data = hunter; 
    newNode->next = NULL; 

    if (hunterList->tail == NULL){
        hunterList->head = newNode; 
        hunterList->tail = newNode; 
    }
    else{
        hunterList->tail->next = newNode;
        hunterList->tail = newNode;
    }
}

/*
    function: addEvidenceHouse(HouseType *house, EvidenceClass evidence)
    purpose: add evidence to the shared evidence list in the house if the evidence wasnt added this function checks if it should be added 
    in: HouseType *house: takes in a house that we will be looking to add the evidnece too
    in: EvidenceClass evidence: takes in the evidence that will be looking foir in the shared evidence list 
    return: this will return a boolean that will either say the evidence is needed to be added or not 
*/
int addEvidenceHouse(HouseType *house, EvidenceClass evidence) {
    int isUnique = C_TRUE;

    EvidenceNodeType *tempEvidence = house->evidences->head;

    while (tempEvidence != NULL) {
        if (tempEvidence->data == evidence) {
            isUnique = C_FALSE; 
            break;
        }
        tempEvidence = tempEvidence->next;
    }

    if (isUnique) {
        EvidenceNodeType *houseEvidence = malloc(sizeof(EvidenceNodeType));
        houseEvidence->data = evidence;
        houseEvidence->next = NULL;
        
        addEvidence(house->evidences, houseEvidence);
    }

    return C_TRUE;
}

/*
    function: collectingEvidence(HunterType *currHunter, HouseType *house)
    purpose: this allows the hunter to collect evidence from the room if he can and adds it to the shared evidence in the house 
    in/out: HunterType *currHunter: this takes in the hunter that will be trying to collect evidence 
    int/out: HouseType *house: this will take the house that contains the shared evidence list; 
*/
void collectingEvidence(HunterType *currHunter, HouseType *house) {
    EvidenceNodeType *tempEvidence = currHunter->room->evidence->head;

    while (tempEvidence != NULL) {
        if (tempEvidence->data == currHunter->evidence) {
            EvidenceClass evidenceToAdd = tempEvidence->data;

            EvidenceNodeType *hunterEvidence = malloc(sizeof(EvidenceNodeType));
            hunterEvidence->data = evidenceToAdd;
            hunterEvidence->next = NULL;

            removeEvidenceFromRoom(currHunter->room->evidence, evidenceToAdd);
            addEvidence(currHunter->gatheredEvidence, hunterEvidence);

            l_hunterCollect(currHunter->name, evidenceToAdd, currHunter->room->name);

            sem_wait(&(house->mutex));
            addEvidenceHouse(house, evidenceToAdd);
            sem_post(&(house->mutex));

            break;
        }

        tempEvidence = tempEvidence->next;
    }
}


/*
    function: HunterToRandomRoom(RoomsListType *room)
    purpose: this will get a random room so that the hunter could move to it; 
    in: RoomsListType *room: this will take a roomslist that we are itterating through to get a random room 
*/
RoomNodeType* HunterToRandomRoom(RoomsListType *room){

    RoomNodeType *currNode = room->head; 
    int counter = 0;

    while (currNode!=NULL){
        currNode = currNode->next; 
        counter ++; 
    }

    int random = randInt(0,counter);

    RoomNodeType *temp = room->head;
    for (int i=0; i<random; i++){
        temp = temp->next; 
    }

    return temp; 
}


/*
    function:  removeHunterFromList(HunterListType *list, HunterType *hunterToRemove) 
    purpose: this will remove a hunter from the linked list 
    in/out: HunterListType *list: this is the hunterlist that the hunter will be removed from 
    in: HunterType *hunterToRemove: this is the the hunter that needs to be removed from the list 
    returns: wether the hunter has been removed or not
*/
int removeHunterFromList(HunterListType *list, HunterType *hunterToRemove) {
    if (list == NULL || hunterToRemove == NULL) {
        return C_FALSE; 
    }

    HunterNodeType *currentNode = list->head;
    HunterNodeType *previousNode = NULL;

    while (currentNode != NULL) {
        if (currentNode->data == hunterToRemove) {
            if (previousNode == NULL) {
                list->head = currentNode->next;
            } 
            else {
                previousNode->next = currentNode->next;
            }

                
            if (currentNode == list->tail) {
                list->tail = previousNode;
            }

            free(currentNode); 
            return C_TRUE;

        }
        previousNode = currentNode;
        currentNode = currentNode->next;
    }

    return C_FALSE; 
}

/*
    function: moveHunter(HunterType *hunter)
    purpose: this function moves the hunter from one room to another
    in: HunterType *hunter: this takes in the hunter that we will be moving 
    returns: int: weather the hunter moved:
*/
int moveHunter(HunterType *hunter){
     
    RoomType *newRoom = HunterToRandomRoom(hunter->room->connected)->data;

    RoomType *currRoom = hunter->room;

    while(1) {
        if(sem_trywait(&(currRoom->mutex)) != 0){
            continue;
        }

        if(sem_trywait(&(newRoom->mutex)) != 0){
            sem_post(&(currRoom->mutex));
            continue;
        }

        break;
    }

    removeHunterFromList(hunter->room->hunters,hunter);
    addHunter(newRoom->hunters,hunter);
    hunter->room = newRoom;
    
    l_hunterMove(hunter->name,hunter->room->name);

    sem_post(&(newRoom->mutex));
    sem_post(&(currRoom->mutex));
    
    return C_TRUE;
    
}


/*
    function: reviewEidences(HouseType *house)
    purpose: this function checks if we have enough evidence to identify the ghost 
    in: HouseType *house: this is used to check the shared evidences list 
    returns: int: the count of evidence collected
*/
int reviewEvidences (HouseType *house){
    sem_wait(&(house->mutex));
    int count = 0; 

    EvidenceNodeType *temp = house->evidences->head; 

    while(temp!=NULL){
        count++;
        temp = temp->next;
    }
    sem_post(&(house->mutex));

   

    return count; 
}

/*
    function: removeEvidenceFromRoom(EvidencesListType *list, EvidenceClass evidenceToRemove)
    purpose: this removes the evidence from the room once collected 
    in/out: EvidencesListType *list: the list that we weill be removing the evidnece from
    in: EvidenceClass evidenceToRemove: this takes in the evidence that must be removed
    returns: int: This returns weather it was removes sucessfuly or not
*/
int removeEvidenceFromRoom(EvidencesListType *list, EvidenceClass evidenceToRemove) {
    

    EvidenceNodeType *currentNode = list->head;
    EvidenceNodeType *previousNode = NULL;

    while (currentNode != NULL) {
        if (currentNode->data == evidenceToRemove) {
            if (previousNode == NULL) {
                list->head = currentNode->next;
            } 
            else {
                previousNode->next = currentNode->next;
            }

            if (currentNode == list->tail) {
                list->tail = previousNode;
            }

            free(currentNode); 
            return C_TRUE;

        }
        previousNode = currentNode;
        currentNode = currentNode->next;
    }

    return C_FALSE; 
}

/*
    function: void *hunterThread(void *arg) 
    purpose: this is the thread the hunters will be using that will cause them to move collect or review evidnece
    in: void *arg: this will take in a thread struct that contains a house and a hunter so we can do operations to them
*/
void *hunterThread(void *arg) {
    ThreadType *thread = (ThreadType *)arg;
    HouseType *house = thread->house;
    HunterType *hunter = thread->hunter;

    free(thread);

    while (hunter->boredem < BOREDOM_MAX && hunter->fear < FEAR_MAX ) {
        usleep(HUNTER_WAIT);
        if (hunter->room->ghost != NULL) {
            hunter->boredem = 0;
            hunter->fear++;
        } else {
            hunter->boredem++;
        }
        int HunterChoice = randInt(0, 3);

        if (HunterChoice == 0) {
            sem_wait(&(hunter->room->mutex));
            collectingEvidence(hunter, house);
            
            sem_post(&(hunter->room->mutex));
        } else if (HunterChoice == 1) {
            moveHunter(hunter);
            
        } else if (HunterChoice == 2) {
            int evidenceCount = reviewEvidences(house);

            if (evidenceCount == 3) {
                l_hunterReview(hunter->name, LOG_SUFFICIENT);
                removeHunterFromList(hunter->room->hunters, hunter);
                l_hunterExit(hunter->name, LOG_EVIDENCE);
                pthread_exit(NULL);

           }else{
                l_hunterReview(hunter->name, LOG_INSUFFICIENT);
                sem_post(&(hunter->room->mutex));
           }
        
        }
    }
    
    sem_wait(&(hunter->room->mutex));
    
    if (hunter->boredem >= BOREDOM_MAX) {
        l_hunterExit(hunter->name, LOG_BORED);
    } else if (hunter->fear >= FEAR_MAX) {
        l_hunterExit(hunter->name, LOG_FEAR);
    }
    removeHunterFromList(hunter->room->hunters, hunter);
    sem_post(&(hunter->room->mutex));
    return NULL;
}

/*
    function: freeHunterData(HunterListType *list)
    purpose: this frees the data in a hunter list 
    in: HunterListType *list: the hunter list we will be using to clear data
*/
void freeHunterData(HunterListType *list){
    HunterNodeType *tempNode = list->head;
	while(tempNode != NULL){
        freeEvidenceList(tempNode->data->gatheredEvidence);
		free(tempNode->data);
		tempNode = tempNode->next;
	}
}

/*
    function: freeHunterList(HunterListType *list)
    purpose: this frees the nodes and data  
    in: HunterListType *list: the hunter list we will be using to clear data
*/
void freeHunterList(HunterListType *list){
    freeHunterData(list);
    HunterNodeType *tempNode;
	while(list->head != NULL){
		tempNode = list->head;
		list->head = list->head->next;
		free(tempNode);
	}
    free(list);
}