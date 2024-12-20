#include <stdio.h>
#include <stdlib.h>

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

// Fungsi untuk menginisialisasi antrian
void initqueue(queue *q) {
    q->count = 0;
    q->front = NULL;
    q->rear = NULL;
}

// Fungsi untuk menambahkan elemen ke dalam antrian
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
        sscanf(line, "%4[^,],%99[^,],%49[^,],%d,%d",
               newBook->id_buku, newBook->judul_buku,
               newBook->penulis, &newBook->jumlah_buku, 
               &newBook->stock);
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
        fprintf(file, "%s,%s,%s,%d,%d\n",
                current->id_buku, current->judul_buku,
                current->penulis, current->jumlah_buku,
                current->stock);  // Menyimpan stok yang telah diperbarui
        current = current->next;
    }
    fclose(file);
}

// Fungsi untuk mengurangi stok buku berdasarkan ID buku
int reduce_stock(Book *head, const char *id_buku) {
    Book *current = head;
    while (current) {
        if (strcmp(current->id_buku, id_buku) == 0) {
            if (current->stock > 0) {
                current->stock--;
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

// Fungsi untuk menambahkan data peminjam ke dalam file user_list.csv
void save_user_to_csv(const char *filename, const char *nama_user, const char *id_buku) {
    FILE *file = fopen(filename, "a");  // Mode append
    if (file) {
        fprintf(file, "%s,%s\n", nama_user, id_buku);
        fclose(file);
    } else {
        printf("Error: Tidak dapat membuka file %s untuk menyimpan data peminjam.\n", filename);
    }
}


