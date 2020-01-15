#ifndef graphicsdisplay_h
#define graphicsdisplay_h
#include <iostream>
#include <vector>
#include "window.h"
#include "state.h"
#include "observer.h"
#include "info.h"
#include "subject.h"

class Player;
class GraphicsDisplay: public Observer<Info, State> {
    int size;
    Xwindow x;
    
public:
    GraphicsDisplay();
    //Xwindow& getX();
	void fillRec(int x1, int y, int width, int height, int color);
    void drawStr(int x1, int y, std::string message);
    void notify(Subject<Info, State> &whoNotified) override;
};

#endif
