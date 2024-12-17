#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Cari Data -----------------------------------------------------------------------------------------------------------
typedef struct find_node
{
    char idBuku[10];
    char judulBuku[100];
    char penulis[50];
    int jumlahBuku;
    struct find_node *next;
} find_node;

find_node *createfind_node(const char *idBuku, const char *judulBuku, const char *penulis, int jumlahBuku)
{
    find_node *newfind_node = (find_node *)malloc(sizeof(find_node));
    if (!newfind_node)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    strncpy(newfind_node->idBuku, idBuku, 10);
    strncpy(newfind_node->judulBuku, judulBuku, 100);
    strncpy(newfind_node->penulis, penulis, 50);
    newfind_node->jumlahBuku = jumlahBuku;
    newfind_node->next = NULL;
    return newfind_node;
}

void appendfind_node(find_node **find_head, const char *idBuku, const char *judulBuku, const char *penulis, int jumlahBuku)
{
    find_node *newfind_node = createfind_node(idBuku, judulBuku, penulis, jumlahBuku);
    if (*find_head == NULL)
    {
        *find_head = newfind_node;
    }
    else
    {
        find_node *temp = *find_head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newfind_node;
    }
}

void readFile(const char *filename, find_node **find_head)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "Gagal Membuka File: %s\n", filename);
        exit(1);
    }

    char line[256];
    int isFirstLine = 1;
    while (fgets(line, sizeof(line), file))
    {
        if (isFirstLine)
        {
            isFirstLine = 0;
            continue;
        }

        char idBuku[10];
        char judulBuku[100];
        char penulis[50];
        int jumlahBuku;

        if (sscanf(line, "%9[^,],%99[^,],%49[^,],%d", idBuku, judulBuku, penulis, &jumlahBuku) == 4)
        {
            appendfind_node(find_head, idBuku, judulBuku, penulis, jumlahBuku);
        }
        else
        {
            fprintf(stderr, "Warning: Baris Bermasalah, Tolong Check Format CSV: %s\n", line);
        }
    }

    fclose(file);
}

find_node *searchById(find_node *find_head, const char *id)
{
    while (find_head != NULL)
    {
        if (strcmp(find_head->idBuku, id) == 0)
        {
            return find_head;
        }
        find_head = find_head->next;
    }
    return NULL;
}

void displayRecord(find_node *find_node)
{
    if (find_node == NULL)
    {
        printf("Record Tidak Ditemukan\n");
        return;
    }

    printf("---------------------------------------------------------------------------------\n");
    printf("ID Buku        Judul Buku                           Penulis           Jumlah Buku\n");
    printf("---------------------------------------------------------------------------------\n");
    printf("%-12s %-35s %-15s ", find_node->idBuku, find_node->judulBuku, find_node->penulis);
    if (find_node->jumlahBuku == 0)
    {
        printf("Stok Buku Kosong\n");
    }
    else
    {
        printf("%d\n", find_node->jumlahBuku);
    }
    printf("---------------------------------------------------------------------------------\n\n");
}

void displayList(find_node *find_head)
{
    if (find_head == NULL)
    {
        printf("The list is empty.\n");
        return;
    }

    printf("---------------------------------------------------------------------------------\n");
    printf("ID Buku        Judul Buku                           Penulis           Jumlah Buku\n");
    printf("---------------------------------------------------------------------------------\n");

    find_node *temp = find_head;
    while (temp != NULL)
    {
        printf("%-12s %-35s %-15s ", temp->idBuku, temp->judulBuku, temp->penulis);
        if (temp->jumlahBuku == 0)
        {
            printf("Stok Buku Kosong\n");
        }
        else
        {
            printf("%d\n", temp->jumlahBuku);
        }
        temp = temp->next;
    }
    printf("---------------------------------------------------------------------------------\n");
}

