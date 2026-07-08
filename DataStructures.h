#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <string>

// Struct Buku untuk ARRAY (Penyimpanan Data Utama)
struct Buku {
    std::string id;
    std::string judul;
    std::string pengarang;
    int tahunTerbit;
};

// Struct Node untuk LINKED LIST (Riwayat / Log Aktivitas)
struct LogNode {
    std::string pesanAktivitas;
    LogNode* next;
};

// Struct Node untuk Stack (Undo Riwayat Hapus)
struct StackNode {
    std::string id;
    std::string judul;
    std::string pengarang;
    int tahunTerbit;
    StackNode* next;
};

// Struct Node untuk Queue (Antrean Pengunjung)
struct QueueNode {
    std::string namaPengunjung;
    QueueNode* next;
};

#endif