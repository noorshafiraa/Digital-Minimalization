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

//initialise the Prime Implicants Table with all cells -1 means empty
void initTable() {

    for(int j = 0; j <= limit-1; j++)
        for(int i = 0; i <= limit-1; i++)
            Table.brr[j][i]=-1;
    }

void addToTable() {
    int i, j, k, allMatched;
    node *p;
    p = head;

    while(p != NULL) {
        if(!(p->hasPaired)) {
            //Checking duplicates
            if(Table.top != 0) {
                for(j = 0; j < Table.top; j++) {
                    allMatched = 1;
                    for(k = 0; k < p->numberOfPairs; k++) {
                        if(Table.brr[j][p->paired.paired[k]] == 1)
                            continue;
                        else {
                            allMatched = 0;
                            break;
                            }
                        }
                    if(allMatched == 1) {
                        break;
                        }
                    }
                if(allMatched == 1) {
                    p = p->next;
                    continue;
                    }
                }
            i = bitsSize-1;
            while(i != -1) {
                Table.arr[Table.top][i] = p->binary[i];
                i--; }
            for(i = 0; i < p->numberOfPairs; i++) {
                //Assigning don't cares' outside of the table
                if(ifDontCare(p->paired.paired[i]) == 1) {
                    Table.brr[Table.top][p->paired.paired[i]] =- 1;
                    continue;
                    }
                Table.mintermCounter[Table.top]++;
                Table.brr[Table.top][p->paired.paired[i]] = 1;
                } Table.top++;
            } p = p->next;
        }
    }

/*finds the prime implicant which has the greatest number of minterms
unused at that point and returns the number and row is assigned the row
number*/
int findMaxInTable(int *row) {
    int i, max = -1;
    for(i = 0; i < Table.top; i++) {
        if(Table.mintermCounter[i] > max) {
            *row = i;
            max = Table.mintermCounter[i];
            }
        }
    return max;
    }

//display the prime implicants table
void displayTable() {
    int i,j;
    printf("\n[Tabel Prime Implicants]\n");
    for(i=0; i<Table.top; i++) {
        convertBinaryToMintermNotation(i);
        for(j=0; j<=limit-1; j++) {

            if(Table.brr[i][j]==1)
                printf("   %d  ",j);
            }
        printf("\n");
        }
    }

//Analysing work of the table
void analyseTable() {
    int i,j,k,greatestRow,First=1;
    int essentialPI[limit]; //stores the row number of all essential prime implicants
    int temp,c;
    for(i=0; i<=limit-1; i++)
        essentialPI[i]=-1;
    for(i=0; i<=limit-1; i++) {
        if(mintermsGiven[i]==1) {
            if(numberOfImplicants(i,&temp)==1) {
                essentialPI[i]=temp;
                }
            }
        }
    for(i=0; i<=limit-1; i++) {
        if(essentialPI[i]!=-1) {
            if(First!=1)
                printf(" + ");
            else
                First=0;
            convertBinaryToMintermNotation(essentialPI[i]);

            removeMintermsFromTable(essentialPI[i]);
            for(j=i+1; j<=limit-1; j++) {
                if(essentialPI[j]==essentialPI[i])
                    essentialPI[j]=-1;
                }
            essentialPI[i]=-1;

            }
        }
    while(findMaxInTable(&greatestRow)!=0) {

        if(First!=1)
            printf(" + ");
        else
            First=0;
        convertBinaryToMintermNotation(greatestRow);

        removeMintermsFromTable(greatestRow);
        }
    printf("\b");

    }
