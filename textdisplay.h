//
//  textdisplay.hpp
//  A5
//
//  Created by Yolanda on 2019-11-21.
//  Copyright © 2019 Yolanda. All rights reserved.
//

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


