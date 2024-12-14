# Final Project Struktur Data
## Kelompok 7
Anggota Kelompok:
- CAEZARLOV NUGRAHA
- FAJAR RAHMAD JAYA
- MIFTAHUS SYIFAUL HAQQI
- SALSABILLA NUHAA AINI
- I KADEK RANGGA SANDI KUSUMA WARDANA

## Studi Kasus
- Sistem Manajemen Peminjaman Buku di Perpustakaan

## Languages
- C

## Pembagian Tugas
|Single Linked List (Cari Buku)| Status|
|------------------------------|-------|
|FAJAR RAHMAD JAYA             |[Selesai](https://github.com/CATSSRIN/Final-Project-Struktur-Data/blob/main/Check%20Data/Check%20Data.c)|

|Queue| Status|
|-----|-------|
|CAEZARLOV NUGRAHA (Peminjaman)||
|I KADEK RANGGA SANDI KUSUMA WARDANA (peminjamam)||
|MIFTAHUS SYIFAUL HAQQI (pengembalian)||

|Double Linked List (Riwayat Peminjaman)|Status|
|----------------------------|------|
|SALSABILLA NUHAA AINI       |  |
 
 ## Algoritma
 ```
printf("Cari buku")


printf("Peminjaman")
    printf("nama pemimjam
            id buku")
        printf("antrian")

printf("Pengembalian")
    printf("id buku yang ingin dikembalikan")


printf("Riwayat")
    printf("id buku yang ingin dicek:")

printf("Keluar")

__________________________________________________________________________________

cari buku   
    tampilkan semua
        (tampil semua)
    masukkan id buku    
        printf("%id_buku tersedia sebanyak: %stock")

Peminjaman
    nama Peminjaman
        id buku yang mau dipinjam:
            (cek %stock) (if else)
            antrian peminjaman: 
            (tambahkan user ke queue)
            printf(queue)
            (%stock - 1)
            (balik ke menu awal)

Pengembalian
    nama peminjam
        id buku yang ingin dikembalikan
        printf("buku telah dikembalikan")
        (hapus user dari queue)
        (%stock + 1)

Riwayat (double)
        riwayat semua peminjaman
            (%nama_user, %id_buku)
        id buku yang ingin dicek
            printf("%id_buku telah dipinjam oleh:" %nama_user)
```
