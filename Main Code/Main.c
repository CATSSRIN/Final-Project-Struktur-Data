#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Cari Buku -------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef struct find_node
{
    char idBuku[10];
    char judulBuku[100];
    char penulis[50];
    int jumlahBuku;
    int stock; 
    struct find_node *next;
} find_node;

find_node *createfind_node(const char *idBuku, const char *judulBuku, const char *penulis, int jumlahBuku, int stock)
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
    newfind_node->stock = stock; 
    newfind_node->next = NULL;
    return newfind_node;
}

void appendfind_node(find_node **find_head, const char *idBuku, const char *judulBuku, const char *penulis, int jumlahBuku, int stock)
{
    find_node *newfind_node = createfind_node(idBuku, judulBuku, penulis, jumlahBuku, stock);
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
        int jumlahBuku, stock;

        if (sscanf(line, "%9[^,],%99[^,],%49[^,],%d,%d", idBuku, judulBuku, penulis, &jumlahBuku, &stock) == 5)
        {
            appendfind_node(find_head, idBuku, judulBuku, penulis, jumlahBuku, stock);
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

    printf("-----------------------------------------------------------------------------------------\n");
    printf("ID Buku        Judul Buku                           Penulis           Jumlah Buku   Stock\n");
    printf("-----------------------------------------------------------------------------------------\n");
    printf("%-12s %-35s %-15s %-14d ", find_node->idBuku, find_node->judulBuku, find_node->penulis, find_node->jumlahBuku);
    if (find_node->stock == 0)
    {
        printf("Stok Buku Kosong\n");
    }
    else
    {
        printf("%d\n", find_node->stock);
    }
    printf("--------------------------------------------------------------------------------------------------\n\n");
}

void displayList(find_node *find_head)
{
    if (find_head == NULL)
    {
        printf("The list is empty.\n");
        return;
    }

    printf("--------------------------------------------------------------------------------------------------\n");
    printf("ID Buku        Judul Buku                           Penulis           Jumlah Buku   Stock\n");
    printf("--------------------------------------------------------------------------------------------------\n");

    find_node *temp = find_head;
    while (temp != NULL)
    {
        printf("%-12s %-35s %-15s %-14d ", temp->idBuku, temp->judulBuku, temp->penulis, temp->jumlahBuku);
        if (temp->stock == 0)
        {
            printf("Stok Buku Kosong\n");
        }
        else
        {
            printf("%d\n", temp->stock);
        }
        temp = temp->next;
    }
    printf("---------------------------------------------------------------------------------------------------\n");
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

// Peminjaman -------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef struct Book {
    char id_buku[5];
    char judul_buku[100];
    char penulis[50];
    int jumlah_buku;
    int stock;
    struct Book *next;
} Book;

typedef struct node {
    struct node *next;
    char nama_user[50];
    char id_buku[5];
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

void enqueue(queue *q, char nama_user[50], char id_buku[5]) {
    node *newNode = (node *)malloc(sizeof(node));
    if (newNode == NULL) {
        printf("Alokasi memori gagal!\n");
        return;
    }
    strcpy(newNode->nama_user, nama_user);
    strcpy(newNode->id_buku, id_buku);
    newNode->next = NULL;

    if (q->rear == NULL) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
    q->count++;
}

Book *load_books_from_csv(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: File tidak dapat dibuka.\n");
        return NULL;
    }

    Book *head = NULL, *current = NULL;
    char line[256];

    fgets(line, sizeof(line), file); 

    while (fgets(line, sizeof(line), file)) {

        if (line[0] == '\0' || line[0] == '\n') {
            continue; 
        }

        Book *newBook = (Book *)malloc(sizeof(Book));
        if (!newBook) {
            printf("Error: Gagal alokasi memori untuk buku.\n");
            fclose(file);
            return NULL;
        }

        int itemsParsed = sscanf(line, "%4[^,],%99[^,],%49[^,],%d,%d",
                                  newBook->id_buku, newBook->judul_buku,
                                  newBook->penulis, &newBook->jumlah_buku, 
                                  &newBook->stock);

        if (itemsParsed != 5) {
            free(newBook);
            continue; 
        }

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

void save_books_to_csv(const char *filename, Book *head) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error: Tidak dapat membuka file untuk menyimpan data.\n");
        return;
    }

    fprintf(file, "idBuku,judulBuku,penulis,jumlahBuku,stock\n");

    Book *current = head;
    while (current) {

        if (current->id_buku[0] == '\0' || current->judul_buku[0] == '\0') {
            continue; 
        }

        fprintf(file, "%s,%s,%s,%d,%d\n",
                current->id_buku, current->judul_buku,
                current->penulis, current->jumlah_buku,
                current->stock);  
        current = current->next;
    }

    fclose(file);
}

int reduce_stock(Book *head, const char *id_buku) {
    Book *current = head;
    while (current) {
        if (strcmp(current->id_buku, id_buku) == 0) {
            if (current->stock > 0) {
                current->stock--;
                return 1; 
            } else {
                printf("Stok buku \"%s\" habis!\n", current->judul_buku);
                return 0; 
            }
        }
        current = current->next;
    }
    printf("Buku dengan ID %s tidak ditemukan!\n", id_buku);
    return 0; 
}

void save_user_to_csv(const char *filename, const char *nama_user, const char *id_buku) {
    FILE *file = fopen(filename, "a");  
    if (file) {
        fprintf(file, "%s,%s\n", nama_user, id_buku);
        fclose(file);
    } else {
        printf("Error: Tidak dapat membuka file %s untuk menyimpan data peminjam.\n", filename);
    }
}

// Antrian  -------------------------------------------------------------------------------------------------------------------------------------------------------------
void displayQueue(queue *q)
{
    char buffer[255];
    FILE *file = fopen("user_list.csv", "r");
    if (file == NULL)
    {
        printf("File error, tidak dapat ditemukan!\n");
        return;
    }
    printf("List Antrian:\n");
    while (fgets(buffer, sizeof(buffer), file))
    {
        printf("%s", buffer);
    }
    fclose(file);
}

// Riwayat  -------------------------------------------------------------------------------------------------------------------------------------------------------------
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

const char *cariIDBuku(const char *id_buku)
{
    FILE *file = fopen("user_list.csv", "r");
    if (file == NULL)
    {
        printf("Gagal membuka file user_list.csv\n");
        return NULL;
    }

    static char nama_user[50]; 
    char id_buku_temp[10];

    while (fscanf(file, " %49[^,],%9s", nama_user, id_buku_temp) == 2)
    {
        if (strcmp(id_buku_temp, id_buku) == 0)
        {
            fclose(file);
            return nama_user;
        }
    }

    fclose(file);
    return NULL; 
}

void loadRiwayat(Node **head, Node **tail)
{
    FILE *file = fopen("riwayat.csv", "r");
    if (file == NULL)
    {
        printf("Gagal membuka file user_list.csv\n");
        return;
    }

    char nama[50];
    char id_buku_temp[10];

    while (fscanf(file, " %49[^,],%9s", nama, id_buku_temp) == 2)
    {

        if (strlen(nama) > 0 && strlen(id_buku_temp) > 0)
        {
            tambahRiwayat(head, tail, nama, id_buku_temp);
        }
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

// Pengembalian -------------------------------------------------------------------------------------------------------------------------------------------------------------
#define MAX_LINE_LENGTH 256

void displayUsers() {
    FILE *file = fopen("user_list.csv", "r");
    if (file == NULL) {
        printf("Error opening user_list.csv\n");
        return;
    }

    char line[MAX_LINE_LENGTH];
    printf("Tampilkan Peminjaman:\n");
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
}

void returnBook(const char *borrower) {
    FILE *userFile = fopen("user_list.csv", "r");
    FILE *tempUserFile = fopen("temp_user_list.csv", "w");
    FILE *dataFile = fopen("data.csv", "r");
    FILE *tempDataFile = fopen("temp_data.csv", "w");
    FILE *historyFile = fopen("riwayat.csv", "a");

    if (!userFile || !tempUserFile || !dataFile || !tempDataFile || !historyFile) {
        printf("Error opening file\n");
        return;
    }

    char userLine[MAX_LINE_LENGTH];
    char bookId[MAX_LINE_LENGTH];
    char line[MAX_LINE_LENGTH];
    int bookFound = 0;
    int borrowerFound = 0;

    while (fgets(userLine, sizeof(userLine), userFile)) {
        char name[MAX_LINE_LENGTH];
        sscanf(userLine, "%[^,]", name); 

        if (strcmp(name, borrower) != 0) {
            fprintf(tempUserFile, "%s", userLine); 
        } else {

            sscanf(userLine, "%*[^,],%s", bookId);
            fprintf(historyFile, "%s,%s\n", borrower, bookId); 
            borrowerFound = 1;
            bookFound = 1; 
        }
    }

    if (!borrowerFound) {
        printf("Peminjam tidak ada\n");
        fclose(userFile);
        fclose(tempUserFile);
        fclose(dataFile);
        fclose(tempDataFile);
        fclose(historyFile);
        remove("temp_user_list.csv");
        remove("temp_data.csv");
        return;
    }

    int firstLine = 1;  
    if (bookFound) {
        while (fgets(line, sizeof(line), dataFile)) {
            if (firstLine) {

                fprintf(tempDataFile, "%s", line);
                firstLine = 0;
                continue;
            }

            char idBuku[MAX_LINE_LENGTH];
            char judulBuku[MAX_LINE_LENGTH];
            char penulis[MAX_LINE_LENGTH];
            int jumlahBuku, stock;

            sscanf(line, "%[^,],%[^,],%[^,],%d,%d", idBuku, judulBuku, penulis, &jumlahBuku, &stock);

            if (strcmp(idBuku, bookId) == 0) {
                stock++;  
            }

            fprintf(tempDataFile, "%s,%s,%s,%d,%d\n", idBuku, judulBuku, penulis, jumlahBuku, stock);
        }
    }

    fclose(userFile);
    fclose(tempUserFile);
    fclose(dataFile);
    fclose(tempDataFile);
    fclose(historyFile);

    remove("user_list.csv");
    rename("temp_user_list.csv", "user_list.csv");

    remove("data.csv");
    rename("temp_data.csv", "data.csv");

    printf("Buku telah dikembalikan dan stok diperbarui.\n");
}

int main()
{
// Cari Buku
    find_node *find_head = NULL;
    const char *filename = "data.csv";

    readFile(filename, &find_head);

    int find_choice;

    char id[10];

// Peminjaman dan Antrian
    Book *book_list = load_books_from_csv("data.csv");
    if (!book_list) return 1;
    queue antrian;
    initqueue(&antrian);

    char nama_user[50];
    char id_buku[5];
    int pinjam_choice;

    char buffer[50];
    int antri_choice;

// Riwayat
    Node *head = NULL;
    Node *tail = NULL;
    loadRiwayat(&head, &tail);
    int pilihan_riwayat;

    char input_id_buku[50];
    const char *id_buku_result = NULL;

// Pengembalian
    int return_choice;

// Main
    int choice;

    while (1)
    {
        printf("Menu:\n");
        printf("1. Cari buku\n");
        printf("2. Pinjam buku\n");
        printf("3. Antrian\n");
        printf("4. Kembalikan buku\n");
        printf("5. Riwayat\n");
        printf("6. Keluar\n");
        printf("Masukkan Pilihan:");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("\nCari Buku\n");
            printf("-------------------------\n");
            printf("1. Tampilkan Semua Buku\n");
            printf("2. Cari Buku Menggunakan ID Buku\n");
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

            switch (pinjam_choice)
            {
            case 1:

            printf("Nama user: ");
            fgets(nama_user, sizeof(nama_user), stdin);
            nama_user[strcspn(nama_user, "\n")] = '\0'; 

            printf("ID buku yang mau dipinjam: ");
            scanf("%s", id_buku);

            if (reduce_stock(book_list, id_buku)) {
                enqueue(&antrian, nama_user, id_buku);
                printf("Buku berhasil dipinjam oleh %s (ID Buku: %s)\n", nama_user, id_buku);

                Book *current = book_list;
                while (current) {
                    if (strcmp(current->id_buku, id_buku) == 0) {
                        printf("Stok buku setelah pengurangan: %d\n", current->stock);
                    }
                    current = current->next;
                }

                save_books_to_csv("data.csv", book_list);

                save_user_to_csv("user_list.csv", nama_user, id_buku);

            }

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
            printf(" 1. Tampilkan Antrian \n");
            printf(" 2. Keluar. \n");
            printf("Masukkan Pilihan:");
            scanf("%d", &antri_choice);
            getchar();
            switch (antri_choice)
            {

            case 1:
                displayQueue(&antrian);
                break;

            case 2:
                printf("Keluar dari program.\n");
                break;

            default:
                printf("Pilihan Tidak Valid!");
                break;
            }
            break;
        case 5:
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
                printf("Masukkan ID buku untuk mencari peminjam terakhir: ");
                fgets(input_id_buku, sizeof(input_id_buku), stdin);
                input_id_buku[strcspn(input_id_buku, "\n")] = 0;

                id_buku_result = cariIDBuku(input_id_buku); 

                if (id_buku_result == NULL)
                {
                    printf("ID buku tidak ditemukan, cek kembali.\n");
                }
                else
                {
                    printf("Peminjam dari ID buku %s adalah: %s\n", input_id_buku, id_buku_result);
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
        case 4:
            printf("1. Tampilkan Peminjaman\n");
            printf("2. Kembalikan Buku\n");
            printf("3. Exit\n");
            printf("Pilih opsi: ");
            scanf("%d", &return_choice);
            getchar(); 

            switch (return_choice) {
                case 1:
                    displayUsers();
                    break;
                case 2:
                    {
                        char borrower[MAX_LINE_LENGTH];
                        printf("Masukkan Nama Peminjam: ");
                        fgets(borrower, sizeof(borrower), stdin);
                        borrower[strcspn(borrower, "\n")] = '\0'; 
                        returnBook(borrower);
                    }
                    break;
                case 3:
                    printf("Keluar dari program.\n");
                    break;
                default:
                    printf("Opsi tidak valid. Silakan coba lagi.\n");
            }
            break;
        case 6:
            printf("Exiting...\n");
            return 0;
        default:
            printf("Pilihan Salah.\n");
        }
    }

    return 0;
}
