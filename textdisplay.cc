//
//  textdisplay.cc
//  Displays an accurate representation of the game board after each turn in text format to stdout 

#include "textdisplay.h"
#include <vector>
using namespace std;

//construct the initial gameboard layout
TextDisplay::TextDisplay(){
    for (int i = 0; i < 8; i++){
        vector<char> v;
        for (int j = 0; j < 8; j++){
            v.push_back('*');
        }
        theDisplay.push_back(v);
    }
}

//notifies testdisplay so it changes the surface accordingly
void TextDisplay::notify(Subject<Info, State> &whoNotified){
    int r = whoNotified.getInfo().row;
    int c = whoNotified.getInfo().col;
    int spaces = whoNotified.getInfo().spaces;
    char ch = whoNotified.getInfo().name;
    

    if (whoNotified.getState().type == LinkType::Cells){
        theDisplay.at(r).at(c) = ch;
        return;
    }

    //if (whoNotified.getState().ability == Ability::No){
    Direction d = whoNotified.getState().direction;
    
    if (d == Direction::Up) {if (r < 7) theDisplay.at(r+spaces).at(c) = '*';}
    else if (d == Direction::Down) {if (r > 0) theDisplay.at(r-spaces).at(c) = '*';}
    else if (d == Direction::Left) theDisplay.at(r).at(c+spaces) = '*';
    else if (d == Direction::Right) theDisplay.at(r).at(c-spaces) = '*';
    //cout << "r: " << r;
    if (r > 7 || r < 0) return;
    if (r == 7 && (c == 3 || c == 4)) return;
    if (whoNotified.getState().status == GameStatus::Loser) return;
   if (whoNotified.getState().active == Active::Inactive){
       theDisplay.at(r).at(c) = '*';
       return;
   }
    
    theDisplay.at(r).at(c) = ch; //changing the label of the cell
   // }
}

//overrides the << operator, properly displays the game board
std::ostream &operator<<(std::ostream &out, const TextDisplay &td){
    vector<vector<char>> g = td.theDisplay;
    //if (td.pnum == 1){
        for (int i = 0; i < 8; i++){
            for (int j = 0; j < 8; j++){
                out << " " << g.at(i).at(j) << " ";
            }
            out << "\n";
        }
    return out;
}
