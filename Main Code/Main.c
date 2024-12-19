#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

void option2()
{
    printf("Option 2 selected.\n");
}

typedef struct Book {
    char id_buku[5];
    char judul_buku[100];
    char penulis[50];
    int jumlah_buku;
    struct Book *next;
} Book;
typedef struct node {
    struct node *next;
    char nama_user[50];
    int id_buku;
} node;

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

void enqueue(queue *q, char nama_user[50], int id_buku) {

    node *newNode = (node *)malloc(sizeof(node));
    if (newNode == NULL) {
        printf("Alokasi memori gagal!\n");
        return;
    }
    strcpy(newNode->nama_user, nama_user);
    newNode->id_buku = id_buku;
    newNode->next = NULL;

    if (q->rear == NULL) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
    q->count++;
}
// Fungsi untuk memuat data buku dari CSV ke dalam Linked List
Book *load_books_from_csv(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: File tidak dapat dibuka.\n");
        return NULL;
    }

    Book *head = NULL, *current = NULL;
    char line[256];

    while (fgets(line, sizeof(line), file)) {
        Book *newBook = (Book *)malloc(sizeof(Book));
        if (!newBook) {
            printf("Error: Gagal alokasi memori untuk buku.\n");
            fclose(file);
            return NULL;
        }
        sscanf(line, "%4[^,],%99[^,],%49[^,],%d",
               newBook->id_buku, newBook->judul_buku,
               newBook->penulis, &newBook->jumlah_buku);
        newBook->next = NULL;

        if (head == NULL) {
            head = current = newBook;
        } else {
            current->next = newBook;
            current = newBook;
        }
    }
    fclose(file);
    return head;
}

// Fungsi untuk menyimpan Linked List kembali ke file CSV
void save_books_to_csv(const char *filename, Book *head) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error: Tidak dapat membuka file untuk menyimpan data.\n");
        return;
    }

    Book *current = head;
    while (current) {
        fprintf(file, "%s,%s,%s,%d\n",
                current->id_buku, current->judul_buku,
                current->penulis, current->jumlah_buku);
        current = current->next;
    }
    fclose(file);
}

// Fungsi untuk mengurangi stok buku berdasarkan ID buku
int reduce_stock(Book *head, const char *id_buku) {
    Book *current = head;
    while (current) {
        if (strcmp(current->id_buku, id_buku) == 0) {
            if (current->jumlah_buku > 0) {
                current->jumlah_buku--;
                return 1; // Stok berhasil dikurangi
            } else {
                printf("Stok buku \"%s\" habis!\n", current->judul_buku);
                return 0; // Stok habis
            }
        }
        current = current->next;
    }
    printf("Buku dengan ID %s tidak ditemukan!\n", id_buku);
    return 0; // Buku tidak ditemukan
}
void displayQueue(queue *q)
{
    char buffer[255];
    FILE *file = fopen("user_list.csv", "r");
    if (file == NULL) {
        printf("File error, tidak dapat ditemukan!\n");
        return;
    }
    printf("List Antrian:\n");
    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s", buffer);
    }
    fclose(file);
}

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

int main()
{

    find_node *find_head = NULL;
    const char *filename = "data.csv";

    readFile(filename, &find_head);

    int find_choice;

    char id[10];


    Book *book_list = load_books_from_csv("data.csv");
    if (!book_list) return 1;
    char id_buku[5];
    char nama_user[50];

    queue antrian;
    initqueue(&antrian);
    int pinjam_choice;

    char buffer[50];
    int antri_choice;

    Node *head = NULL;
    Node *tail = NULL;
    loadRiwayat(&head, &tail);
    int pilihan_riwayat;

    char input_nama[50];
    const char *id_buku_result = NULL;

    BorrowData* borrowList = NULL;
    const char* borrowFile = "user_list.csv";
    const char* returnFile = "pengembalian.csv";
    const char* dataFile = "data.csv";

    readBorrowFile(borrowFile, &borrowList);

    int return_choice;
    char idBuku[10];

    int choice;
    int pilihan;

    while (1)
    {
        printf("Menu:\n");
        printf("1. Cari buku\n");
        printf("2. Pinjam buku\n");
        printf("3. Antrian\n");
        printf("4. Riwayat\n");
        printf("5. Kembalikan buku\n");
        printf("6. Keluar\n");
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
                displayList(find_head);
                break;
            case 2:
                printf("Masukkan ID Buku: ");
                scanf("%s", id);
                find_node *foundNode = searchById(find_head, id);
                displayRecord(foundNode);
                break;
            case 3:
                freeList(find_head);
                printf("Keluar...\n");
                break;
            default:
                printf("Masukkan Pilihan 1, 2 Atau 3.\n");
            }
            break;
       case 2:
            printf("\n=== Menu Peminjaman Buku ===\n");
            printf("1. Pinjam Buku\n");
            printf("2. Keluar\n");
            printf("Pilihan Anda: ");
            scanf("%d", &pinjam_choice);
             getchar(); 

            switch (pinjam_choice) {
                case 1:
                printf("Nama user: ");
                fgets(nama_user, sizeof(nama_user), stdin);
                nama_user[strcspn(nama_user, "\n")] = '\0'; // Hapus newline

                printf("ID buku yang mau dipinjam: ");
                scanf("%s", id_buku);

                // Kurangi stok buku jika tersedia
                if (reduce_stock(book_list, id_buku)) {
                // Tambahkan ke antrian peminjaman
                enqueue(&antrian, nama_user, id_buku);
                 printf("Buku berhasil dipinjam oleh %s (ID Buku: %s)\n", nama_user, id_buku);

                // Simpan data buku yang telah diperbarui ke file CSV
                save_books_to_csv("data.csv", book_list);

                // Simpan data peminjaman ke file user_list.csv
                FILE *file = fopen("user_list.csv", "a");
                if (file) {
                 fprintf(file, "%s,%s\n", nama_user, id_buku);
                 fclose(file);
                } else {
                 printf("Error: Tidak dapat membuka file user_list.csv\n");
                }
                } else {
                printf("Stok buku tidak mencukupi atau ID buku tidak valid.\n");
                }

                printf("Antrian peminjaman: %d. %s\n", antrian.count, antrian.front->nama_user);

                // Bersihkan memori Linked List
                Book *current = book_list;
                while (current) {
                Book *temp = current;
                current = current->next;
                free(temp);
                }
                break;

                case 2:
                    printf("\nKeluar dari menu peminjaman.\n");
                    break;

                default:
                    printf("\nPilihan tidak valid! Silakan pilih 1 atau 2.\n");
                    break;
            }
            break;

        case 3:
            printf("\n Menu Antrian \n");
            printf(" 1. Tampilkan Antrian : \n");
            printf(" 2. Keluar. \n");
            scanf("%d",&antri_choice);
            getchar(); 
            switch(antri_choice){

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
            break;
        case 4:
            printf("\nMenu Riwayat:\n");
            printf("1. Tampilkan seluruh riwayat peminjaman\n");
            printf("2. Cari ID buku berdasarkan nama pengguna\n");
            printf("3. Keluar\n");
            printf("Masukkan pilihan (1-3): ");
            scanf("%d", &pilihan_riwayat);
            getchar(); 

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
                break;

            default:
                printf("Pilihan tidak valid! Kembali ke menu utama.\n");
                break;
            }
            break;
        case 5:
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
                    break;
            }
            default:
                printf("Pilihan Tidak Valid. Tolong Coba Lagi.\n");
        }
        break;
        case 7:
            printf("Exiting...\n");
            exit(0);
        default:
            printf("Pilihan Salah.\n");
    }
}

return 0;
}
