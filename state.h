//
//  state.h
//  A5
//
//  Created by Yolanda on 2019-11-21.
//  Copyright © 2019 Yolanda. All rights reserved.
//

#ifndef STATE_H
#define STATE_H

enum class LinkType { Data, Virus, Cells };
enum class Ability { No, Server, LinkBoost, Firewall, Download, Polarize, Scan, Trade, Next, Battle };
enum class Direction { Up, Down, Left, Right, Middle };
enum class GameStatus { Winner, Loser, Unknown, Invalid };
enum class Visible { Single, Both };
enum class StateType { Move, Reply, Null };
enum class Active { Active, Inactive };


struct State {
    LinkType type;  // See above
    Ability ability;
    Direction direction;  // In which direction from me is the new piece?
    GameStatus status;
    Visible visible;
    StateType state;
    Active active;
};

#endif