void find_freelist(find_node *find_head)
{
    find_node *temp;
    while (find_head != NULL)
    {
        temp = find_head;
        find_head = find_head->next;
        free(temp);
    }
}
// Pilih buku -----------------------------------------------------------------------------------------------------------
void option2()
{
    printf("Option 2 selected.\n");
}
// Pinjam buku -----------------------------------------------------------------------------------------------------------
void option3()
{
    printf("Option 3 selected.\n");
}
// Antrian -----------------------------------------------------------------------------------------------------------
typedef struct queue {
    int count;
    node *front;
    node *rear;
} queue;
void initqueue(queue *q) {
    q->count = 0;
    q->front = NULL;
    q->rear = NULL;
}
void displayQueue(queue *q)
{
    char buffer[255];
    FILE *file = fopen("user_list.csv", "r");
    if (file == NULL) {
        printf("File error, tidak dapat ditemukan!\n");
        return;
    }
    printf("Isi File 'user_list.csv':\n");
    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s", buffer);
    }
    fclose(file);
}
// Riwayat -----------------------------------------------------------------------------------------------------------
typedef struct Node
{
    char nama_user[50];
    char id_buku[10];
    struct Node *left;
    struct Node *right;
} Node;

void tambahRiwayat(Node **head, Node **tail, const char *nama, const char *id)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (!newNode)
    {
        printf("Gagal alokasi memori!\n");
        return;
    }

    strcpy(newNode->nama_user, nama);
    strcpy(newNode->id_buku, id);
    newNode->right = NULL;
    newNode->left = *tail;

    if (*tail != NULL)
    {
        (*tail)->right = newNode;
    }
    *tail = newNode;

    if (*head == NULL)
    {
        *head = newNode;
    }
}

void tampilkanRiwayat(Node *head)
{
    if (head == NULL)
    {
        printf("Riwayat peminjaman kosong.\n");
        return;
    }

    printf("---------------------------------------------------\n");
    printf("Nama Pengguna                 ID Buku\n");
    printf("---------------------------------------------------\n");

    Node *current = head;
    while (current != NULL)
    {
        // Menampilkan nama dan ID buku
        printf("%-30s %-10s\n", current->nama_user, current->id_buku);
        current = current->right;
    }
}

const char *cariIDBuku(Node *head, const char *nama)
{
    Node *current = head;
    while (current != NULL)
    {
        if (strcmp(current->nama_user, nama) == 0)
        {
            return current->id_buku;
        }
        current = current->right;
    }
    return NULL;
}

void loadRiwayat(Node **head, Node **tail)
{
    FILE *file = fopen("user_list.csv", "r");
    if (file == NULL)
    {
        printf("Gagal membuka file user_list.csv\n");
        return;
    }

    char nama[50];
    char id_buku_temp[10];

    while (fscanf(file, " %50[^,],%9s\n", nama, id_buku_temp) == 2)
    {
        tambahRiwayat(head, tail, nama, id_buku_temp);
    }

    fclose(file);
}

void freeList(Node *head)
{
    Node *current = head;
    while (current != NULL)
    {
        Node *temp = current;
        current = current->right;
        free(temp);
    }
}

// Kembalikan buku -----------------------------------------------------------------------------------------------------------
typedef struct BorrowData {
    char nama[50];
    char idBuku[10];
    struct BorrowData* next;
} BorrowData;

BorrowData* createBorrowData(const char* nama, const char* idBuku) {
    BorrowData* newNode = (BorrowData*)malloc(sizeof(BorrowData));
    if (!newNode) {
        fprintf(stderr, "Alokasi Memori Gagal\n");
        exit(1);
    }
    strncpy(newNode->nama, nama, sizeof(newNode->nama) - 1);
    strncpy(newNode->idBuku, idBuku, sizeof(newNode->idBuku) - 1);
    newNode->nama[sizeof(newNode->nama) - 1] = '\0';
    newNode->idBuku[sizeof(newNode->idBuku) - 1] = '\0';
    newNode->next = NULL;
    return newNode;
}

void appendBorrowData(BorrowData** head, const char* nama, const char* idBuku) {
    BorrowData* newNode = createBorrowData(nama, idBuku);
    if (*head == NULL) {
        *head = newNode;
    } else {
        BorrowData* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

void readBorrowFile(const char* filename, BorrowData** head) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Gagal untuk membuka file: %s\n", filename);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char nama[50], idBuku[10];
        if (sscanf(line, "%49[^,],%9s", nama, idBuku) == 2) {
            appendBorrowData(head, nama, idBuku);
        }
    }

    fclose(file);
}

