# Final Project Struktur Data
## Kelompok 7
Anggota Kelompok:
- [CAEZARLOV NUGRAHA](https://github.com/CATSSRIN)
- [FAJAR RAHMAD JAYA](https://github.com/Fajar-RahmadJaya)
- [MIFTAHUS SYIFAUL HAQQI](https://github.com/MiftahusHaqqi)
- SALSABILLA NUHAA AINI
- I KADEK RANGGA SANDI KUSUMA WARDANA

## Studi Kasus
- Sistem Manajemen Peminjaman Buku di Perpustakaan

## Pembagian Tugas
|Single Linked List (Cari Buku)| Status|
|------------------------------|-------|
|FAJAR RAHMAD JAYA             |[Selesai](https://github.com/CATSSRIN/Final-Project-Struktur-Data/blob/main/Pembagian%20Tugas/Cari%20Buku/Cari%20Buku.c)|

|Queue| Status|
|-----|-------|
|CAEZARLOV NUGRAHA (Peminjaman)|[Selesai](https://github.com/CATSSRIN/Final-Project-Struktur-Data/blob/main/Pembagian%20Tugas/peminjaman/menu%20peminjaman.c)|
|I KADEK RANGGA SANDI KUSUMA WARDANA (peminjamam)|[Selesai](https://github.com/CATSSRIN/Final-Project-Struktur-Data/blob/main/Pembagian%20Tugas/peminjaman/menu%20peminjaman.c)|
|MIFTAHUS SYIFAUL HAQQI (pengembalian)|[Selesai](https://github.com/CATSSRIN/Final-Project-Struktur-Data/blob/main/Pembagian%20Tugas/Pengembalian/Menu%20Pengembalian.c)|

|Double Linked List (Riwayat Peminjaman)|Status|
|----------------------------|------|
|SALSABILLA NUHAA AINI       |[Selesai](https://github.com/CATSSRIN/Final-Project-Struktur-Data/blob/main/Pembagian%20Tugas/riwayat/riwayat%20peminjaman.c)|

## Progress Main Code
|Menu|Status|
|------|------|
|Cari buku| Selesai |
|Pinjam buku| Selesai |
|Antrian| Selesai |
|Riwayat| Selesai |
|Kembalikan buku| Selesai |

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
