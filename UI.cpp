#include "UI.h"
#include <iostream>
#include <thread>
#include <chrono>

// === CROSS-PLATFORM MACROS ===
#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
#endif

using namespace std;

UI::UI() {
    cursor = 0;
    lib.muatDariFile();
}

// === PERBAIKAN: KEMBALI MENGGUNAKAN SYSTEM("CLEAR") YANG 100% STABIL ===
void UI::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear"); 
#endif
}

int UI::getKeyPress() {
#ifdef _WIN32
    int ch = _getch();
    if (ch == 224 || ch == 0) { 
        ch = _getch();
        if (ch == 72) return 1; 
        if (ch == 80) return 2; 
    }
    if (ch == 13) return 3; 
    return 0; 
#else
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    if (ch == 27) { 
        if (getchar() == 91) {
            ch = getchar();
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            if (ch == 65) return 1; 
            if (ch == 66) return 2; 
            return 0;
        }
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    if (ch == 10) return 3; 
    return 0;
#endif
}

void UI::splashScreen() {
    string GREEN = "\033[32m";
    string RESET = "\033[0m";

    string dino1 = 
        "             __ \n"
        "            / _)\n"
        "     .-^^^-/ /  \n"
        "  __/       /   \n"
        "  <__.|_|-|_|   \n";

    string dino2 = 
        "             __ \n"
        "            / _)\n"
        "     .-^^^-/ /  \n"
        "  __/       /   \n"
        "  <__./|--|\\   \n";

    for (int i = 0; i < 15; i++) {
        clearScreen();
        cout << "\n\n\n\n\n"; 
        
        if (i % 2 == 0) cout << GREEN << dino1 << RESET;
        else cout << GREEN << dino2 << RESET;
        
        cout << "\n    Menyiapkan Sistem Perpustakaan";
        for (int j = 0; j < (i % 4); j++) cout << ".";
        cout << "\n";
        
        std::this_thread::sleep_for(std::chrono::milliseconds(200)); 
    }
    cout << "\n    Sistem Siap! Tekan [Enter] untuk mulai...";
    while(getKeyPress() != 3) {} 
}

void UI::animasiOmbak() {
    string BLUE = "\033[36m"; // Warna Cyan/Biru Air
    string RESET = "\033[0m";

    // Frame animasi ombak bergantian
    string frame1 = 
        "       ~         ~         ~         ~       \n"
        "    .     .   .     .   .     .   .     .    \n"
        "  ~         ~         ~         ~         ~  \n";

    string frame2 = 
        "    .     .   .     .   .     .   .     .    \n"
        "  ~         ~         ~         ~         ~  \n"
        "       ~         ~         ~         ~       \n";
        
    string frame3 = 
        "  ~         ~         ~         ~         ~  \n"
        "       ~         ~         ~         ~       \n"
        "    .     .   .     .   .     .   .     .    \n";

    for (int i = 0; i < 12; i++) {
        clearScreen();
        cout << "\n\n\n\n\n"; 
        
        // Mengganti frame berdasarkan sisa bagi (modulus)
        if (i % 3 == 0) cout << BLUE << frame1 << RESET;
        else if (i % 3 == 1) cout << BLUE << frame2 << RESET;
        else cout << BLUE << frame3 << RESET;
        
        cout << "\n    Memuat Data Samudra Perpustakaan";
        for (int j = 0; j < (i % 4); j++) cout << ".";
        cout << "\n";
        
        std::this_thread::sleep_for(std::chrono::milliseconds(250)); // Jeda 0.25 detik
    }
}

void UI::drawQueueMenu(int qCursor) {
    clearScreen();
    
    // Kode warna ANSI untuk Kuning
    string HIGHLIGHT = "\033[33m"; 
    string RESET = "\033[0m";

    cout << "=== KELOLA ANTREAN PENGUNJUNG ===\n\n";

    string qMenu[4] = {
        "1. Pengunjung Masuk Antrean",
        "2. Layani Pengunjung Terdepan",
        "3. Lihat Seluruh Antrean",
        "4. Kembali ke Menu Utama"
    };

    for (int i = 0; i < 4; i++) {
        if (i == qCursor) {
            // Sisipkan warna HIGHLIGHT (Kuning) saat kursor berada di menu ini
            cout << HIGHLIGHT << " > [ " << qMenu[i] << " ] < \n" << RESET; 
        } else {
            cout << "   " << qMenu[i] << "   \n";
        }
    }
    cout << "\nNavigasi: [^] Atas  [v] Bawah  [Enter] Pilih\n";
}
void UI::queueMenu() {
    int qCursor = 0;
    bool inQueueMenu = true;

    while (inQueueMenu) {
        drawQueueMenu(qCursor);
        int key = getKeyPress();

        if (key == 1) { 
            qCursor = (qCursor - 1 >= 0) ? qCursor - 1 : 3;
        } 
        else if (key == 2) { 
            qCursor = (qCursor + 1 <= 3) ? qCursor + 1 : 0;
        } 
        else if (key == 3) { 
            clearScreen();
            switch (qCursor) {
                case 0: {
                    string nama;
                    // === PERBAIKAN: Tambah flush agar terminal Mac tidak menahan teks ===
                    cout << "Masukkan Nama Pengunjung: " << flush;
                    cin >> ws;
                    getline(cin, nama);
                    lib.enqueue(nama);
                    break;
                }
                case 1: 
                    lib.dequeue(); 
                    break;
                case 2: 
                    lib.tampilkanAntrean(); 
                    break;
                case 3: 
                    inQueueMenu = false; 
                    break;
            }
            if (inQueueMenu) {
                cout << "\nTekan [Enter] untuk kembali ke menu antrean...";
                while(getKeyPress() != 3) {}
            }
        }
    }
}

void UI::drawMenu() {
    clearScreen();
    
    // Kode warna ANSI untuk Kuning
    string HIGHLIGHT = "\033[33m"; 
    string RESET = "\033[0m";

    cout << "=== SISTEM PERPUSTAKAAN DIGITAL ===\n\n";

    string menu[9] = {
        "1. Tambah Buku",
        "2. Tampilkan Buku",
        "3. Update Buku",
        "4. Hapus Buku",
        "5. Cari Buku",
        "6. Urutkan Buku",
        "7. Undo Hapus Buku",
        "8. Antrean Pengunjung",
        "9. Keluar"
    };

    for (int i = 0; i < 9; i++) {
        if (i == cursor) {
            // Sisipkan warna HIGHLIGHT (Kuning) saat kursor berada di menu ini
            cout << HIGHLIGHT << " > [ " << menu[i] << " ] < \n" << RESET; 
        } else {
            cout << "   " << menu[i] << "   \n";
        }
    }
    cout << "\nGunakan tombol panah Atas/Bawah dan tekan Enter.\n";
}

void UI::start() {
    splashScreen(); 
    animasiOmbak();

    bool running = true;
    while (running) {
        drawMenu();
        int key = getKeyPress();

        if (key == 1) { 
            cursor = (cursor - 1 >= 0) ? cursor - 1 : 8;
        } 
        else if (key == 2) { 
            cursor = (cursor + 1 <= 8) ? cursor + 1 : 0;
        } 
        else if (key == 3) { 
            clearScreen();
            switch (cursor) {
                case 0: lib.tambahBuku(); break;
                case 1: lib.tampilkanBuku(); break;
                case 2: lib.updateBuku(); break;
                case 3: lib.hapusBuku(); break;
                case 4: lib.cariBuku(); break;
                case 5: lib.urutkanBuku(); break;
                case 6: lib.undoHapus(); break;
                case 7: queueMenu(); break;
                case 8: 
                    cout << "👋 Terima kasih! Sistem dimatikan. Data aman tersimpan.\n";
                    running = false; 
                    break;
            }
            if (running && cursor != 7) {
                cout << "\nTekan [Enter] untuk kembali ke menu utama...";
                while(getKeyPress() != 3) {}
            }
        }
    }
}