void displayBorrowData(BorrowData* head) {
    if (head == NULL) {
        printf("Tidak Ada Catatan Peminjaman yang tersedia.\n");
        return;
    }

    printf("\nDaftar Buku yang Dipinjam:\n");
    printf("-------------------------------\n");
    printf("Nama           :   ID Buku\n");
    printf("-------------------------------\n");
    BorrowData* temp = head;
    while (temp != NULL) {
        printf("%-15s :   %s\n", temp->nama, temp->idBuku);
        temp = temp->next;
    }
    printf("-------------------------------\n");
}

void saveBorrowData(const char* filename, BorrowData* head) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Gagal untuk membuka file: %s\n", filename);
        return;
    }

    BorrowData* temp = head;
    while (temp != NULL) {
        fprintf(file, "%s,%s\n", temp->nama, temp->idBuku);
        temp = temp->next;
    }

    fclose(file);
}

void updateJumlahBuku(const char* dataFilename, const char* idBuku) {
    FILE *file = fopen(dataFilename, "r");
    FILE *tempFile = fopen("temp.csv", "w");

    if (!file || !tempFile) {
        fprintf(stderr, "Gagal Membuka file untuk pembaruan operasi\n");
        if (file) fclose(file);
        if (tempFile) fclose(tempFile);
        return;
    }

    char line[256];
    int updated = 0;

    while (fgets(line, sizeof(line), file)) {
        char tempId[10], title[100], author[50];
        int jumlahBuku;

        if (sscanf(line, "%9[^,],%99[^,],%49[^,],%d", tempId, title, author, &jumlahBuku) == 4) {
            if (strcmp(tempId, idBuku) == 0) {
                jumlahBuku++; 
                updated = 1;
            }
            fprintf(tempFile, "%s,%s,%s,%d\n", tempId, title, author, jumlahBuku);
        } else {
            fprintf(tempFile, "%s", line); 
        }
    }

    fclose(file);
    fclose(tempFile);

    if (updated) {
        remove(dataFilename);
        rename("temp.csv", dataFilename);
        printf("Memperbarui jumlahBuku untuk ID %s di %s.\n", idBuku, dataFilename);
    } else {
        remove("temp.csv");
        printf("ID Buku %s Tidak Ditemukan di %s.\n", idBuku, dataFilename);
    }
}

void returnBorrowedBook(BorrowData** head, const char* idBuku, const char* returnFile, const char* dataFile) {
    BorrowData *temp = *head, *prev = NULL;

    while (temp != NULL && strcmp(temp->idBuku, idBuku) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Catatan Peminjam dengan ID %s tidak ditemukan.\n", idBuku);
        return;
    }

    FILE* file = fopen(returnFile, "a");
    if (!file) {
        fprintf(stderr, "Gagal untuk membuka file: %s\n", returnFile);
        return;
    }
    fprintf(file, "%s,%s\n", temp->nama, temp->idBuku);
    fclose(file);

    if (prev == NULL) {

        *head = temp->next;
    } else {
        prev->next = temp->next;
    }

    free(temp);

    updateJumlahBuku(dataFile, idBuku);

    printf("Catatan dengan ID %s telah dikembalikan dan diperbarui.\n", idBuku);
}

