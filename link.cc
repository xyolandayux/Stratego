#include "link.h"
#include "graphicsdisplay.h"

Link::Link(int row, int col, int pnum, char name, LinkType type, int strength, Ability ab, int spaces):
row{row}, col{col}, pnum{pnum}, name{name}, strength{strength}, spaces{spaces}{
    setState({type, ab, Direction::Middle, GameStatus::Unknown, Visible::Single, StateType::Null, Active::Active});
}


bool Link::move(Direction d){
    setState({getState().type, getState().ability, d, GameStatus::Unknown, getState().visible, StateType::Move, getState().active});
    notifyObservers();
    if (getState().status == GameStatus::Invalid) return false;
    switch(d){
        case Direction::Up:
            if (row - spaces < 0){
                if (pnum == 1) return false;
                else setState({getState().type, getState().ability, getState().direction, GameStatus::Winner, getState().visible, getState().state, getState().active});
            }
            row = row - spaces;
            break;
        case Direction::Down:
            if (row + spaces > 7){
                if (pnum == 2) return false;
                else setState({getState().type, getState().ability, getState().direction, GameStatus::Winner, getState().visible, getState().state, getState().active});
            }
            row = row + spaces;
            break;
        case Direction::Right:
            if (col + spaces > 7) return false;
            col = col + spaces;
            break;
        case Direction::Left:
            if (col - spaces < 0) return false;
            col = col - spaces;
            break;
        case Direction::Middle:
            return false;
            break;
    }
    return true;
}


void Link::remove(){
    setState({getState().type, getState().ability, getState().direction, getState().status, getState().visible, getState().state, Active::Inactive});
}


//gets the reverse direction for reply
Direction Link::newDir(Direction dir){
    Direction newdir;
    switch(dir){
        case Direction::Up:
            newdir = Direction::Down;
            break;
        case Direction::Down:
            newdir = Direction::Up;
            break;
        case Direction::Left:
            newdir = Direction::Right;
            break;
        case Direction::Right:
            newdir = Direction::Left;
            break;
        case Direction::Middle:
            newdir = Direction::Middle;
    }
    return newdir;
}

//gets the direction of this cell to the notified cell
Direction Link::getDir(int r, int c){
    Direction dir;
    if (r > row) dir = Direction::Up;
    else if (row == r){
        if (c > col) dir = Direction::Left;
        else dir = Direction::Right;
    } else dir = Direction::Down;
    return dir;
}


void Link::helperNotifyMove(Subject<Info, State> &whoNotified){
    bool same = pnum == whoNotified.getInfo().pnum;
    Direction otherDir = whoNotified.getState().direction;
    Ability ab = getState().ability;
    LinkType l = getState().type;
    if (same){
        if (ab == Ability::Firewall) return;
        setState({l, ab, newDir(otherDir), GameStatus::Invalid, getState().visible, StateType::Reply, getState().active});
    } else{
        if (ab == Ability::Server)
            setState({l, ab, newDir(otherDir), GameStatus::Winner, getState().visible, StateType::Reply, getState().active});
        else if (ab == Ability::Firewall){
            if (whoNotified.getState().type == LinkType::Virus)
                setState({l, ab, newDir(otherDir), GameStatus::Loser, Visible::Both, StateType::Reply, getState().active});
            else setState({l, ab, newDir(otherDir), GameStatus::Unknown, Visible::Both, StateType::Reply, getState().active});
        } else {
            if (strength > whoNotified.getInfo().strength)
                setState({l, ab, newDir(otherDir), GameStatus::Winner, Visible::Both, StateType::Reply, getState().active});
            else setState({l, ab, newDir(otherDir), GameStatus::Loser, Visible::Both, StateType::Reply, getState().active});
        }
    }
    notifyObservers();
}


void Link::helperNotifyReply(Subject<Info, State> &whoNotified){
    Direction otherDir = whoNotified.getState().direction;
    Ability ab = getState().ability;
    LinkType l = getState().type;
    Visible v = whoNotified.getState().visible;
    if (whoNotified.getState().status == GameStatus::Invalid)
        setState({l, ab, newDir(otherDir), GameStatus::Invalid, getState().visible, StateType::Null, getState().active});
    else if (whoNotified.getState().status == GameStatus::Loser)
        setState({l, ab, newDir(otherDir), GameStatus::Winner, v, StateType::Null, getState().active});
    else if (whoNotified.getState().status == GameStatus::Winner)
        setState({l, ab, newDir(otherDir), GameStatus::Loser, v, StateType::Null, getState().active});
    else if (whoNotified.getState().status == GameStatus::Unknown)
        setState({l, ab, otherDir, GameStatus::Unknown, v, StateType::Null, getState().active});
}


void Link::notify(Subject<Info, State> &whoNotified){
    if(getState().active == Active::Inactive) return;
    Direction otherDir = whoNotified.getState().direction;
    int otherR = whoNotified.getInfo().row;
    int otherC = whoNotified.getInfo().col;
    int move_spaces = whoNotified.getInfo().spaces;
    
    if (getDir(otherR, otherC) != otherDir) return;
    
    if (whoNotified.getState().state == StateType::Move){
        switch(otherDir){
                case Direction::Up:
                if (row + move_spaces == otherR && col == otherC) helperNotifyMove(whoNotified);
                break;
            case Direction::Down:
                if (row - move_spaces == otherR && col == otherC) helperNotifyMove(whoNotified);
                break;
            case Direction::Right:
                if (col - move_spaces == otherC && row == otherR) helperNotifyMove(whoNotified);
                break;
            case Direction::Left:
                if (col + move_spaces == otherC && row == otherR) helperNotifyMove(whoNotified);
                break;
            case Direction::Middle:
                
                break;
        }
    } else if(whoNotified.getState().state == StateType::Reply){
        switch(otherDir){
                case Direction::Up:
                if (row + spaces == otherR && col == otherC) helperNotifyReply(whoNotified);
                break;
            case Direction::Down:
                if (row - spaces == otherR && col == otherC) helperNotifyReply(whoNotified);
                break;
            case Direction::Right:
                if (col - spaces == otherC && row == otherR) helperNotifyReply(whoNotified);
                break;
            case Direction::Left:
                if (col + spaces == otherC && row == otherR) helperNotifyReply(whoNotified);
                break;
            case Direction::Middle:
                break;
        }
    }
}


Info Link::getInfo() const{
    Info info = {row, col, pnum, name, strength, spaces};
    return info;
}


void Link::doAbility(Ability ability){
    if (ability == Ability::Scan)
        setState({getState().type, getState().ability, getState().direction, getState().status, Visible::Both, getState().state, getState().active});
    else if (ability == Ability::Polarize){
        if(getState().type == LinkType::Virus)
            setState({LinkType::Data, getState().ability, getState().direction, getState().status, getState().visible, getState().state, getState().active});
        else if (getState().type == LinkType::Data)
            setState({LinkType::Virus, getState().ability, getState().direction, getState().status, getState().visible, getState().state, getState().active});
    }
    else if (ability == Ability::LinkBoost){
        spaces = 2;
        setState({getState().type, Ability::LinkBoost, getState().direction, getState().status, getState().visible, getState().state, getState().active});
    }
}


std::ostream &operator<<(std::ostream &out, const Link &l){
    if (LinkType::Virus == l.getState().type) out << "V";
    else out << "D";
    out << l.strength << " ";
    return out;
}
