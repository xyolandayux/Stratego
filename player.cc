//
//  player.cpp
//  A5
//
//  Created by Yolanda on 2019-11-21.
//  Copyright © 2019 Yolanda. All rights reserved.
//

#include "player.h"
#include "link.h"
#include "graphicsdisplay.h"
#include <string>
#include <iostream>
using namespace std;

Player::~Player(){
}

Player::Player(int pnum, string the_abilities): pnum{pnum}, virus{0}, data{0}{
    for(const char &ability: the_abilities){
        Ability ab;
        if (ability == 'L') ab = Ability::LinkBoost;
        else if (ability == 'F') ab = Ability::Firewall;
        else if (ability == 'D') ab = Ability::Download;
        else if (ability == 'S') ab = Ability::Scan;
        else if (ability == 'P') ab = Ability::Polarize;
        else if (ability == 'B') ab = Ability::Battle;
        else if (ability == 'T') ab = Ability::Trade;
        else if (ability == 'N') ab = Ability::Next;
        
        abilities.push_back(make_pair(ab, 1));
    }
}


void Player::download(Link* link){
    if (link->getState().type == LinkType::Virus) virus++;
    else if (link->getState().type == LinkType::Data) data++;
    link->remove();
}

Ability Player::getAbility(int n){
    if (abilities.at(n).second == 1) return abilities.at(n).first;
    else return Ability::No;
}

void Player::removeAb(int n){
    abilities.at(n).second = 0;
}

void Player::displayAbilities(){
    cout << "Abilities: \n";
    for(int i = 0; i < 5; i++){
        Ability ability = abilities.at(i).first;
        char cur_ab;
        if(ability == Ability::LinkBoost) cur_ab = 'L';
        else if(ability == Ability::Firewall) cur_ab = 'F';
        else if(ability == Ability::Download) cur_ab = 'D';
        else if(ability == Ability::Scan) cur_ab = 'S';
        else if(ability == Ability::Polarize) cur_ab = 'P';
        else if(ability == Ability::Battle) cur_ab = 'B';
        else if(ability == Ability::Next) cur_ab = 'N';
        else if(ability == Ability::Trade) cur_ab = 'T';
        else cur_ab = ' ';
        cout << cur_ab << "(" << i + 1 << ") ";
        if (abilities.at(i).second == 0) cout << ": Used ";
    }
    cout << endl;
}

//std::vector<Ability> Player::get_abilities(){
//    return abilities;
//}
//std::vector<Ability> Player::get_og_abilities(){
//    return og_abilities;
//}

//void Player::displayAbilities(){
//    cout << "Original abilities: ";
//    for(int i = 0; i < og_abilities.size(); i++){
//        Ability ability = og_abilities.at(i);
//        char cur_ab;
//        if(ability == Ability::LinkBoost) cur_ab = 'L';
//        else if(ability == Ability::Firewall) cur_ab = 'F';
//        else if(ability == Ability::Download) cur_ab = 'D';
//        else if(ability == Ability::Scan) cur_ab = 'S';
//        else if(ability == Ability::Polarize) cur_ab = 'P';
//        else cur_ab = ' ';
//
//        if(ability != Ability::No){
//            cout << cur_ab << "(" << i + 1 << ") ";
//        }
//    }
//    cout << endl;
//    cout << "Available abilities: ";
//    for(int i = 0; i < abilities.size(); i++){
//        Ability ability = abilities.at(i);
//        char cur_ab;
//        if(ability == Ability::LinkBoost) cur_ab = 'L';
//        else if(ability == Ability::Firewall) cur_ab = 'F';
//        else if(ability == Ability::Download) cur_ab = 'D';
//        else if(ability == Ability::Scan) cur_ab = 'S';
//        else if(ability == Ability::Polarize) cur_ab = 'P';
//        else cur_ab = ' ';
//
//        if(ability != Ability::No){
//            cout << cur_ab << "(" << i + 1 << ") ";
//        }
//    }
//    cout << endl;
//
//}

void Player::drawInfo(GraphicsDisplay *gd, int num){
    if (this->pnum == num + 1){
        gd->drawStr(30, 600, "All Abilities:");
    
        int x = 30;
        
        for(int i = 0; i < 5; i++){
            Ability ability = abilities.at(i).first;
            string cur_ab;
            if(ability == Ability::LinkBoost) cur_ab = "L";
            else if(ability == Ability::Firewall) cur_ab = "F";
            else if(ability == Ability::Download) cur_ab = "D";
            else if(ability == Ability::Scan) cur_ab = "S";
            else if(ability == Ability::Polarize) cur_ab = "P";
            else if(ability == Ability::Battle) cur_ab = "B";
            else if(ability == Ability::Next) cur_ab = "N";
            else if(ability == Ability::Trade) cur_ab = "T";
            else cur_ab = " ";
            
            if (abilities.at(i).second == 0)
            gd->drawStr(x, 620, cur_ab + "(" + to_string(i + 1) + ") " + ": Used ");
            else gd->drawStr(x, 620, cur_ab + "(" + to_string(i + 1) + ") ");
            
            x+=65;
        }
    }

    int count = 0;
    for (auto i: abilities){
        if (i.second == 1) count ++;
    }
    
    if (pnum == 1){
        gd->drawStr(30, 30, "Player" + to_string(pnum) + ":");
        gd->drawStr(30, 50, "Downloaded: " + to_string(data) + "D, " + to_string(virus) + "V");
        gd->drawStr(30, 70, "Abilities: " + to_string(count));
    } else {
        gd->drawStr(30, 530, "Player" + to_string(pnum) + ":");
        gd->drawStr(30, 550, "Downloaded: " + to_string(data) + "D, " + to_string(virus) + "V");
        gd->drawStr(30, 570, "Abilities: " + to_string(count));
    }
}

int Player::winner(){
    if (pnum == 1){
        if (virus >= 4) return 2;
        if (data >= 4) return 1;
        else return 0;
    } else {
        if (virus >= 4) return 1;
        if (data >= 4) return 2;
        else return 0;
    }
}

std::ostream &operator<<(std::ostream &out, const Player &p){
    int count = 0;
    for (auto i: p.abilities){
        if (i.second == 1) count ++;
    }
    
    out << "Player " << p.pnum << ":\n";
    out << "Downloaded: " << p.data << "D, " << p.virus << "V\n";
    out << "Abilities: " << count << "\n";
    return out;
}