//----------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------
int main()
{
    // Cari Data --------------------------------------------------------------------------------------------------
    find_node *find_head = NULL;
    const char *filename = "data.csv";

    readFile(filename, &find_head);

    int find_choice;

    char id[10];
    // Pilih buku --------------------------------------------------------------------------------------------------

    // Pinjam buku --------------------------------------------------------------------------------------------------

    // Antrian --------------------------------------------------------------------------------------------------
    char buffer[50];
    queue antrian;
    initqueue(&antrian);
    // Riwayat --------------------------------------------------------------------------------------------------
    Node *head = NULL;
    Node *tail = NULL;
    loadRiwayat(&head, &tail);
    int pilihan_riwayat;

    char input_nama[50];
    const char *id_buku_result = NULL;
    // Kembalikan Buku  --------------------------------------------------------------------------------------------------
    BorrowData* borrowList = NULL;
    const char* borrowFile = "user_list.csv";
    const char* returnFile = "pengembalian.csv";
    const char* dataFile = "data.csv";

    readBorrowFile(borrowFile, &borrowList);

    int return_choice;
    char idBuku[10];
    // Main ----------------------------------------------------------------------------------------------------------
    int choice;
    int pilihan;
    //--------------------------------------------------------------------------------------------------------
    while (1)
    {
        printf("Menu:\n");
        printf("1. Cari buku\n");
        printf("2. Pilih buku\n");
        printf("3. Pinjam buku\n");
        printf("4. Antrian\n");
        printf("5. Riwayat\n");
        printf("6. Kembalikan buku\n");
        printf("7. Keluar\n");
        printf("Masukkan Pilihan:");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("\nCari Buku\n");
            printf("-------------------------\n");
            printf("1. Tampilkan Semua Buku\n");
            printf("2. Cari Buku\n");
            printf("3. Keluar\n");
            printf("-------------------------\n");
            printf("Tips: \nEdit Data.csv Untuk Menambahkan atau Menghapus Data Buku\n");
            printf("-------------------------\n");
            printf("Masukkan Pilihan: ");
            scanf("%d", &find_choice);

            switch (find_choice)
            {
            case 1:
                printf("Data Buku:\n");
                displayList(head);
                break;
            case 2:
                printf("Masukkan ID Buku: ");
                scanf("%s", id);
                find_node *foundNode = searchById(head, id);
                displayRecord(foundNode);
                break;
            case 3:
                freeList(head);
                printf("Keluar...\n");
                return 0;
            default:
                printf("Masukkan Pilihan 1, 2 Atau 3.\n");
            }
            break;
        case 2:
            option2();
            break;
        case 3:
            option3();
            break;
        case 4:
            printf("\n Menu Antrian \n");
            printf(" 1. Tampilkan Antrian : \n");
            printf(" 2. Keluar. \n");
            scanf("%d",&choice);

            switch(choice){

                case 1 : 
                    displayQueue(&antrian);
                    break;
                
                case 2 : 
                    printf("Keluar dari program.\n");
                    break;
                
                default :
                    printf("Pilihan Tidak Valid!");
                    break;     
            }
        case 5:
            printf("\nMenu Riwayat:\n");
            printf("1. Tampilkan seluruh riwayat peminjaman\n");
            printf("2. Cari ID buku berdasarkan nama pengguna\n");
            printf("3. Keluar\n");
            printf("Masukkan pilihan (1-3): ");
            scanf("%d", &pilihan);
            getchar(); // Menghapus karakter newline dari buffer

            switch (pilihan_riwayat)
            {
            case 1:
                tampilkanRiwayat(head);
                break;

            case 2:
                printf("Masukkan nama pengguna untuk mencari ID buku: ");
                fgets(input_nama, sizeof(input_nama), stdin);
                input_nama[strcspn(input_nama, "\n")] = 0;
                id_buku_result = cariIDBuku(head, input_nama);

                if (id_buku_result == NULL)
                {
                    printf("Nama pengguna tidak ditemukan dalam riwayat peminjaman.\n");
                }
                else
                {
                    printf("ID Buku yang dipinjam oleh %s adalah: %s\n", input_nama, id_buku_result);
                }
                break;

            case 3:
                freeList(head);
                printf("Keluar dari program.\n");
                return 0;

            default:
                printf("Pilihan tidak valid! Kembali ke menu utama.\n");
                break;
            }
        case 6:
            printf("\nMenu:\n");
            printf("1. Tampilkan seluruh buku yang dipinjam\n");
            printf("2. Mengembalikan Buku\n");
            printf("3. Keluar\n");
            printf("Masukan Pilihan Anda: ");
            if (scanf("%d", &return_choice) != 1) {
                printf("Invalid input. Masukan Angka.\n");
                while (getchar() != '\n'); 
                continue;
            }

            switch (return_choice) {
                case 1: {

                    displayBorrowData(borrowList);
                    break;
                }
                case 2: {

                    printf("Masukan ID buku untuk pengembalian: ");
                    scanf("%s", idBuku);

                    returnBorrowedBook(&borrowList, idBuku, returnFile, dataFile); 
                    saveBorrowData(borrowFile, borrowList); 
                    break;
                }
                case 3: {

                    printf("Keluar Dari Program. Membersihkan memori...\n");
                    BorrowData* temp;
                    while (borrowList != NULL) {
                        temp = borrowList;
                        borrowList = borrowList->next;
                        free(temp);
                    }
                    return 0;
            }
            default:
                printf("Pilihan Tidak Valid. Tolong Coba Lagi.\n");
        }
        case 7:
            printf("Exiting...\n");
            exit(0);
        default:
            printf("Pilihan Salah.\n");
    }
}

return 0;
}