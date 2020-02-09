// graphicsdisplay.cc
// Initialize the graphics dispaly with default setup up, and updates after each turn accordingly 
// using the MVC model by observing each link piece

#include "graphicsdisplay.h"
#include "state.h"
#include <iostream>
#include <string>

//constructor
GraphicsDisplay::GraphicsDisplay(){
    size = 50;  //size of each cell
    x.fillRectangle(0, 0, 420, 640, 0);
    int color;
    for (int r = 0; r < 8; r++){
        for (int c = 0; c < 8; c++){
            int n = r % 2;
            int n1 = c % 2;
            if (n == n1) color = 2;
            else color = 3;
            x.fillRectangle(c*size+10, r*size+100, size, size, color);
        }
    }
}


void GraphicsDisplay::fillRec(int x1, int y, int width, int height, int color){
    x.fillRectangle(x1, y, width, height, color);
}

void GraphicsDisplay::drawStr(int x1, int y, std::string message){
    x.drawString(x1, y, message);
}

//update the graphics
void GraphicsDisplay::notify(Subject<Info, State> &whoNotified){
    int r = whoNotified.getInfo().row;
    int c = whoNotified.getInfo().col;
    
    int spaces = whoNotified.getInfo().spaces;

    std::string name(1, whoNotified.getInfo().name);
    
    int color;
    
    int n = r % 2;
    int n1 = c % 2;
    if (n == n1) color = 3;
    else color = 2;
    
    if (whoNotified.getState().type == LinkType::Cells){
         x.drawString(c*size+25+10, r*size+25+100, name);
        return;
    }

    Direction d = whoNotified.getState().direction;
    
    if (d == Direction::Up) {if (r < 7) x.fillRectangle(c*size+10, (r + spaces)*size+100, size, size, color);}
    else if (d == Direction::Down) {if (r > 0) x.fillRectangle(c*size+10, (r-spaces)*size+100, size, size, color);}
    else if (d == Direction::Left) x.fillRectangle((c+spaces)*size+10, r*size+100, size, size, color);
    else if (d == Direction::Right) x.fillRectangle((c-spaces)*size+10, r*size+100, size, size, color);
    if (r > 7 || r < 0) return;
    if (r == 7 && (c == 3 || c == 4)) return;
    if (whoNotified.getState().status == GameStatus::Loser) return;

    if (color == 2) color = 3;
    else color = 2;
    if (whoNotified.getState().active == Active::Inactive){
       x.fillRectangle(c*size+10, r*size+100, size, size, color);
       return;
   }
    x.fillRectangle(c*size+10, r*size+100, size, size, color);
    
    x.drawString(c*size+25+10, r*size+25+100, name);
}
