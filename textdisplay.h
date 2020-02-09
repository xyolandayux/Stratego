//  textdisplay.h
//  Header file for TextDisplay class which utilizes MVC to output a model of the gameboard after each turn to stdout

#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H
#include <iostream>
#include <vector>
#include "subject.h"
#include "observer.h"
#include "state.h"
#include "info.h"

class TextDisplay: public Observer<Info, State> {
    std::vector<std::vector<char>> theDisplay;
    //const int gridSize;
    //int pnum;
    
public:
    TextDisplay();
    //void setPlayerView(int pnum);
    void notify(Subject<Info, State> &whoNotified) override;
    
    friend std::ostream &operator<<(std::ostream &out, const TextDisplay &td);
};
#endif


