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
