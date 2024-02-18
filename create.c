/*EL2008 Pemecahan Masalah dengan C 2021/2022
*Modul              : 9 - Tugas Besar
*Kelompok           : 02
*Anggota Kelompok   : Audrey Nissi (13220006) - Noor Shafira (13220008) - Thio Triansyah Putra (13220010)
*Nama File          : main.c
*Deskripsi          : Program Logic Minimization
*/

#include <stdio.h>
#include <malloc.h>

#define bitsSize 4 //banyak variabel pada fungsi
#define limit 16 //Maksimum banyak minterms-1 (2^bitSize-1)

//list kumpulan kelompok minterms
struct vector 
    {
    int paired[limit];
    };

//menyimpan informasi minterms
struct Node 
    {
    struct Node* next;             //link node berikutnya
    int hasPaired;                 //1 apabila paired, 0 apabila belum
    int numberOfOnes;              //banyaknya angka 1
    struct vector paired;       
    int group;                     //kelompok berdasarkan banyak angka 1
    int binary[bitsSize];          
    int numberOfPairs;             //banyak kelompok yang sudah dibentuk
    }; typedef struct Node node;

//Defining Prime Implicants Table
struct implicantsTable         
    {
    int arr[limit][bitsSize];
    int brr[limit][limit];
    int top;                       //banyaknya prime implicant yang sudah ditambah
    int mintermCounter[limit];     //banyak minterms pada prime implicant tertentu
    } Table;

//Declarations of function prototype 
node* createNode(int);
node* createNodePair(node*,node*);
node *head,*head2;

void add(int);
void pair();
void display();
void displayTable();
void binaryFill(node*,node*,node*);
void initTable();
void addPair(node*,node*);
void addToTable();
void analyseTable();
void binaryFill(node*,node*,node*);
void convertBinaryToMintermNotation(int);
void removeMintermsFromTable(int);

int ifPairingPossible(node*,node*);
int ifDontCare(int);
int ifMintermPresentInImplicant(int,int);
int findMaxInTable(int*);
int numberOfImplicants(int,int*);
int minterms;               //banyaknya minterms
int maxGroup,newMaxGroup; 
int mintermsGiven[limit];   //nilai minterms
int dontCares[limit];       //nilai dontCares

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------*/

//checks if a particular minterm is a dont Care
int ifDontCare(int i) {
    if(dontCares[i]==1)
        return 1;
    else
        return 0;
    }

//creates a linked list to store given minterms
void add(int n) {
    node *p,*q,*temp;
    p = createNode(n);

    if(p != NULL) {
        
        if(head == NULL) {
            head = p;
            head->next = NULL;
            return;
            }

        else {
            
            q = head;
            
            if(p->group < head->group) {
                p->next = head;
                head = p;
                return;
                }

            while(q->next != NULL &&((q->next->group) <= (p->group))) {
                q = q->next;
                }

            if(q->next == NULL) {
                q->next = p;
                p->next = NULL;
                }

            else {
                temp = q->next;
                q->next = p;
                p->next = temp;
                }
            }
        }
    }

//Storing paired minterms using linked list
void addPair(node *p,node *q) {
    node *r,*temp;
    r = createNodePair(p,q);

    if(head2 == NULL) { head2 = r; }
    else {
        temp = head2;
        while(temp->next != NULL)
            temp = temp->next;
        temp->next = r;
        }
    }

//creates a new node using given nodes
node* createNodePair(node *p,node *q) {
    int i,j;
    node *r;
    r = (node *)malloc(sizeof(node));

    if(r == NULL)
        printf("Insertion Failed.....\n");
    else {
        for(i = 0; i < p->numberOfPairs; i++) {
            r->paired.paired[i] = p->paired.paired[i];
            }

        r->numberOfPairs = p->numberOfPairs*2;

        for(j=0; j < q->numberOfPairs; j++) {
            r->paired.paired[i++] = q->paired.paired[j];
            }
        r->hasPaired = 0;
        r->next = NULL;
        r->group = p->group;
        binaryFill(p,q,r);
        }
    return r;
    }

//Fill in binary values using linked list//
void binaryFill(node *p,node *q,node *r) {
    int c = bitsSize - 1;

    while(c != -1) {
        if(p->binary[c] == q->binary[c]) {
            r->binary[c] = p->binary[c]; }
        else {
            r->binary[c] =- 1; }
        c--; }
    }

//creates a node to store the minterm data
node* createNode(int n) {
    int c = bitsSize-1;
    node *p;
    p = (node *)malloc(sizeof(node));

    if(p == NULL)
        printf("Insertion Failed.....\n");
    else {
        p->numberOfOnes = 0;
        p->paired.paired[0] = n;
        p->numberOfPairs = 1;

        while(n != 0) {
            p->binary[c] = n%2;
            if(p->binary[c] == 1)
                p->numberOfOnes++;
            c--;
            n=n/2;
            }
        
        while(c!=-1) {
            p->binary[c]=0;
            c--;
            }
        p->hasPaired=0;
        }

    p->group = p->numberOfOnes;
    if(p->group > maxGroup)
        maxGroup = p->group;
    return p;
    }

//displays the minterms and their pairing and binary values at each pass
void display() {
    int c = 1, count = 0, j = 0;
    node *p;
    p = head;

    while(p != NULL) {
        j = 0;
        while(count < (p->numberOfPairs)) {
            printf("%d,",p->paired.paired[count]);
            count++; }

        printf("\b");
        count = 0;
        printf("   ");

        while(j < bitsSize) {
            if(p->binary[j] == -1)
                printf("%c",'-');
            else
                printf("%d",p->binary[j]);
            j++; }
        printf("\n");
        c++;
        p = p->next;
        }
    }

//Pairing Function
void pair() {
    node *p,*q;
    int oneMatched = 0;
    static int iteration = 1;  //stores the iteration or pass count
    p = head;
    q = p;

    printf("\nIteration  %d........\n", iteration);
    iteration++;
    display();
    newMaxGroup =- 1;
    while(p->group != maxGroup) {
        q = q->next;
        while(q != NULL && (q->group == p->group)) {
            q = q->next; }
        if(q == NULL) {
            p = p -> next;
            q = p;
            continue; }
        else {
            if(q->group != (p->group + 1)) {
                p = p->next;
                q = p;
                continue; }

            //If pairing is possible, set p and q value to 1 and add them to the new linked list    
            if(ifPairingPossible(p,q)) {
                oneMatched = 1;
                p->hasPaired = 1;
                q->hasPaired = 1;
                addPair(p,q);

                if((p->group) > newMaxGroup)
                    newMaxGroup = p->group;
                }
            }
        } addToTable();

//checks if atleast one pair has been formed else it returns
    if(oneMatched) {
        head = head2;
        head2 = NULL;
        maxGroup = newMaxGroup;
        pair();
        }
    }


//Checking bit differences
int ifPairingPossible(node *a,node *b) {
    int c = bitsSize - 1;
    int oneNotSame = 0;

    while(c != -1) {
        if(a->binary[c] != b->binary[c]) {
            if(oneNotSame)
                return 0;
            else
                oneNotSame = 1;
            } c--;
        } return 1;
    }
