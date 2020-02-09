//
//  state.h
//  Properties of observer objects

#ifndef STATE_H
#define STATE_H

enum class LinkType { Data, Virus, Cells }; 
enum class Ability { No, Server, LinkBoost, Firewall, Download, Polarize, Scan, Trade, Next, Battle }; //special abilities
enum class Direction { Up, Down, Left, Right, Middle };   //in realtion to the newly moved link
enum class GameStatus { Winner, Loser, Unknown, Invalid };  //result after battling
enum class Visible { Single, Both };
enum class StateType { Move, Reply, Null };      
enum class Active { Active, Inactive };  //inactive once a link is downloaded


struct State {
    LinkType type;  
    Ability ability;
    Direction direction;
    GameStatus status;
    Visible visible;
    StateType state;
    Active active;
};

#endif
