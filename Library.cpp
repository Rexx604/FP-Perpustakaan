#include "Library.h"
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

Library::Library() {
    jumlahBuku = 0; // Inisialisasi jumlah buku di array
    headLog = nullptr; tailLog = nullptr; // Inisialisasi LL Log
    topStack = nullptr;
    frontQueue = nullptr; rearQueue = nullptr;
}

Library::~Library() {
    // Membebaskan memori Linked List Log
    LogNode* curLog = headLog;
    while (curLog != nullptr) {
        LogNode* temp = curLog;
        curLog = curLog->next;
        delete temp;
    }
    // Membebaskan memori Stack
    StackNode* curStack = topStack;
    while (curStack != nullptr) {
        StackNode* temp = curStack;
        curStack = curStack->next;
        delete temp;
    }
    // Membebaskan memori Queue
    QueueNode* curQueue = frontQueue;
    while (curQueue != nullptr) {
        QueueNode* temp = curQueue;
        curQueue = curQueue->next;
        delete temp;
    }
}

// === LINKED LIST LOG UTILITAS ===
void Library::tambahLog(string pesan) {
    LogNode* newNode = new LogNode();
    newNode->pesanAktivitas = pesan;
    newNode->next = nullptr;
    if (headLog == nullptr) {
        headLog = tailLog = newNode;
    } else {
        tailLog->next = newNode;
        tailLog = newNode;
    }
}

void Library::tampilkanLogAktivitas() {
    if (headLog == nullptr) {
        cout << "📌 Belum ada riwayat aktivitas sistem.\n";
        return;
    }
    LogNode* cur = headLog;
    cout << "=== RIWAYAT LOG AKTIVITAS SISTEM (LINKED LIST) ===\n";
    int no = 1;
    while (cur != nullptr) {
        cout << "[" << no++ << "] " << cur->pesanAktivitas << "\n";
        cur = cur->next;
    }
}

// === FILE HANDLING (ARRAY) ===
void Library::simpanKeFile() {
    ofstream file(FILE_NAME);
    if (!file.is_open()) {
        cout << "❌ Gagal membuka file untuk menyimpan data.\n";
        return;
    }
    for (int i = 0; i < jumlahBuku; i++) {
        file << daftarBuku[i].id << "|" << daftarBuku[i].judul << "|" 
             << daftarBuku[i].pengarang << "|" << daftarBuku[i].tahunTerbit << "\n";
    }
    file.close();
}

void Library::tambahBukuKeMemory(string id, string judul, string pengarang, int tahun) {
    if (jumlahBuku >= MAX_BUKU) {
        cout << "⚠️ Memori penyimpanan array penuh!\n";
        return;
    }
    daftarBuku[jumlahBuku].id = id;
    daftarBuku[jumlahBuku].judul = judul;
    daftarBuku[jumlahBuku].pengarang = pengarang;
    daftarBuku[jumlahBuku].tahunTerbit = tahun;
    jumlahBuku++;
}

void Library::muatDariFile() {
    ifstream file(FILE_NAME);
    if (!file.is_open()) return; 
    
    string id, judul, pengarang, line, txtTahun;
    while (getline(file, line)) {
        if (line.empty()) continue;
        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);
        size_t p3 = line.find('|', p2 + 1);
        
        if (p1 != string::npos && p2 != string::npos && p3 != string::npos) {
            id = line.substr(0, p1);
            judul = line.substr(p1 + 1, p2 - p1 - 1);
            pengarang = line.substr(p2 + 1, p3 - p2 - 1);
            txtTahun = line.substr(p3 + 1);
            
            int tahun = 0;
            try {
                tahun = stoi(txtTahun);
            } catch (...) {
                tahun = 0;
            }
            tambahBukuKeMemory(id, judul, pengarang, tahun);
        }
    }
    file.close();
}

