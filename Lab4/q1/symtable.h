#include <string.h>

typedef struct token {
    char tName[256];
    char lName[256];
    int index;
    int line;
    int column;
} token;


typedef struct symTableEntry {
    int id;
    char varName[256];
    char varType[256];
    char varMemSize[256];
    char scope;
    int numArgs;
    token* args[256];
    char returnType[256];
    struct symTableEntry* next;
} symTableEntry;

int h(int x);
int search(symTableEntry* st[], char l[]);
void insert(symTableEntry* st[], symTableEntry* newBoi, int* symCount);

int h(int x) {
    return x % 10;
}

int search(symTableEntry* st[], char l[]) {
    symTableEntry* temp;
    for(int i = 0; i < 10; i++) {
        for(temp = st[i]; temp; temp = temp->next) {            
            if (strcmp(temp->varName, l) == 0) {
                return temp->id;
            }
        }
    }
    return -1;
}

void insert(symTableEntry* st[], symTableEntry* newBoi, int* symCount) {
    (*symCount)++;
    int here = h(*symCount);
    symTableEntry* temp, *last;
    if( st[here] == NULL ) {
        st[here] = newBoi;
        return;
    }   
    for(temp = st[here]; temp; temp = temp->next) {
        last = temp;
    }
    last->next = newBoi;
}