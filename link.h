// link.h
// Creates a Link object for each link with information such as type, strength, owner. location and ability casted. 

#ifndef link_h
#define link_h
#include <cstddef>
#include "subject.h"
#include "observer.h"
#include "info.h"
#include "state.h"
#include <iostream>

class GraphicsDisplay;
class Link: public Subject<Info, State>, public Observer<Info, State> {
    int row, col;
    int pnum;
    char name;
    int strength;
    int spaces;
    
    Direction getDir(int row, int col);
    Direction newDir(Direction dir);
    void helperNotifyMove(Subject<Info, State> &whoNotified);
    void helperNotifyReply(Subject<Info, State> &whoNotified);
    
public:
    Link(int row, int col, int pnum, char name, LinkType type, int strength, Ability ab, int spaces = 1);
    void notify(Subject<Info, State> &whoNotified) override;
    bool move(Direction d);
    void remove();
    Info getInfo() const override;
    
    void doAbility(Ability ab);
    
    friend std::ostream &operator<<(std::ostream &out, const Link &l);
};

#endif 
