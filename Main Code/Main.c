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
void option4()
{
    printf("Option 3 selected.\n");
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
void option6()
{
    printf("Option 3 selected.\n");
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

    // Riwayat --------------------------------------------------------------------------------------------------
    Node *head = NULL;
    Node *tail = NULL;
    loadRiwayat(&head, &tail);

    char input_nama[50];
    const char *id_buku_result = NULL;
    // Main ----------------------------------------------------------------------------------------------------------
    int choice;
    int pilihan;
    int pilihan_riwayat;
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
            option4();
            break;
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
    case 7:
        printf("Exiting...\n");
        exit(0);
    default:
        printf("Pilihan Salah.\n");
    }
}

return 0;
}
