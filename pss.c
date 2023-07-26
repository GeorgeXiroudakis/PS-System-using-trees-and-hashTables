#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

#include "pss.h"

struct Group *G;
struct HT *S;
struct TreeInfo *C;
int M;
int P;
int a;
int b;

/**
 * @brief Optional function to initialize data structures that
 *        need initialization
 *
 * @param m Size of the hash table.
 * @param p Prime number for the universal hash functions.
 *
 * @return 0 on success
 *         1 on failure
 */
int initialize(int m, int p){
    G = malloc(MG * sizeof(struct Group));
    if(G == NULL)return EXIT_FAILURE;

    int i;
    for(i = 0; i < MG; i++){
        G[i].gId = i;
        G[i].gr = NULL;
        G[i].gsub = NULL;
    }

    M = m;
    P = p;

    S = HTcreate();

    srand(time(0));
    a = rand() % (P - 1) + 1;
    b = rand() % (P - 1) + 0;

    C = NULL;

    return EXIT_SUCCESS;
}

/**
 * @brief Free resources
 *
 * @return 0 on success
 *         1 on failure
 */
int free_all(void){
    return EXIT_SUCCESS;
}

/**
 * @brief Insert info
 *
 * @param iTM Timestamp of arrival
 * @param iId Identifier of information
 * @param gids_arr Pointer to array containing the gids of the Event.
 * @param size_of_gids_arr Size of gids_arr including -1
 * @return 0 on success
 *          1 on failure
 */
int Insert_Info(int iTM,int iId,int* gids_arr,int size_of_gids_arr){
    int i;
    int j;
    for(i = 0; i < size_of_gids_arr - 1; i++){
        if(gids_arr[i] < 0 || gids_arr[i] >= MG)return EXIT_FAILURE;
        if(BLBTlookUp(G[gids_arr[i]].gr, iId) != NULL)return EXIT_FAILURE;
        G[gids_arr[i]].gr = DLBTinsert(G[gids_arr[i]].gr, iId, iTM);
        
        struct Info* info = BLBTlookUp(G[gids_arr[i]].gr, iId);
        if(info == NULL)return EXIT_FAILURE;

        for(j = 0; j < size_of_gids_arr - 1; j++){
            info->igp[gids_arr[j]] = 1;           
        }
        
        InsertPirnt(G[gids_arr[i]], 1);
        printf("\n");

    }

        return EXIT_SUCCESS;
}
/**
 * @brief Subsriber Registration
 *
 * @param sTM Timestamp of arrival
 * @param sId Identifier of subscriber
 * @param gids_arr Pointer to array containing the gids of the Event.
 * @param size_of_gids_arr Size of gids_arr including -1
 * @return 0 on success
 *          1 on failure
 */
int Subscriber_Registration(int sTM,int sId,int* gids_arr,int size_of_gids_arr){

    HT_insert(S, sId, sTM);

    int i;
    struct SubInfo* temp = HT_lookUp(S, sId);
    if(temp == NULL)return EXIT_FAILURE;
    for(i = 0; i < size_of_gids_arr -  1; i++){
        if(L_LookUp(G[gids_arr[i]].gsub, sId) != NULL)return EXIT_FAILURE;
        G[gids_arr[i]].gsub = L_Insert(G[gids_arr[i]].gsub, sId);

        temp->sgp[gids_arr[i]] = NULL;
        temp->tgp[gids_arr[i]] = NULL;
    }

    RegPrint(gids_arr, size_of_gids_arr);
    printf("\n");

    return EXIT_SUCCESS;
}
/**
 * @brief Prune Information from server and forward it to client
 *
 * @param tm Information timestamp of arrival
 * @return 0 on success
 *          1 on failure
 */
int Prune(int tm){

    int i;
    for(i = 0; i < MG; i++){
        if(i == 4&&tm ==1591){
        i  = 4;
    }
        struct Info* r = G[i].gr;
        
        tranferInfo(r, tm, i);
    }

    Rprint();

    return EXIT_SUCCESS;
}
/**
 * @brief Consume Information for subscriber
 *
 * @param sId Subscriber identifier
 * @return 0 on success
 *          1 on failure
 */