// === CRUD UTAMA (ARRAY) ===
void Library::tambahBuku() {
    if (jumlahBuku >= MAX_BUKU) {
        cout << "❌ Gagal: Kapasitas penyimpanan penuh.\n";
        return;
    }
    string id, judul, pengarang;
    int tahun;
    
    cout << "Masukkan ID Buku        : " << flush; cin >> ws; getline(cin, id);
    cout << "Masukkan Judul Buku     : " << flush; getline(cin, judul);
    cout << "Masukkan Pengarang Buku : " << flush; getline(cin, pengarang);
    cout << "Masukkan Tahun Terbit   : " << flush; cin >> tahun;

    tambahBukuKeMemory(id, judul, pengarang, tahun);
    simpanKeFile();
    
    tambahLog("Menambahkan buku baru: " + judul + " [ID: " + id + "]");
    cout << "🎉 Buku berhasil ditambahkan!\n";
}

void Library::tampilkanBuku() {
    if (jumlahBuku == 0) {
        cout << "📌 Tidak ada data buku di dalam sistem.\n";
        return;
    }
    cout << "========================================================================================\n";
    cout << "| " << setw(8) << left << "ID Buku" 
         << "| " << setw(28) << left << "Judul Buku" 
         << "| " << setw(28) << left << "Pengarang" 
         << "| " << setw(12) << left << "Tahun Terbit" << " |\n";
    cout << "========================================================================================\n";
    
    for (int i = 0; i < jumlahBuku; i++) {
        cout << "| " << setw(8) << left << daftarBuku[i].id 
             << "| " << setw(28) << left << (daftarBuku[i].judul.length() > 26 ? daftarBuku[i].judul.substr(0, 24) + ".." : daftarBuku[i].judul)
             << "| " << setw(28) << left << (daftarBuku[i].pengarang.length() > 26 ? daftarBuku[i].pengarang.substr(0, 24) + ".." : daftarBuku[i].pengarang) 
             << "| " << setw(12) << left << daftarBuku[i].tahunTerbit << " |\n";
    }
    cout << "========================================================================================\n";
}

void Library::updateBuku() {
    string idTarget;
    cout << "Masukkan ID Buku yang akan diubah: " << flush; cin >> ws;
    getline(cin, idTarget);
    
    for (int i = 0; i < jumlahBuku; i++) {
        if (daftarBuku[i].id == idTarget) {
            cout << "--- Masukkan Data Baru ---\n";
            cout << "Judul Buku Baru     : " << flush; getline(cin, daftarBuku[i].judul);
            cout << "Pengarang Buku Baru : " << flush; getline(cin, daftarBuku[i].pengarang);
            cout << "Tahun Terbit Baru   : " << flush; cin >> daftarBuku[i].tahunTerbit;
            simpanKeFile();
            
            tambahLog("Mengubah data buku ID: " + idTarget);
            cout << "✏️ Data buku berhasil diperbarui!\n";
            return;
        }
    }
    cout << "❌ Buku dengan ID '" << idTarget << "' tidak ditemukan.\n";
}

void Library::hapusBuku() {
    string idTarget;
    cout << "Masukkan ID Buku yang akan dihapus: " << flush; cin >> ws;
    getline(cin, idTarget);

    for (int i = 0; i < jumlahBuku; i++) {
        if (daftarBuku[i].id == idTarget) {
            // Push ke stack untuk undo
            pushStack(daftarBuku[i].id, daftarBuku[i].judul, daftarBuku[i].pengarang, daftarBuku[i].tahunTerbit);
            
            tambahLog("Menghapus buku: " + daftarBuku[i].judul + " [ID: " + idTarget + "]");
            
            // Pergeseran elemen array ke kiri untuk menghapus data
            for (int j = i; j < jumlahBuku - 1; j++) {
                daftarBuku[j] = daftarBuku[j + 1];
            }
            jumlahBuku--;
            simpanKeFile();
            cout << "🗑️ Buku berhasil dihapus! (Aksi dapat dibatalkan melalui menu Undo).\n";
            return;
        }
    }
    cout << "❌ Buku dengan ID '" << idTarget << "' tidak ditemukan.\n";
}

