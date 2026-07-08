#ifndef LIBRARY_H
#define LIBRARY_H

#include "DataStructures.h"
#include <string>

class Library {
private:
    // --- IMPLEMENTASI ARRAY DATA UTAMA ---
    static const int MAX_BUKU = 100; // Batas maksimal array
    Buku daftarBuku[MAX_BUKU];       // Array dari struct Buku
    int jumlahBuku;                  // Mencatat jumlah buku saat ini

    // --- IMPLEMENTASI LINKED LIST (RIWAYAT LOG) ---
    LogNode* headLog;
    LogNode* tailLog;
    void tambahLog(std::string pesan); // Fungsi internal LL

    StackNode* topStack;
    QueueNode* frontQueue;
    QueueNode* rearQueue;
    const std::string FILE_NAME = "database.txt";

    void tambahBukuKeMemory(std::string id, std::string judul, std::string pengarang, int tahun);

public:
    Library();
    ~Library(); // Destructor untuk membersihkan memori heap
    
    // File Handling
    void simpanKeFile();
    void muatDariFile();

    // CRUD Utama (Berbasis Array)
    void tambahBuku();
    void tampilkanBuku();
    void updateBuku();
    void hapusBuku();

    // Searching & Sorting (Berbasis Array)
    void cariBuku();
    void urutkanBuku();

    // Stack (Undo)
    void pushStack(std::string id, std::string judul, std::string pengarang, int tahun);
    bool popStack(std::string &id, std::string &judul, std::string &pengarang, int &tahun);
    void undoHapus();

    // Queue (Antrean)
    void enqueue(std::string nama);
    void dequeue();
    void tampilkanAntrean();
    void menuAntrean();
    
    // Menampilkan Linked List Log
    void tampilkanLogAktivitas(); 
};

#endif