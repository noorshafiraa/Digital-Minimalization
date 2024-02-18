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