// === SEARCHING & SORTING (ARRAY) ===
void Library::cariBuku() {
    string kataKunci;
    cout << "Masukkan Judul Buku yang dicari: " << flush; cin >> ws;
    getline(cin, kataKunci);

    bool ditemukan = false;
    for (int i = 0; i < jumlahBuku; i++) {
        if (daftarBuku[i].judul.find(kataKunci) != string::npos) {
            if (!ditemukan) {
                cout << "\n🔎 Hasil Pencarian Ditemukan:\n";
                ditemukan = true;
            }
            cout << "- ID: " << daftarBuku[i].id << " | " << daftarBuku[i].judul << " (" << daftarBuku[i].tahunTerbit << ")\n";
        }
    }
    if (!ditemukan) {
        cout << "\n❌ Buku dengan judul '" << kataKunci << "' tidak ditemukan.\n";
    }
}

void Library::urutkanBuku() {
    if (jumlahBuku <= 1) return;

    // Bubble Sort manual pada Array (Tanpa STL)
    for (int i = 0; i < jumlahBuku - 1; i++) {
        for (int j = 0; j < jumlahBuku - i - 1; j++) {
            if (daftarBuku[j].tahunTerbit > daftarBuku[j + 1].tahunTerbit) {
                // Swap manual entitas struct
                Buku temp = daftarBuku[j];
                daftarBuku[j] = daftarBuku[j + 1];
                daftarBuku[j + 1] = temp;
            }
        }
    }
    cout << "📊 Buku berhasil diurutkan berdasarkan Tahun Terbit (Tertua -> Terbaru).\n";
    tampilkanBuku();
}

// === STACK UNDO ===
void Library::pushStack(string id, string judul, string pengarang, int tahun) {
    StackNode* newNode = new StackNode();
    newNode->id = id; newNode->judul = judul;
    newNode->pengarang = pengarang; newNode->tahunTerbit = tahun;
    newNode->next = topStack;
    topStack = newNode;
}

bool Library::popStack(string &id, string &judul, string &pengarang, int &tahun) {
    if (topStack == nullptr) return false;
    StackNode* temp = topStack;
    id = temp->id; judul = temp->judul;
    pengarang = temp->pengarang; tahun = temp->tahunTerbit;
    topStack = topStack->next;
    delete temp;
    return true;
}

void Library::undoHapus() {
    string id, judul, pengarang;
    int tahun;
    if (popStack(id, judul, pengarang, tahun)) {
        tambahBukuKeMemory(id, judul, pengarang, tahun);
        simpanKeFile();
        tambahLog("Melakukan Undo: Mengembalikan buku " + judul);
        cout << "🔄 Undo Berhasil! Buku '" << judul << "' dikembalikan ke sistem.\n";
    } else {
        cout << "📌 Riwayat kosong. Tidak ada aksi penghapusan yang dapat dibatalkan.\n";
    }
}

// === QUEUE ANTREAN ===
void Library::enqueue(string nama) {
    QueueNode* newNode = new QueueNode();
    newNode->namaPengunjung = nama;
    newNode->next = nullptr;
    if (rearQueue == nullptr) {
        frontQueue = rearQueue = newNode;
    } else {
        rearQueue->next = newNode;
        rearQueue = newNode;
    }
    cout << "✅ " << nama << " berhasil masuk ke antrean.\n";
}

void Library::dequeue() {
    if (frontQueue == nullptr) {
        cout << "📌 Antrean kosong. Tidak ada pengunjung untuk dilayani.\n";
        return;
    }
    QueueNode* temp = frontQueue;
    cout << "📢 Melayani pengunjung: " << temp->namaPengunjung << "\n";
    frontQueue = frontQueue->next;
    if (frontQueue == nullptr) rearQueue = nullptr;
    delete temp;
}

void Library::tampilkanAntrean() {
    if (frontQueue == nullptr) {
        cout << "📌 Antrean saat ini kosong.\n";
        return;
    }
    QueueNode* cur = frontQueue;
    cout << "=== DAFTAR ANTREAN PENGUNJUNG ===\n";
    int no = 1;
    while (cur != nullptr) {
        cout << no++ << ". " << cur->namaPengunjung << "\n";
        cur = cur->next;
    }
}