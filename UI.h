#ifndef UI_H
#define UI_H

#include "Library.h"

class UI {
private:
    Library lib;
    int cursor; 
    void drawMenu();
    void clearScreen();
    int getKeyPress(); 
    void splashScreen(); 
    void animasiOmbak();
    void drawQueueMenu(int qCursor); 
    void queueMenu();

public:
    UI();
    void start();
};

#endif