int Consume(int sId){
   struct SubInfo* sub =  HT_lookUp(S, sId);
   if(sub == NULL)return EXIT_FAILURE;

    int i;
    for(i = 0; i < MG; i++){
        if(sub->sgp[i] != (struct TreeInfo*) 1 ){
            printf("GROUPID = <%d>, TREELIST = <", G[i].gId);
            while(sub->sgp[i] != NULL){
                printf("%d ", sub->sgp[i]->tId);

                if(sub->sgp[i]->next != NULL)sub->sgp[i] = sub->sgp[i]->next;
                else break;
            }
            if(sub->sgp[i] == NULL) printf(">,NEWSGP = <NULL>");
            else printf(">,NEWSGP = <%d>", sub->sgp[i]->tId);

            printf("\n");
        }            
    }
    return EXIT_SUCCESS;
}
/**
 * @brief Delete subscriber
 *
 * @param sId Subscriber identifier
 * @return 0 on success
 *          1 on failure
 */
int Delete_Subscriber(int sId){
    struct SubInfo* ToDelete = HT_lookUp(S, sId);
    if(ToDelete == NULL)return EXIT_FAILURE;

    int i;
    int k = 0;
    int num[MG];
    for(i = 0; i < MG; i++){
        if(ToDelete->sgp[i] != (struct TreeInfo*) 1){
            G[i].gsub = L_Delete(G[i].gsub, sId);
            num[k++] = i;
        }
    }
    HT_delete(S, sId);

    RegPrint(num, k + 1);
    printf("\n");

    return EXIT_SUCCESS;
}
/**
 * @brief Print Data Structures of the system
 *
 * @return 0 on success
 *          1 on failure
 */
int Print_all(void){
    int i;
    for(i = 0; i < MG; i++){
        InsertPirnt(G[i], 2);
        printf("\n");
        
    }
    printf("SUBSCRIBERLIST =");
    PrintSubListIDs(S);
    printf("\n\n");

    int countOfSubs = 0;
    for(i = 0; i< M; i++){
        struct SubInfo* tempSub = S->entries[i];
        while(tempSub != NULL){
            countOfSubs += 1;
            printf("SUBSCRIBERID = <%d>, GROUPLIST =", tempSub->sId);
            int y;
            for(y = 0; y < MG; y++){
                if(tempSub->sgp[y] != (struct TreeInfo*) 1)printf("%d ", G[y].gId );
            }
            printf("\n");
            tempSub = tempSub->snext;
        }
    }

    printf("\nNO_GROUPS = <%d>, NO_SUBSCRIBERS = <%d>\n", MG, countOfSubs);
    

    return EXIT_SUCCESS;
}


struct Info* DLBTinsert(struct Info* r, int iId, int itm){
    struct Info* new = (struct Info*) malloc(sizeof(struct Info));
    new->iId = iId;
    new->itm = itm;
    new->ip = NULL;
    new->ilc = NULL;
    new->irc = NULL;

    /*if tree is emty*/
    if(r == NULL)return new;

    struct Info* p = r;
    struct Info* prev = NULL;

    /*finding the parent of the new node(will be the prev pointer)*/
    while (p != NULL){
        prev = p;
        if(iId < p->iId)p = p->ilc;
        else p = p->irc;
    }

    new->ip = prev;

    if(iId < prev->iId) prev->ilc = new;
    else prev->irc = new;

    return r;
}

struct Info* DLBTdelete(struct Info* r, int iId){
    if (r == NULL)return r;
    
    struct Info* p = r; 

     while (p != NULL && p->iId != iId){
        if(iId < p->iId)p = p->ilc;
        else p = p->irc;
    }
    /*it it not found*/
    if (p == NULL)return r;

    /*p is the node to delete
    we take the 3 cases*/

    /*is a leaf deleting with no changes*/
    if(p->ilc == NULL && p->irc == NULL){
        if(p->ip == NULL)return NULL;
        if(p->ip->ilc == p)p->ip->ilc = NULL;
        else p->ip->irc = NULL;
        return r;
    }

