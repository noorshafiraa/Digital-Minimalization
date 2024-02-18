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

int main(void) {

    //initiate variables
    int i,temp, dontCare = 0, dontCareNumber;
    maxGroup = -1;
    newMaxGroup = -1;

    printf("Masukkan banyak minterms (selain dont'care. max : 256) > ");
    scanf("%d",&minterms);
    //conditional jika input tidak memiliki minterm or invalid input (minus minterms)
    if (minterms == 0 || minterms < 0 || (minterms % 1 != 0) || minterms > limit)
    {
        printf("Error : input invalid!");
        return 1;
    }

    //input tidak melebihi maksimal
    for (i = 0; i < limit; i++)
    {
        mintermsGiven[i] = -1; //-1 inisial nilai tidak ada
        dontCares[i] = -1;
    }

    printf("\nSebutkan minterms (tanpa don't care)>\n");
    for (i = 0; i < minterms; i++) 
    {
        printf("Masukkan minterms ke-%d > ", i+1);
        scanf("%d",&temp);
        if (temp < 0 || ((temp % 1) != 0) || temp > limit)
        {
        printf("Error : input invalid!");
        return 1;
        }

        mintermsGiven[temp] = 1; //menggunakan SOP maka minterm bernilai 1
        add(temp);
    }

    printf("Apakah terdapat don't care? (Yes = 1, No = 0) > ");
    scanf("%d",&dontCare);

    if (dontCare == 1)
    {
        printf("\nMasukkan jumlah don't care> ");
        scanf("%d",&dontCareNumber);
        //conditional jika input tidak memiliki dont care or invalid input (minus dont care)
        if (dontCareNumber == 0 || dontCareNumber < 0 || (dontCareNumber % 1 != 0) || dontCareNumber > limit)
        {
            printf("Error : input invalid!");
            return 1;
        }

        printf("\nSebutkan minterms dont care>\n");
        for (i = 0; i < dontCareNumber; i++) 
        {
            printf("Masukkan don't care ke-%d > ", i+1);
            scanf("%d",&temp);
            if (temp < 0 || ((temp % 1) != 0) || temp > limit)
            {
            printf("Error : input invalid!");
            return 1;
            }
            dontCares[temp]=1;
            add(temp);
        }
    }

    if (dontCare != 1 && dontCare != 0){
        printf("Error : Invalid input!");
        return 1;
    }

    Table.top=0;
    initTable();        //Inisialisasi tabel prime implicant dengan inisial nilai -1
    pair();             //Proses pemasangan
    displayTable();     //Menampilkan tabel prime implicant

    printf("Hasil minimisasi logika yaitu : ");
    analyseTable(); //Analisis tabel prime implicant
    return 1;
    }
