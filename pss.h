/***************************************************************
 *
 * file: pss.h
 *
 * @Authors  Nikolaos Vasilikopoulos (nvasilik@csd.uoc.gr), John Petropoulos (johnpetr@csd.uoc.gr)
 * @Version 30-11-2022
 *
 * @e-mail       hy240-list@csd.uoc.gr
 *
 * @brief   Header file for the Public Subscribe System,
 * with the structures and function prototypes
 *
 ***************************************************************
 */

#ifndef pss_h
#define pss_h
#define MG 64



struct Info {
    int iId;
    int itm;
    int igp[MG];
    struct Info *ilc;
    struct Info *irc;
    struct Info *ip;
};
struct Subscription {
    int sId;
    struct Subscription *snext;
};
struct Group {
    int gId;
    struct Subscription *gsub;
    struct Info *gr;
};
struct SubInfo {
    int sId;
    int stm;
    struct TreeInfo *tgp[MG];
    struct TreeInfo *sgp[MG];
    struct SubInfo *snext;
};
struct TreeInfo {
    int tId;
    int ttm;
    struct TreeInfo *tlc;
    struct TreeInfo *trc;
    struct TreeInfo *tp;
    struct TreeInfo *next;
    struct TreeInfo *prev;
};

/**
 * @brief Optional function to initialize data structures that
 *        need initialization
 *
 * @param m Size of hash table
 * @param p Prime number for the universal hash function
 *
 * @return 0 on success
 *         1 on failure
 */
int initialize(int m, int p);

/**
 * @brief Free resources
 *
 * @return 0 on success
 *         1 on failure
 */
int free_all(void);

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
int Insert_Info(int iTM,int iId,int* gids_arr,int size_of_gids_arr);

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
int Subscriber_Registration(int sTM,int sId,int* gids_arr,int size_of_gids_arr);

/**
 * @brief Prune Information from server and forward it to client
 *
 * @param tm Information timestamp of arrival
 * @return 0 on success
 *          1 on failure
 */
int Prune(int tm);

/**
 * @brief Consume Information for subscriber
 *
 * @param sId Subscriber identifier
 * @return 0 on success
 *          1 on failure
 */
int Consume(int sId);

/**
 * @brief Delete subscriber
 *
 * @param sId Subscriber identifier
 * @return 0 on success
 *          1 on failure
 */
int Delete_Subscriber(int sId);

/**
 * @brief Print Data Structures of the system
 *
 * @return 0 on success
 *          1 on failure
 */
int Print_all(void);

struct Info* DLBTinsert(struct Info* r, int iId, int itm);

struct Info* DLBTdelete(struct Info* r, int iId);

struct Info* BLBTlookUp(struct Info* r, int iId);

void DLBTprint(struct Info* r);

void InsertPirnt(struct Group group, int mode);

void Printids(struct Info* list);


struct TreeInfo* LOincert(struct TreeInfo* r, int tId, int ttm);

struct TreeInfo* LOlookUp(struct TreeInfo* r, int id);


int hasFunct(int x);

struct HT{
    struct SubInfo** entries;
};

struct HT* HTcreate();

struct SubInfo* SL_Insert(struct SubInfo* head, int sTm, int sId);

void HT_insert(struct HT* hashtable, int id, int tm );

struct SubInfo* SL_Delete(struct SubInfo* head, int idKey);

void HT_delete(struct HT* hashtable, int id);

struct SubInfo* SL_LookUp(struct SubInfo* head, int key);

struct SubInfo* HT_lookUp(struct HT* hashtable, int id);


struct Subscription* L_Insert(struct Subscription* head, int id);

struct Subscription* L_Delete(struct Subscription* head, int id);

struct Subscription* L_LookUp(struct Subscription* head, int id);

void L_print(struct Subscription* head);


void RegPrint(int* gids_arr, int size_of_gids_arr);

void PrintSubListIDs(struct HT* head);

void PrintGroupSubLists(int* gids_arr, int size_of_gids_arr);


void tranferInfo(struct Info* r, int tm, int i);

void Rprint();



#endif /* pss_h */