    struct Info* replacement;

    /*if it has one child*/
    if( (p->ilc != NULL || p->irc != NULL) && (!(p->ilc != NULL && p->irc != NULL))){
        if(p->ilc != NULL) replacement = p->ilc;
        if(p->irc != NULL) replacement = p->irc;

    if(p->ip != NULL){
        if(p->ip->irc == p)p->ip->irc = replacement;else p->ip->ilc = replacement;
        replacement->ip = p->ip;
    }

        
        return r;
    }

    /*if it has two children 
    to find the previous in the in order you can go to the left child and then full right*/

    /*check if th root is changing*/
    if(p == r) r = p->ilc;

    /*find replacement*/
    replacement = p->ilc;
    while (replacement->irc != NULL){
        replacement = replacement->irc;
    }

    replacement->irc = p->irc;
    if(p->ip != NULL){
        if(p->ip->irc == p)p->ip->irc = replacement;else p->ip->ilc = replacement;
        replacement->ip = p->ip;
    }

    return r;
 
}

struct Info* BLBTlookUp(struct Info* r, int iId){

    struct Info* p = r;

    while (p != NULL && p->iId != iId){
        if(iId < p->iId)p = p->ilc;
        else p = p->irc;
    }

    return p;
}

void DLBTprint(struct Info* r){
    if(r == NULL)return;
    
    DLBTprint(r->ilc);
    printf("%d ", r->iId );
    DLBTprint(r->irc);
}

void InsertPirnt(struct Group group, int mode){
    printf("GROUPID = %d, INFOLIST =", group.gId);
    Printids(group.gr);

    if(mode == 2){
        printf(", SUBLIST =");
        struct Subscription* sub = group.gsub;
        while(sub != NULL){
            printf("%d ", sub->sId);
            sub = sub->snext;
        }
    }
    printf("\n");
}

void Printids(struct Info* list){
    struct Info* temp = list;
    if(temp == NULL)printf(" -EMPTY-");
    
    DLBTprint(list);
}


struct TreeInfo* LOincert(struct TreeInfo* r, int tId, int ttm){

    struct TreeInfo* new = (struct TreeInfo*) malloc(sizeof(struct TreeInfo));
    new->next = NULL;
    new->prev = NULL;
    new->tId = tId;
    new->tlc = NULL;
    new->tp = NULL;
    new->trc = NULL;
    new->ttm = ttm;

    if (r == NULL)return new;

    struct TreeInfo* p = r;
    struct TreeInfo* prev = NULL;
    
    /*finding the parent of the new node(will be the prev pointer)*/
    while (p != NULL){
        prev = p;
        if(ttm <= p->ttm)p = p->tlc;
        else p = p->trc;
    }

    /*tranfer the adres of the node we will atack the new node to*/
    p = prev;

    /*only one node*/
    if(r->tlc == NULL && r->trc == NULL){
        struct TreeInfo* newP = (struct TreeInfo*) malloc(sizeof(struct TreeInfo));
        newP->next = NULL;
        newP->prev = NULL;
        newP->tId = tId;
        newP->tlc = NULL;
        newP->tp = NULL;
        newP->trc = NULL;
        newP->ttm = ttm;

        r = newP;
        if(ttm < p->ttm){
            new->next = p;
            p->prev = new;
            p->tp = newP;
            newP->trc = p;
            new->tp = newP;
            newP->tlc = new;
        }else{
            p->next = new;
            new->prev = p;
            new->tp = newP;
            newP->trc = new;
            p->tp = newP;
            newP->tlc = p;
        }

        return r;
    }

    struct TreeInfo* parentOfP = p->tp;

