// grid.cc
// Initilize the game board with default setup of links in set positions. 
// Checks validity of moves and usage of abilities.
// Interact with both the Player and Link class for high cohesiveness

#include <utility>
#include "grid.h"
#include "textdisplay.h"
#include "graphicsdisplay.h"
#include "link.h"
#include <sstream>
#include <string>
using namespace std;

//destructor
Grid::~Grid(){
    delete td;
    delete gd;
    for (auto i : onelinks){
        delete i;
    }
    for (auto i : twolinks){
        delete i;
    }
}

Grid::Grid(string ability1, string ability2, string link1, string link2, bool graphics_enabled): one{1, ability1}, two{2, ability2}, turns{0}, ability_used{false}{
    onelinks.clear();
    twolinks.clear();
    delete td;
    delete gd;
    td = new TextDisplay(); //set up the textdisplay
    if(graphics_enabled) gd = new GraphicsDisplay();
    else gd = nullptr;
    
    stringstream ss;
    ss << link1;
    char cur_piece;
    int cur_strength;
    LinkType linktype;
    for(int i = 0; i < 8; i++){
        ss >> cur_piece;
        ss >> cur_strength;
        if(cur_piece == 'D'){
            linktype = LinkType::Data;
        } else {
            linktype = LinkType::Virus;
        }
        if(i == 3 || i == 4){
            onelinks.push_back(new Link(1, i, 1, 'a' + i, linktype, cur_strength, Ability::No));
        } else {
            onelinks.push_back(new Link(0, i, 1, 'a' + i, linktype, cur_strength, Ability::No));
        }
    }
    
    ss.clear();
    ss << link2;
    for(int i = 0; i < 8; i++){
        ss >> cur_piece;
        ss >> cur_strength;
        if(cur_piece == 'D'){
            linktype = LinkType::Data;
        } else {
            linktype = LinkType::Virus;
        }
        if(i == 3 || i == 4){
            twolinks.push_back(new Link(6, i, 2, 'A' + i, linktype, cur_strength, Ability::No));
        } else {
            twolinks.push_back(new Link(7, i, 2, 'A' + i, linktype, cur_strength, Ability::No));
        }
    }
    
    onelinks.push_back(new Link(0, 3, 1, 'S', LinkType::Cells, 0, Ability::Server));
    onelinks.push_back(new Link(0, 4, 1, 'S', LinkType::Cells, 0, Ability::Server));
    twolinks.push_back(new Link(7, 3, 2, 'S', LinkType::Cells, 0, Ability::Server));
    twolinks.push_back(new Link(7, 4, 2, 'S', LinkType::Cells, 0, Ability::Server));
    
    
    for (auto& i : onelinks){
        td->notify(*i);
        if(gd) gd->notify(*i);
        for (auto& j : onelinks){
            if (i != j) i->attach(j);
        }
        for (auto& k : twolinks){
            i->attach(k);
        }
    } for (auto& i : twolinks){
        td->notify(*i);
        if(gd) gd->notify(*i);
        for (auto& j : twolinks){
            if (i != j) i->attach(j);
        }
        for (auto& k : onelinks){
            i->attach(k); 
        }
    }
    cout << *this;
    if(gd) graphics(40, 540, onelinks, twolinks);
}


bool Grid::move(char c, string dir){
    Direction d;
    if (dir == "up") d = Direction::Up;
    else if (dir == "down") d = Direction::Down;
    else if (dir == "right") d = Direction::Right;
    else if (dir == "left") d = Direction::Left;
    else {
        InvalidInput();
        return false;
    }
    if (turns == 0){
        if(moveHelper(c, d, onelinks, twolinks, one, two)){
            if (win()) return true;
            turns = 1;
            ability_used = false;
            cout << *this;
            if(gd) graphics(540, 40, twolinks, onelinks);
        }
    } else {
        if(moveHelper(c, d, twolinks, onelinks, two, one)){
            if (win()) return true;
            turns = 0;
            ability_used = false;
            cout << *this;
            if(gd) graphics(40, 540, onelinks, twolinks);
        }
    }
    return false;
}


bool Grid::moveHelper(char c, Direction d, vector<Link*> &myLinks, vector<Link*> &otherLinks,
                      Player& me, Player& other){
    Link *link = findLink(c, myLinks);
    
    if (!link) InvalidInput();
    else if (link->getState().active == Active::Inactive) InvalidInput();
    else if (link->move(d)){
        td->notify(*link);
        if(gd) gd->notify(*link);
        if (link->getState().status == GameStatus::Winner) {
            Link *l = nullptr;
            for(auto& i : otherLinks){
                if (i->getState().status == GameStatus::Loser){
                    if(i->getState().active == Active::Inactive) continue;
                    if (i->getState().type == LinkType::Cells) continue;
                    else {
                        cout << " lose ";
                        cout << "  " << i->getInfo().name << "  ";
                        l = i;
                        me.download(i);
                    }
                }
            } if (!l){
                cout << " lose2 ";
                me.download(link);
            }
        } else if (link->getState().status == GameStatus::Loser){
            other.download(link);
        }
        return true;
    } else InvalidInput();
    return false;
}

