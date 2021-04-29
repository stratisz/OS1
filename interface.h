#include "types.h"

struct index* insertIndex(struct index*,struct student*);
TableEntry* createHashtable(int);
int hash(char*,int);
struct student* insertHashtable(int,struct student*,TableEntry*);
struct student* lookup(int,TableEntry*,char*);
void delete(int,char*,TableEntry*,struct index*);
int number(struct index*,int);
void top(struct index*,int,int);
void average(struct index*,int);
void minimum(struct index*,int);
void count(struct index*);
void postalcode(TableEntry*,int,int);
void exitFree(TableEntry*,struct index*,int);