//
//  player.h
//  Header for the Player class that holds player information


#ifndef player_h
#define player_h
#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include "state.h"

class GraphicsDisplay;
class Link;

class Player{
    int pnum;
    int virus = 0;
    int data = 0;
    std::vector<std::pair<Ability, int>> abilities;

public:
    ~Player();
    Player(int pnum, std::string the_abilities);
    void download(Link* link);
    Ability getAbility(int n);
    void removeAb(int n);
    void displayAbilities();
    int winner();
    void drawInfo(GraphicsDisplay *gd, int num);
    
    friend std::ostream &operator<<(std::ostream &out, const Player &p);
};

#endif 
