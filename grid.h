// grid.h
// Sets up the Grid, keep track of where all the links are.

#ifndef GRID_H
#define GRID_H
#include <iostream>
#include <vector>
#include <cstddef>
#include <utility>
#include "state.h"
#include "player.h"

class GraphicsDisplay;
class TextDisplay;
class Link;
template <typename InfoType, typename StateType> class Observer;
class InvalidMove{};

class Grid {
    std::vector<Link*> onelinks;
    std::vector<Link*> twolinks;
    
    TextDisplay *td = nullptr; // The text display.
    
    GraphicsDisplay *gd = nullptr;  //the graphics display
    Player one, two;
    int turns;   //keeps track of whos turn it is
    bool ability_used;
    
    bool win();
    void InvalidInput();
    bool moveHelper(char c, Direction d, std::vector<Link*> &myLinks, std::vector<Link*> &otherLinks, Player& me, Player& other);
    
    bool Firewall(int r, int c);
    bool oneCharAbility(Ability ab, Link *link, Player &cur_player);
    bool twoCharAbility(Ability ab, Link *myLink, Link *otherLink, Player &me, Player &other);
    void graphics(int myY, int otherY, std::vector<Link*> &myLinks, std::vector<Link*> &otherLinks);
    Link* findLink(char name, std::vector<Link*> link);
    
public:
    ~Grid();
    Grid(std::string ability1, std::string ability2, std::string link1, std::string link2, bool graphics_enabled);
    
    bool move(char c, std::string dir);
    bool useAbility(int ab, char first = 0, char second = 0);
    void displayAbilities();

    friend std::ostream &operator<<(std::ostream &out, const Grid &g);
};

#endif