Link* Grid::findLink(char name, std::vector<Link*> link){
    Link* theLink = nullptr;
    for(auto& i : link){
        if (i->getInfo().name == name) theLink = i;
    }
    return theLink;
}

bool Grid::useAbility(int ab, char first, char second){
    if (ab < 0 || ab > 4){
        InvalidInput();
        return false;
    }
    Player *cur_player;
    Player *other;
    if(turns == 0){
        cur_player = &one;
        other = &two;
    } else {
        cur_player = &two;
        other = &one;
    }
    
    Ability ability = cur_player->getAbility(ab);
    if (ability == Ability::No){
        InvalidInput();
        return false;
    }

    if(ability_used){
        cout << "You've already used an ability, try again!\n";
        return false;
    }
    
    if(first != 0 && second != 0){
        if(first >= '0' && first <= '7' && second >= '0' && second <= '7'){
            if (ability == Ability::Firewall){
                if(Firewall(first - '0', second - '0')){
                    ability_used = true;
                    cur_player->removeAb(ab);
                    cout << *this;
                }
            }
        } else if(((first >= 'a' && first <= 'h') || (first >= 'A' && first <= 'H')) && ((second >= 'a' && second <= 'h') || (second >= 'A' && second <= 'H'))){
            if (ability == Ability::Battle || ability == Ability::Trade){
                Link *link1 = findLink(first, onelinks);
                Link *link2 = findLink(second, onelinks);
                if (!link1) link1 = findLink(first, twolinks);
                if (!link2) link2 = findLink(second, twolinks);
                if (!link1) InvalidInput();
                else if (!link2) InvalidInput();
                else if (!(link1->getState().type == LinkType::Virus || link1->getState().type == LinkType::Data)) InvalidInput();
                else if (!(link2->getState().type == LinkType::Virus || link2->getState().type == LinkType::Data)) InvalidInput();
                else if (twoCharAbility(ability, link1, link2, *cur_player, *other)){
                    ability_used = true;
                    cur_player->removeAb(ab);
                    if (win()) return true;
                    cout << *this;
                }
            } else InvalidInput();
        } else InvalidInput();
    } else if (first != 0){
        if(((first >= 'a' && first <= 'h') || (first >= 'A' && first <= 'H'))){
            if (ability == Ability::Scan || ability == Ability::Download || ability == Ability::Polarize || ability == Ability::LinkBoost){
                Link *link = findLink(first, onelinks);
                if (!link) link = findLink(first, twolinks);
                if (!link) InvalidInput();
                else if (!(link->getState().type == LinkType::Virus || link->getState().type == LinkType::Data)) InvalidInput();
                else if (oneCharAbility(ability, link, *cur_player)){
                    cur_player->removeAb(ab);
                    ability_used = true;
                    if (win()) return true;
                    cout << *this;
                }
            } else InvalidInput();
        } else InvalidInput();
    } else {
        if (ability == Ability::Next){
            if(turns == 0){
                turns = 1;
                ability_used = false;
            } else {
                turns = 0;
                ability_used = false;
            }
        } else InvalidInput();
    }
    return false;
}

bool Grid::oneCharAbility(Ability ab, Link *link, Player &me){
    int pnum = link->getInfo().pnum;
    if (ab == Ability::Download){
        if ((turns == 0 && pnum == 2) || (turns == 1 && pnum == 1)){
            me.download(link);
            td->notify(*link);
            if(gd) gd->notify(*link);
            return true;
        } else {
            InvalidInput();
            return false;
        }
    } else {
        link->doAbility(ab);
        return true;
    }
}


bool Grid::twoCharAbility(Ability ab, Link *myLink, Link *otherLink, Player &me, Player &other){
    int myPnum = myLink->getInfo().pnum;
    int otherPnum = otherLink->getInfo().pnum;
    
    if((turns == 0 && myPnum == 1 && otherPnum == 2) || (turns == 1 && myPnum == 2 && otherPnum == 1)){
        if (ab == Ability::Trade){
            bool found = false;
            Link *tmp = myLink;
            //cout <<" " << myLink->getInfo().name<< " **";
            //myLink = otherLink;
            //cout <<" " << myLink->getInfo().name<< " **";
            //otherLink = tmp;
            for (auto i : onelinks){
                if (myLink == i){
                    i = otherLink;
                    found = true;
                }
            } if (found){
                for (auto i : twolinks){
                    if (otherLink == i){
                        i = tmp;
                    }
                }
            } if (!found){
                for (auto i : twolinks){
                    if (myLink == i){
                        i = otherLink;
                        found = true;
                    }
                //cout <<" " << i->getInfo().name<< " ";
                } for (auto i : onelinks){
                    if (otherLink == i){
                        i = tmp;
                    }
                }
            } 
        } else if (ab == Ability::Battle){
            int strength1 = myLink->getInfo().strength;
            int strength2 = otherLink->getInfo().strength;
            if (strength1 >= strength2){
                me.download(otherLink);
                td->notify(*otherLink);
                if(gd) gd->notify(*otherLink);
            } else{
                other.download(myLink);
                td->notify(*myLink);
                if(gd) gd->notify(*myLink);
            }
        }
        return true;
    }
    else return false;
}