    /*if left child*/
    if(ttm < p->ttm) {

        struct TreeInfo* newP = (struct TreeInfo*) malloc(sizeof(struct TreeInfo));
        newP->next = NULL;
        newP->prev = NULL;
        newP->tId = tId;
        newP->tlc = NULL;
        newP->tp = NULL;
        newP->trc = NULL;
        newP->ttm = ttm;

        newP->tp = parentOfP;

        if(parentOfP->tlc == p)parentOfP->tlc = newP;
        else parentOfP->trc = newP;
        
        if(p->prev != NULL){
            new->prev = p->prev;
            p->prev->next = new;
           
        }
         p->prev = new;
        
        new->next = p;
        p->tp = newP;
        newP->trc = p;
        new->tp = newP;
        newP->tlc = new;
    }
    else {
        struct TreeInfo* newP = (struct TreeInfo*) malloc(sizeof(struct TreeInfo));
        newP->next = NULL;
        newP->prev = NULL;
        newP->tId = p->tId;
        newP->tlc = NULL;
        newP->tp = NULL;
        newP->trc = NULL;
        newP->ttm = p->ttm;

        newP->tp = parentOfP;

        if(parentOfP->tlc == p)parentOfP->tlc = newP;
        else parentOfP->trc = newP;
        new->prev = p;
        p->next = new;
        p->tp = newP;
        newP->tlc = p;
        new->tp = newP;
        newP->trc = new;
    }

    return r;


}

struct TreeInfo* LOlookUp(struct TreeInfo* r, int id){
    struct TreeInfo* p = r;

    while (  !(p == NULL || (p->tId == id && p->tlc == NULL && p->trc == NULL) )  ){
        if(id <= p->tId)p = p->tlc;
        else p = p->trc;
    }

    return p;
}


int hasFunct(int x) {return ((a * x +b ) % P) % M;}

struct HT* HTcreate(){
    struct HT* hashTable = malloc(sizeof(struct HT));

    hashTable->entries = malloc(sizeof(struct SubInfo) * M);

    int i;

    for(i = 0; i < M; i++)hashTable->entries[i] = NULL;

    return hashTable;
}

struct SubInfo* SL_Insert(struct SubInfo* head, int sTm, int sId){
    struct SubInfo* temp;
    struct SubInfo* new = (struct SubInfo*) malloc(sizeof(struct SubInfo));
    new->sId = sId;
    new->stm = sTm;
    new->snext = NULL;
    int i;
    for(i = 0; i < MG; i++){
        new->sgp[i] = (struct TreeInfo*) 1;
        new->tgp[i] = (struct TreeInfo*) 1;
    }

    if(head == NULL || sTm < head->stm){
        new->snext = head;
        head = new;
    }else{
        temp = head;
        while(temp->snext != NULL && temp->snext->stm < sTm)temp = temp->snext;
        new->snext = temp->snext;
        temp->snext = new;
    }
    return head;
}

void HT_insert(struct HT* hashtable, int id, int tm ){
    int slot = hasFunct(id);
    
    struct SubInfo* entry = hashtable->entries[slot];

    entry = SL_Insert(entry, tm, id);

    hashtable->entries[slot] = entry;

}

struct SubInfo* SL_Delete(struct SubInfo* head, int idKey){
    struct SubInfo* cur = head;
    struct SubInfo* prev = head;

    if(head == NULL)return head;

    if(head->sId == idKey){
        head = cur->snext;
        cur = NULL;
        return head;
    }
    while(cur->sId != idKey){
        if(cur->snext == NULL)return head;
        prev = cur;
        cur = cur->snext;
    }
    prev->snext = cur->snext;
    cur = NULL;
return head;
}

void HT_delete(struct HT* hashtable, int id){
    int slot = hasFunct(id);
    
    struct SubInfo* entry = hashtable->entries[slot];

    entry = SL_Delete(entry, id);

    hashtable->entries[slot] = entry;
}

struct SubInfo* SL_LookUp(struct SubInfo* head, int key){
    struct SubInfo* temp = head;
    while (temp != NULL && temp->sId != key)temp = temp->snext;
    return temp;
}

struct SubInfo* HT_lookUp(struct HT* hashtable, int id){
    int slot = hasFunct(id);
    
    struct SubInfo* entry = hashtable->entries[slot];

    entry = SL_LookUp(entry, id);

    return entry;
}


