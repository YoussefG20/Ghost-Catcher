#include "defs.h"

int main()
{
    EvidenceClass evidenceList[] = { EMF,TEMPERATURE, FINGERPRINTS,SOUND};
    // Initialize the random number generator
    srand(time(NULL));

    HouseType house;
    initHouse(&house);
    populateRooms(&house);

    RoomType *van = house.rooms->head->data;

    for (int i=0; i < NUM_HUNTERS; i++){
        
        char userName[MAX_STR]; 

        printf("Please Enter Hunters Name:"); 
        scanf("%s", userName);

        HunterType *currentHunter;
        

        initHunter(van,evidenceList[i],userName,&currentHunter);
        hunterIntoRoom(van,currentHunter);
        addHunter(house.hunters, currentHunter);

    }

    pthread_t hunterThreadArray[4];

    RoomType *ghostRoom = addGhostToRandomRoom(house.rooms)->data;
    
     GhostType *ghost; 

     initGhost(randInt(0,4),ghostRoom,&ghost); 

    pthread_t pThreadghost;

       

    pthread_create(&pThreadghost, NULL, ghostThread, (void*) ghost);

    ThreadType *thread1 = (ThreadType*) malloc(sizeof(ThreadType));    
    thread1->house = &house;
    thread1->hunter = house.hunters->head->data;
    pthread_create(&hunterThreadArray[0], NULL, hunterThread, (void*) thread1);


    ThreadType *thread2 = (ThreadType*) malloc(sizeof(ThreadType));    
    thread2->house = &house;
    thread2->hunter = house.hunters->head->next->data;
    pthread_create(&hunterThreadArray[1], NULL, hunterThread, (void*) thread2);


    ThreadType *thread3 = (ThreadType*) malloc(sizeof(ThreadType));    
    thread3->house = &house;
    thread3->hunter = house.hunters->head->next->next->data;

    pthread_create(&hunterThreadArray[2], NULL, hunterThread, (void*) thread3);

    ThreadType *thread4 = (ThreadType*) malloc(sizeof(ThreadType));    
    thread4->house = &house;
    thread4->hunter = house.hunters->head->next->next->next->data;
    pthread_create(&hunterThreadArray[3], NULL, hunterThread, (void*) thread4);


    pthread_join(hunterThreadArray[0], NULL);
    pthread_join(hunterThreadArray[1], NULL);
    pthread_join(hunterThreadArray[2], NULL);
    pthread_join(hunterThreadArray[3], NULL);
    pthread_join(pThreadghost, NULL);


    printResults(&house, ghost);

    freeRoomList(house.rooms);
    freeEvidenceList(house.evidences);
    freeHunterList(house.hunters);
    free(ghost);

    return 0;
}

/*
    function: printResults(HouseType *house, GhostType *ghost)
    purpose: prints the results after done executing 
    in: HouseType *house: takes in the house to get teh shared evidence list 
    in GhostType *ghost:  takes in the ghost to see if we have identified it 
*/
void printResults(HouseType *house, GhostType *ghost){
    const char *ghostTypeList[] = { "POLTERGEIST", "BANSHEE", "BULLIES", "PHANTOM", "GHOST_COUNT", "GH_UNKNOWN" };

    printf("================================== \n");
    printf("All Done ! Let's look at the results :\n ");
    printf("================================== \n");

    getFearAndBoredom(house);
    if(reviewEvidences(house) >=3 ){
        printf("Ghost was discovered !\n");
        printf("\n");
        printf( "Using the evidence they found the hunters correctly deteremined the ghost was a %s \n", ghostTypeList[getGhostClass(house, ghost)]);
        printf("\n");
        printf("The hunters collected the following evidence : \n");
        printEvidence(house);
    }else{
        printf("Ghost was not discovered ! \n");
        printf("The ghost has won !");
    }
}

/*
    function: getGhostClass(HouseType *house, GhostType *ghost)
    purpose: checks if we have enough evidence and that we identified the right ghost 
    in: HouseType *house: takes in house to get shared list;
    in: take in the ghost to return the ghosttype 
    out: the ghost class is returned 
    return: the ghost class gets returned 
*/
GhostClass getGhostClass(HouseType *house, GhostType *ghost){

    int count  = reviewEvidences(house);
    if(count == 3){
        return ghost->GhostClass;
    }else{
        return GH_UNKNOWN;
    }

}

/*
    function: printEvidence(HouseType *house)
    purpose: prints out the evidence that the hunter has collected 
    in: HouseType *house: takes in house to get shared list;
*/
void printEvidence(HouseType *house){
    const char *evidenceList[] = { "EMF","TEMPERATURE", "FINGERPRINTS","SOUND"};
    EvidenceNodeType *temp = house->evidences->head;
    if(house->evidences->head == NULL){
        return;
    }
    else{
        while(temp != NULL){
            printf("    *   %s ",evidenceList[temp->data] );
            printf("\n");
            temp = temp->next;
        }
    }
}

/*
    function: getFearAndBoredom(HouseType *house)
    purpose: prints out the hunters that reached max boredom or max fear
    in: takes in house to get shared list;
*/
void getFearAndBoredom(HouseType *house){
    HunterNodeType *temp = house->hunters->head;
    while(temp != NULL){
        if(temp->data->fear >= FEAR_MAX){
            printf("%s's fear was too high !", temp->data->name);
            printf("\n");
        }
        if(temp->data->boredem >= BOREDOM_MAX){
            printf("%s's boredom was too high!", temp->data->name);
            printf("\n");
        }
        temp = temp->next;
    }
}