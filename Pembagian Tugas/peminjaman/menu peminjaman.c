#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struktur untuk menyimpan data buku
typedef struct Book {
    char id_buku[5];
    char judul_buku[100];
    char penulis[50];
    int jumlah_buku;
    struct Book *next;
} Book;

// Struktur untuk menyimpan antrian peminjam
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

// Fungsi untuk inisialisasi antrian
void initqueue(queue *q) {
    q->count = 0;
    q->front = NULL;
    q->rear = NULL;
}

// Fungsi untuk menambahkan peminjam ke antrian
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

int main() {
    Book *book_list = load_books_from_csv("data.csv");
    if (!book_list) return 1;

    queue antrian;
    initqueue(&antrian);

    char nama_user[50];
    char id_buku[5];

    printf("Nama user: ");
    fgets(nama_user, sizeof(nama_user), stdin);
    nama_user[strcspn(nama_user, "\n")] = '\0'; // Hapus newline

    printf("ID buku yang mau dipinjam: ");
    scanf("%s", id_buku);

    // Kurangi stok buku jika tersedia
    if (reduce_stock(book_list, id_buku)) {
        enqueue(&antrian, nama_user, id_buku);
        printf("Buku berhasil dipinjam oleh %s (ID Buku: %s)\n", nama_user, id_buku);

        // Simpan data buku yang telah diperbarui ke file
        save_books_to_csv("data.csv", book_list);

        // Simpan data peminjaman ke file user_list.csv
        FILE *file = fopen("user_list.csv", "a");
        if (file) {
            fprintf(file, "%s,%s\n", nama_user, id_buku);
            fclose(file);
        } else {
            printf("Error: Tidak dapat membuka file user_list.csv\n");
        }
    }

    printf("Antrian peminjaman: %d. %s\n", antrian.count, antrian.front->nama_user);

    // Bersihkan memori Linked List
    Book *current = book_list;
    while (current) {
        Book *temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}