struct Subscription* L_Insert(struct Subscription* head, int id){
    struct Subscription* temp = head;
    struct Subscription* new = (struct Subscription*) malloc(sizeof (struct Subscription));
    new->sId = id;
    new->snext = NULL;
    if(head == NULL)return new;
    
    new->snext = head;
    head = new;
}

struct Subscription* L_Delete(struct Subscription* head, int id){
    if(head == NULL)return head;

    if(head->sId == id){
        head = head->snext;
        return head;
    }
    struct Subscription* cur = head;
    struct Subscription* prev = head;
    while(cur != NULL && cur->sId != id){prev = cur;cur = cur->snext;}
    if(cur == NULL)return head;
    prev->snext = cur->snext;

    return head;
}

struct Subscription* L_LookUp(struct Subscription* head, int id){
    struct Subscription* temp = head;
    while(temp != NULL && temp->sId != id)temp = temp->snext;
    return temp;
}

void L_print(struct Subscription* head){
    struct Subscription* temp = head;
    while (temp != NULL){
        printf(" %d", temp->sId);
        temp = temp->snext;
    }   
}



void RegPrint(int* gids_arr, int size_of_gids_arr){
    printf("SUBSCRIBERLIST =");
    PrintSubListIDs(S);
    printf("\n\n");

    PrintGroupSubLists(gids_arr, size_of_gids_arr);
}

void PrintSubListIDs(struct HT* head){
    struct HT* temp = head;

    int i;
    for(i = 0; i< M; i++){
        struct SubInfo* tempSub = temp->entries[i];
        while(tempSub != NULL){
            printf("%d ", temp->entries[i]->sId);
            tempSub = tempSub->snext;
        }
    }
}

void PrintGroupSubLists(int* gids_arr, int size_of_gids_arr){
    int i;
    for(i = 0; i < size_of_gids_arr - 1; i++){
        printf("\tGROUPID = %d, SUBLIST =", G[gids_arr[i]].gId);
        L_print(G[gids_arr[i]].gsub);
        printf("\n");
    }
}



void tranferInfo(struct Info* r, int tm, int i){
   if(r == NULL)return;

    tranferInfo(r->ilc, tm, i);

    tranferInfo(r->irc, tm, i);

    if(r->itm < tm){
        struct Subscription* temp =  G[i].gsub;
        while( temp != NULL){
            struct SubInfo* sub = HT_lookUp(S, temp->sId);
            sub->tgp[i] = LOincert(sub->tgp[i], r->iId, r->itm);

            struct TreeInfo* treeTemp = sub->tgp[i];
            struct TreeInfo* treeTempPrev = NULL;
            while(treeTemp != NULL){
                treeTempPrev = treeTemp;
                treeTemp = treeTemp->tlc;
            }
            sub->sgp[i] = treeTempPrev;

            temp = temp->snext;
        }

        G[i].gr = DLBTdelete(G[i].gr, r->iId);
    }
  
}

void Rprint(){
    int i;
    for(i = 0; i < MG; i++){
        printf("GROUPID = <%d>, INFOLIST = <", G[i].gId);
        DLBTprint(G[i].gr);
        printf(">,SUBLIST = <");
        L_print(G[i].gsub);
        printf(">");
        printf("\n\n");
    }

    for(i = 0; i < M; i++){
        struct SubInfo* cur = S->entries[i];
        while(cur != NULL){
            printf("\nSUBSCRIBERID = <%d>, GROUPLIST =\n", cur->sId);
            
            int y;
            for(y = 0; y < MG; y++){
                struct TreeInfo* curInfo = cur->tgp[y];
                struct TreeInfo* curInfoPrev;
                if(curInfo != (struct TreeInfo*) 1){
                    printf("\n<%d>, TREELIST =<", G[y].gId);
                    while(curInfo != NULL){
                        curInfoPrev = curInfo;
                        curInfo = curInfo->tlc;
                    }
                    while(curInfoPrev != NULL){
                        printf("%d ", curInfoPrev->tId);

                        curInfoPrev = curInfoPrev->next;
                    }
                    printf(">");
                }
            }

            printf("\n");
            cur = cur->snext;
        }
    }

    
}