bool Grid::Firewall(int r, int c){
    for (auto link : onelinks) {
        if (link->getInfo().row == r && link->getInfo().col == c) {
            InvalidInput();
            return false;
        }
    }
    for (auto link : twolinks) {
        if (link->getInfo().row == r && link->getInfo().col == c) {
            InvalidInput();
            return false;
        }
    }
    Link* newlink = nullptr;
    if(turns == 0){
        newlink = new Link(r, c, 1, 'm', LinkType::Cells, 0, Ability::Firewall);
        onelinks.push_back(newlink);
    } else if(turns == 1){
        newlink = new Link(r, c, 1, 'w', LinkType::Cells, 0, Ability::Firewall);
        twolinks.push_back(newlink);
    }
    
    td->notify(*newlink);
    if(gd) gd->notify(*newlink);

    for (auto &i : onelinks) {
        if (i != newlink){
            i->attach(newlink);
            newlink->attach(i);
        }
    }
    for (auto &j : twolinks) {
        if (j != newlink){
            j->attach(newlink);
            newlink->attach(j);
        }
    }
    return true;
}

        
void Grid::displayAbilities(){
    if(turns == 0){
        one.displayAbilities();
    } else {
        two.displayAbilities();
    }
}

bool Grid::win(){
    if (one.winner() != 0){
        if(gd) gd->fillRec(145, 270, 120, 60, 4);
        if(gd) gd->drawStr(165, 300, "Player " + to_string(one.winner()) + " wins!");
        cout << "Player " << one.winner() << " wins!" << endl;
        return true;
    } else if (two.winner() != 0){
        if(gd) gd->fillRec(145, 270, 120, 60, 4);
        if(gd) gd->drawStr(165, 300, "Player " + to_string(two.winner()) + " wins!");
        cout << "Player " << two.winner() << " wins!" << endl;
        return true;
    } else return false;
}


void Grid::InvalidInput(){
    cerr << "Invalid Input, Try Again" << endl;
}

void Grid::graphics(int myY, int otherY, vector<Link*> &myLinks, vector<Link*> &otherLinks){
    gd->fillRec(0, 0, 400, 100, 0);
    gd->fillRec(0, 500, 400, 140, 0);
    one.drawInfo(gd, turns);
    int count = 0;
    int x = 230;
    for (auto& i : myLinks){
        if (count == 8) break;
        if (count == 4){
            myY+=20;
            x = 230;
        }
        string name(1, i->getInfo().name);
        string t;
        if (LinkType::Virus == i->getState().type) t = "V";
        else t = "D";
        int strength = i->getInfo().strength;
        // string message
        gd->drawStr(x, myY, name + ": " + t + to_string(strength) + "  ");
        count++;
        x+=35;
    }
    x = 230;
    count = 0;
    for (auto& i : otherLinks){
        if (count == 8) break;
        if (count == 4){
            otherY+=20;
            x = 230;
        }
        string name(1, i->getInfo().name);
        if (Visible::Both == i->getState().visible){
            string t;
            if (LinkType::Virus == i->getState().type) t = "V";
            else t = "D";
            int strength = i->getInfo().strength;
            gd->drawStr(x, otherY, name + ": " + t + to_string(strength) + "  ");
        } else gd->drawStr(x, otherY, name + ": " + "? ");
        count++;
        x+=35;
    }
    two.drawInfo(gd, turns);
}

std::ostream &operator<<(std::ostream &out, const Grid &g){
    if (g.turns == 0){
        out << g.one;
        int count = 0;
        for (auto& i : g.onelinks){
            if (count == 8) break;
            if (count == 4) out << "\n";
            out << i->getInfo().name << ": ";
            out << *(i);
            count++;
        }
        out << "\n\n ====================== \n";
        out << *(g.td);
        out << " ====================== \n\n";
        out << g.two;
        count = 0;
        for (auto& i : g.twolinks){
            if (count == 8) break;
            if (count == 4) out << "\n";
            out << i->getInfo().name << ": ";
            if (Visible::Both == i->getState().visible) out << *(i);
            else out << "? ";
            count++;
        }
    } else {
        out << g.one;
        int count = 0;
        for (auto& i : g.onelinks){
            if (count == 8) break;
            if (count == 4) out << "\n";
            out << i->getInfo().name << ": ";
            if (Visible::Both == i->getState().visible) out << *(i);
            else out << "? ";
            count++;
        }
        out << "\n\n ====================== \n";
        out << *(g.td);
        out << " ====================== \n\n";
        out << g.two;
        count = 0;
        for (auto& i : g.twolinks){
            if (count == 8) break;
            if (count == 4) out << "\n";
            out << i->getInfo().name << ": ";
            out << *(i);
            count++;
        }
    }
    out << "\n" << endl;
    return out;
}
