#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct BorrowData {
    char nama[50];
    char idBuku[10];
    struct BorrowData* next;
} BorrowData;

BorrowData* createBorrowData(const char* nama, const char* idBuku) {
    BorrowData* newNode = (BorrowData*)malloc(sizeof(BorrowData));
    if (!newNode) {
        fprintf(stderr, "Memory allocation failed\n");
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
        fprintf(stderr, "Failed to open file: %s\n", filename);
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
        printf("No borrow records available.\n");
        return;
    }

    printf("\nList of Borrowed Books:\n");
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
        fprintf(stderr, "Failed to open file: %s\n", filename);
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
        fprintf(stderr, "Failed to open files for update operation\n");
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
        printf("Updated jumlahBuku for ID %s in %s.\n", idBuku, dataFilename);
    } else {
        remove("temp.csv");
        printf("Book ID %s not found in %s.\n", idBuku, dataFilename);
    }
}

void returnBorrowedBook(BorrowData** head, const char* idBuku, const char* returnFile, const char* dataFile) {
    BorrowData *temp = *head, *prev = NULL;

    while (temp != NULL && strcmp(temp->idBuku, idBuku) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Borrow record with ID %s not found.\n", idBuku);
        return;
    }

    FILE* file = fopen(returnFile, "a");
    if (!file) {
        fprintf(stderr, "Failed to open file: %s\n", returnFile);
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

    printf("Borrow record with ID %s has been returned and updated.\n", idBuku);
}

int main() {
    BorrowData* borrowList = NULL;
    const char* borrowFile = "user_list.csv";
    const char* returnFile = "pengembalian.csv";
    const char* dataFile = "data.csv";

    readBorrowFile(borrowFile, &borrowList);

    int choice;
    char idBuku[10];

    while (1) {
        printf("\nMenu:\n");
        printf("1. Show all borrowed books\n");
        printf("2. Return a book\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); 
            continue;
        }

        switch (choice) {
            case 1: {

                displayBorrowData(borrowList);
                break;
            }
            case 2: {

                printf("Enter the ID of the book to return: ");
                scanf("%s", idBuku);

                returnBorrowedBook(&borrowList, idBuku, returnFile, dataFile); 
                saveBorrowData(borrowFile, borrowList); 
                break;
            }
            case 3: {

                printf("Exiting program. Freeing memory...\n");
                BorrowData* temp;
                while (borrowList != NULL) {
                    temp = borrowList;
                    borrowList = borrowList->next;
                    free(temp);
                }
                return 0;
            }
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}
