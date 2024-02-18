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

//checks if a particular minterm is present in  a particular implicant row
int ifMintermPresentInImplicant(int minterm, int implicant) {
    if(Table.brr[implicant][minterm] == 1)
        return 1;
    else
        return 0;
    }

/*given a implicant row it deletes all the minterms from it as
well as delete all the minterms from that respective columns too...*/
void removeMintermsFromTable(int n) {
    int i,j;
    for(i=0; i<=limit-1; i++) {
        if(Table.brr[n][i]==1) {
            mintermsGiven[i]=-1;

            for(j=0; j<Table.top; j++) {
                if(Table.brr[j][i]==1) {
                    Table.brr[j][i]=-1;
                    Table.mintermCounter[j]--;
                    }
                }
            }
        }
    }

//returns in how many implicants a particular minterm is present
int numberOfImplicants(int n,int *temp) {
    int i,j;
    int count=0;
    for(i=0; i<Table.top; i++) {
        if(Table.brr[i][n]==1) {
            j=i;
            count++;
            }
        }
    *temp=j;
    return count;
    }

//converts and prints the binary into a variable notation
void convertBinaryToMintermNotation(int n) {
    int c=0;
    char UppercaseChar[]= {'A','B','C','D','E','F','G','H'};
    char LowercaseChar[]= {'a','b','c','d','e','f','g','h'}; //NOT
    while(c!=bitsSize) {
        if(Table.arr[n][c]!=-1) {
            if(Table.arr[n][c]==1)
                printf("%c",UppercaseChar[c]);
            else
                printf("%c",LowercaseChar[c]);
            }
        c++;
        }
    }
