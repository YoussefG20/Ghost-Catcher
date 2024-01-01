#include "defs.h"
/*
    function: createRoom(const char* roomName)
    purpose: intializes the room 
    in: const char* roomName: takes in the room name to intialize it 
    out: RoomType *newRoom: this is the room pointer that is pointing to dynamic memory that will store our room
    return: RoomType* node: the *newroom will be returned so that they could set the rooms in house.c to it  
*/
RoomType* createRoom(const char* roomName){
    RoomType *newRoom = (RoomType*) malloc(sizeof(RoomType));
    sem_init(&(newRoom->mutex),0,1);

    strcpy(newRoom->name, roomName);
    newRoom->connected = (RoomsListType*) malloc(sizeof(RoomsListType));
    initRoomsList(newRoom->connected);

    newRoom->evidence = (EvidencesListType*) malloc(sizeof(EvidencesListType));
    initEvidenceList(newRoom->evidence);

    newRoom->hunters = (HunterListType*) malloc(sizeof(HunterListType));

    initHuntersList(newRoom->hunters);
    newRoom->ghost = NULL; 

    return newRoom;
}

/*
    function: connectRooms(RoomType *room1,  RoomType *room2)
    purpose: adds each room to eachother conected rooms list 
    in: RoomType *room1: this takes in the first room that we must add to
    in: RoomType *room2: this takes in the second room that we must add to
*/
void connectRooms(RoomType *room1,  RoomType *room2) {
    addRoom(room1->connected, room2);
    addRoom(room2->connected, room1);
}

/*
    function: addRoom(RoomsListType *roomList, RoomType *room)
    purpose: adds each room to a rooms list 
    in: RoomListType *roomList: this takes in the list that we will be adding the rooms to 
    in: RoomType *room: this takes in the room that we will be adding to the list 
*/
void addRoom(RoomsListType *roomList, RoomType *room){

    RoomNodeType *newNode = malloc(sizeof(RoomNodeType));

    newNode->data = room; 
    newNode->next = NULL; 

    if (roomList->tail == NULL){
        roomList->head = newNode; 
        roomList->tail = newNode; 
    }
    else{
        roomList->tail->next = newNode;
        roomList->tail = newNode;
    }

}


/*
    function: void hunterIntoRoom(RoomType *room, HunterType *hunter)
    purpose: adds a hunter to rooms hunterlist 
    in: HunterType *hunter: this takes in the hunter that will be added to the list 
    in: RoomType *room: takes in the room that the hunter needs to be added to 
*/
void hunterIntoRoom(RoomType *room, HunterType *hunter){
    addHunter(room->hunters, hunter);
}

/*
    function: freeConnectedRooms(RoomsListType* list)
    purpose: this frees the memory in the connected rooms list in the room structure 
    in/out: RoomsListType *list: taking in the list and clearing all the nodes in the list;
*/
void freeConnectedRooms(RoomsListType* list){
    RoomNodeType *tempNode;
	while(list->head != NULL){
		tempNode = list->head;
		list->head = list->head->next;
        free(tempNode);
	}
}

 
/*
    function: void freeRoomList(RoomsListType *list)
    purpose: this will end up freeing everything in the house cause it will go into the roomn list in the house and free all from the heap; 
    in/out: RoomsListType *list: taking in the list and clearing all that is included in the list and the list itself;
*/

void freeRoomList(RoomsListType *list){
	RoomNodeType *tempNode;
	while(list->head != NULL){
		tempNode = list->head;
		list->head = list->head->next;
        sem_destroy(&(tempNode->data->mutex));
        freeEvidenceList(tempNode->data->evidence);
        freeConnectedRooms(tempNode->data->connected);
        free(tempNode->data->connected);
        freeHunterList(tempNode->data->hunters);
        free(tempNode->data);
		free(tempNode);
	}

    free(list);
}
